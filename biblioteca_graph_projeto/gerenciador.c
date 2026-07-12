#include "gerenciador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar necessária para o Samuel achar o Vertex no Grafo usando a URL
int cmp_url(void *a, void *b) {
    Site *s = (Site *)a;
    char *url = (char *)b;
    return strcmp(s->url, url);
}

// CORREÇÃO: Adicionado 'IndiceInvertido *ind' nos parâmetros
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
        novo_site->lista_palavras = NULL; 

        Graph_insertVertex(g, novo_site);

        // CORREÇÃO: Buscamos o ponteiro do Vertex criado para poder passar para o seu índice
        Vertex *v_site = Graph_findVertexByValue(g, url, cmp_url);

        char *token = strtok(linha, " \n"); 
        token = strtok(NULL, " \n");        
        token = strtok(NULL, " \n");        

        token = strtok(NULL, " \n");
        while (token) {
            PalavraNode *nova_palavra = (PalavraNode *)malloc(sizeof(PalavraNode));
            if (nova_palavra) {
                strcpy(nova_palavra->palavra, token);
                nova_palavra->prox = novo_site->lista_palavras;
                novo_site->lista_palavras = nova_palavra;

                // CORREÇÃO: Alimenta o seu índice dinamicamente
                if (ind && v_site) {
                    Indice_inserirPalavra(ind, token, v_site);
                }
            }
            token = strtok(NULL, " \n"); 
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
    
    // CORREÇÃO: Roda o seu ranking inicial logo após ler toda a estrutura
    Buscador_recalcularRanking(g);

    printf("[SUCESSO] Base de dados totalmente carregada no Grafo!\n");
}

void Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino) {
    Graph_insertEdge(g, id_origem, id_destino, NULL);
    // CORREÇÃO: Recalcula o ranking após inserção
    Buscador_recalcularRanking(g); 
}

void Buscador_removerLink(Graph *g, int id_origem, int id_destino) {
    Vertex *v_origem = Graph_findVertexByLabel(g, id_origem);
    if (v_origem) {
        Edge *aresta_removida = Graph_removeEdge(v_origem, id_destino);
        if (aresta_removida) {
            free(aresta_removida);
            // CORREÇÃO: Recalcula o ranking após remoção de link
            Buscador_recalcularRanking(g); 
        }
    }
}

// CORREÇÃO: Adicionado 'IndiceInvertido *ind' para limpar as referências com segurança
void Buscador_removerSite(Graph *g, IndiceInvertido *ind, int id_site) {
    Vertex *v_removido = Graph_removeVertex(g, id_site);
    
    if (v_removido) {
        // CORREÇÃO: Limpa o site do seu índice ANTES de dar free na memória
        if (ind) {
            Indice_removerReferenciasVertice(ind, v_removido);
        }

        Site *site = (Site *)v_removido->value;
        if (site) {
            PalavraNode *atual = site->lista_palavras;
            while (atual) {
                PalavraNode *prox = atual->prox;
                free(atual); 
                atual = prox;
            }
            free(site); 
        }
        free(v_removido); 
        
        // CORREÇÃO: Recalcula o ranking já que links que iam para este site sumiram
        Buscador_recalcularRanking(g); 
    }
}