#include "gerenciador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Auxiliar pra achar vértice por URL
int cmp_url(void *a, void *b) {
    Site *s = (Site *)a;
    char *url = (char *)b;
    return strcmp(s->url, url);
}

void Buscador_carregarDados(Graph *g, IndiceInvertido *ind, const char *nome_arq_sites, const char *nome_arq_links) {
    FILE *arq_sites = fopen(nome_arq_sites, "r");
    if (!arq_sites) {
        printf("[ERRO] Nao rolou abrir o arquivo %s\n", nome_arq_sites);
        return;
    }

    char linha[512]; 
    printf("Puxando sites pra memoria...\n");
    
    while (fgets(linha, sizeof(linha), arq_sites)) {
        int id;
        char url[150], nome[100];

        if (sscanf(linha, "%d %s %s", &id, url, nome) < 3) continue;

        Site *novo_site = (Site *)malloc(sizeof(Site));
        if (!novo_site) {
            printf("[ERRO] Faltou RAM pro site %s.\n", nome);
            fclose(arq_sites);
            return;
        }

        strcpy(novo_site->url, url);
        strcpy(novo_site->nome, nome);
        novo_site->importancia = 1.0; 
        
        // Array de palavras da struct do Henrique
        novo_site->palavras = NULL;  
        novo_site->qtd_palavras = 0; 

        Graph_insertVertex(g, id, novo_site);

        // Pega o vértice criado pra jogar no índice
        Vertex *v_site = Graph_findVertexByValue(g, url, cmp_url);

        char *token = strtok(linha, " \r\n"); 
        token = strtok(NULL, " \r\n");        
        token = strtok(NULL, " \r\n");        

        token = strtok(NULL, " \r\n");
        while (token) {
            // Realoca o array e copia a string nova
            novo_site->palavras = realloc(novo_site->palavras, (novo_site->qtd_palavras + 1) * sizeof(char *));
            novo_site->palavras[novo_site->qtd_palavras] = malloc(strlen(token) + 1);
            strcpy(novo_site->palavras[novo_site->qtd_palavras], token);
            
            novo_site->qtd_palavras++; 

            if (ind && v_site) {
                Indice_inserirPalavra(ind, token, v_site);
            }
            
            token = strtok(NULL, " \r\n"); 
        }
    }
    fclose(arq_sites);

    FILE *arq_links = fopen(nome_arq_links, "r");
    if (!arq_links) {
        printf("[ERRO] Nao rolou abrir o arquivo %s\n", nome_arq_links);
        return;
    }

    int orig, dest;
    printf("Montando as arestas (links)...\n");
    while (fscanf(arq_links, "%d %d", &orig, &dest) == 2) {
        Graph_insertEdge(g, orig, dest, NULL);
    }
    fclose(arq_links);
    
    Buscador_recalcularRanking(g);

    printf("[SUCESSO] Base de dados totalmente carregada no Grafo!\n");
}

void Buscador_salvarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links) {
    FILE *f_sites = fopen(nome_arq_sites, "w");
    FILE *f_links = fopen(nome_arq_links, "w");

    if (!f_sites || !f_links) {
        printf("[ERRO] Nao foi possivel abrir os arquivos para salvar.\n");
        if(f_sites) fclose(f_sites);
        if(f_links) fclose(f_links);
        return;
    }

    Vertex *v = g->first;
    while (v) {
        Site *s = (Site *)v->value;
        // Salva: ID URL Nome
        fprintf(f_sites, "%d %s %s", v->label, s->url, s->nome);
        
        // Salva as palavras na mesma linha
        for (int i = 0; i < s->qtd_palavras; i++) {
            fprintf(f_sites, " %s", s->palavras[i]);
        }
        fprintf(f_sites, "\n");

        // Salva as arestas (links) deste vértice
        Edge *e = v->first;
        while (e) {
            if (e->head) {
                fprintf(f_links, "%d %d\n", v->label, e->head->label);
            }
            e = e->next;
        }
        v = v->next;
    }

    fclose(f_sites);
    fclose(f_links);
}

void Buscador_cadastrarSite(Graph *g, int id, const char *url, const char *nome) {
    Site *novo_site = (Site *)malloc(sizeof(Site));
    if (!novo_site) {
        printf("[ERRO] Falta de memoria ao cadastrar site.\n");
        return;
    }

    strcpy(novo_site->url, url);
    strcpy(novo_site->nome, nome);
    novo_site->importancia = 1.0; 
    novo_site->palavras = NULL;  
    novo_site->qtd_palavras = 0; 

    Graph_insertVertex(g, id, novo_site);
}

void Buscador_cadastrarPalavra(Graph *g, IndiceInvertido *ind, int id_site, const char *palavra) {
    Vertex *v_site = Graph_findVertexByLabel(g, id_site);
    if (!v_site) {
        printf("[ERRO] Site ID %d nao encontrado.\n", id_site);
        return;
    }

    Site *site = (Site *)v_site->value;
    
    // Aumenta o array de palavras
    site->palavras = realloc(site->palavras, (site->qtd_palavras + 1) * sizeof(char *));
    site->palavras[site->qtd_palavras] = malloc(strlen(palavra) + 1);
    strcpy(site->palavras[site->qtd_palavras], palavra);
    
    site->qtd_palavras++; 

    // Insere no índice invertido para a busca funcionar
    if (ind) {
        Indice_inserirPalavra(ind, (char *)palavra, v_site);
    }
}

void Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino) {
    Graph_insertEdge(g, id_origem, id_destino, NULL);
    Buscador_recalcularRanking(g); 
}

void Buscador_removerLink(Graph *g, int id_origem, int id_destino) {
    Vertex *v_origem = Graph_findVertexByLabel(g, id_origem);
    if (v_origem) {
        Edge *aresta_removida = Graph_removeEdge(v_origem, id_destino);
        if (aresta_removida) {
            free(aresta_removida);
            Buscador_recalcularRanking(g); 
        }
    }
}

void Buscador_removerSite(Graph *g, IndiceInvertido *ind, int id_site) {
    Vertex *v_removido = Graph_removeVertex(g, id_site);
    
    if (v_removido) {
        // Tira do índice antes de apagar o vértice
        if (ind) {
            Indice_removerReferenciasVertice(ind, v_removido);
        }

        Site *site = (Site *)v_removido->value;
        if (site) {
            // Limpa o array dinâmico de strings
            for (int i = 0; i < site->qtd_palavras; i++) {
                free(site->palavras[i]); 
            }
            free(site->palavras); 
            free(site); 
        }
        free(v_removido); 
        
        Buscador_recalcularRanking(g); 
    }
}