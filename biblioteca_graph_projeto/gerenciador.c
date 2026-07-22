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
    
    while (fgets(linha, sizeof(linha), arq_sites)) {
        int id;
        char url[150], nome[100];
        double peso = 1.0;

        // Agora exige 4 campos: id url nome peso
        if (sscanf(linha, "%d %s %s %lf", &id, url, nome, &peso) < 4) continue;

        Site *novo_site = (Site *)malloc(sizeof(Site));
        if (!novo_site) {
            printf("[ERRO] Faltou RAM pro site %s.\n", nome);
            fclose(arq_sites);
            return;
        }

        strcpy(novo_site->url, url);
        strcpy(novo_site->nome, nome);
        novo_site->importancia = 0.0;   // vai ser recalculada de qualquer forma
        novo_site->peso = peso;         // <- agora inicializado corretamente!
        
        novo_site->palavras = NULL;  
        novo_site->qtd_palavras = 0; 

        Graph_insertVertex(g, id, novo_site);

        Vertex *v_site = Graph_findVertexByValue(g, url, cmp_url);

        if (ind && v_site) {
            char termo_nome[100], termo_url[150];
            strcpy(termo_nome, nome);
            strcpy(termo_url, url);

            string_para_minusculo(termo_nome);
            string_para_minusculo(termo_url);

            Indice_inserirPalavra(ind, termo_nome, v_site);
            Indice_inserirPalavra(ind, termo_url, v_site);
        }

        // Pula id, url, nome E peso antes de começar a ler as palavras
        char *token = strtok(linha, " \r\n"); // id
        token = strtok(NULL, " \r\n");        // url
        token = strtok(NULL, " \r\n");        // nome
        token = strtok(NULL, " \r\n");        // peso  <-- pulo novo

        token = strtok(NULL, " \r\n");        // primeira palavra-chave
        while (token) {
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
    while (fscanf(arq_links, "%d %d", &orig, &dest) == 2) {
        Graph_insertEdge(g, orig, dest, NULL);
    }
    fclose(arq_links);
    
    Buscador_recalcularRanking(g);
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
        // Salva: ID URL Nome Peso
        fprintf(f_sites, "%d %s %s %g", v->label, s->url, s->nome, s->peso);
        
        for (int i = 0; i < s->qtd_palavras; i++) {
            fprintf(f_sites, " %s", s->palavras[i]);
        }
        fprintf(f_sites, "\n");

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

int Buscador_cadastrarSite(Graph *g, IndiceInvertido *ind, int id, const char *url, const char *nome, double peso) {
    if (!g || !url || strlen(url) == 0 || peso < 1.0) {
        return 0;
    }

    // Verifica se já existe um site com essa mesma URL
    if (Graph_findVertexByValue(g, (void *)url, cmp_url) != NULL) {
        return 0; 
    }
    
    Site *novo_site = (Site *)malloc(sizeof(Site));
    if (!novo_site) {
        printf("[ERRO] Falta de memoria ao cadastrar site.\n");
        return 0;
    }

    strcpy(novo_site->url, url);
    strcpy(novo_site->nome, nome);
    novo_site->importancia = 0.0; 
    novo_site->palavras = NULL;  
    novo_site->qtd_palavras = 0; 
    novo_site->peso = peso;
    Graph_insertVertex(g, id, novo_site);

    Vertex *v_site = Graph_findVertexByLabel(g, id);

    // Indexa o nome e a URL no cadastro dinâmico
    if (ind && v_site) {
        char termo_nome[100], termo_url[150];
        strcpy(termo_nome, nome);
        strcpy(termo_url, url);

        string_para_minusculo(termo_nome);
        string_para_minusculo(termo_url);

        Indice_inserirPalavra(ind, termo_nome, v_site);
        Indice_inserirPalavra(ind, termo_url, v_site);
    }

    Buscador_recalcularRanking(g);
    return 1;
}

void Buscador_cadastrarPalavra(Graph *g, IndiceInvertido *ind, int id_site, const char *palavra) {
    if (!palavra) return;

    Vertex *v_site = Graph_findVertexByLabel(g, id_site);
    if (!v_site) {
        printf("[ERRO] Site ID %d nao encontrado.\n", id_site);
        return;
    }

    //Cria uma cópia local tratada em minúsculo
    char palavra_min[150];
    strncpy(palavra_min, palavra, sizeof(palavra_min) - 1);
    palavra_min[sizeof(palavra_min) - 1] = '\0';
    string_para_minusculo(palavra_min);

    Site *site = (Site *)v_site->value;

    //Aumenta o array e salva a palavra no site
    char **temp = realloc(site->palavras, (site->qtd_palavras + 1) * sizeof(char *));
    if (!temp) {
        printf("[ERRO] Falha de memoria ao cadastrar palavra.\n");
        return;
    }
    site->palavras = temp;

    site->palavras[site->qtd_palavras] = malloc(strlen(palavra_min) + 1);
    if (site->palavras[site->qtd_palavras]) {
        strcpy(site->palavras[site->qtd_palavras], palavra_min);
        site->qtd_palavras++;
    }

    if (ind) {
        Indice_inserirPalavra(ind, palavra_min, v_site);
    }
}

int Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino) {
    if (!g) return 0;

    Vertex *v_origem = Graph_findVertexByLabel(g, id_origem);
    Vertex *v_destino = Graph_findVertexByLabel(g, id_destino);

    if (!v_origem || !v_destino) {
        return 0; //ids inválidos
    }

    // Verifica se o link já existe no Grafo
    Edge *e = v_origem->first;
    while (e) {
        if (e->head == v_destino) {
            return 0;
        }
        e = e->next;
    }

    //Se não existir, insere e recalcula
    Graph_insertEdge(g, id_origem, id_destino, NULL);
    Buscador_recalcularRanking(g);
    
    return 1; // 
}

int Buscador_removerLink(Graph *g, int id_origem, int id_destino) {
    if (!g) return 0;

    Vertex *v_origem = Graph_findVertexByLabel(g, id_origem);
    
    if (v_origem) {
        Edge *aresta_removida = Graph_removeEdge(v_origem, id_destino);
        
        //a aresta existia e foi removida:
        if (aresta_removida) {
            free(aresta_removida);
            Buscador_recalcularRanking(g); 
            return 1; 
        }
    }

    return 0; 
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