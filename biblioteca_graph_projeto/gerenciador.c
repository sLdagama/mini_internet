#include "gerenciador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Buscador_carregarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links) {
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

        // Se a linha vier mal formatada, ignora e segue a vida
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

        // Insere o vértice. A biblioteca Graph.c já cria o ID sequencial certinho.
        Graph_insertVertex(g, novo_site);

        // Pulando ID, URL e Nome pra chegar nas palavras-chave da linha
        char *token = strtok(linha, " \n"); 
        token = strtok(NULL, " \n");        
        token = strtok(NULL, " \n");        

        token = strtok(NULL, " \n");
        while (token) {
            PalavraNode *nova_palavra = (PalavraNode *)malloc(sizeof(PalavraNode));
            if (nova_palavra) {
                strcpy(nova_palavra->palavra, token);
                // Insere no início da lista (O(1))
                nova_palavra->prox = novo_site->lista_palavras;
                novo_site->lista_palavras = nova_palavra;
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
        // Usa a função exata da biblioteca do projeto
        Graph_insertEdge(g, orig, dest, NULL);
    }
    fclose(arq_links);
    
    printf("[SUCESSO] Base de dados totalmente carregada no Grafo!\n");
}

void Buscador_salvarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links) {
    if (!g) return;

    FILE *arq_sites = fopen(nome_arq_sites, "w");
    FILE *arq_links = fopen(nome_arq_links, "w");

    if (!arq_sites || !arq_links) {
        printf("[ERRO] Problema ao abrir arquivos pra escrita.\n");
        if(arq_sites) fclose(arq_sites);
        if(arq_links) fclose(arq_links);
        return;
    }

    // 1. Salvar os Sites e as Palavras varrendo a lista de vértices do Graph.h
    Vertex *v = g->first;
    while (v) {
        Site *site = (Site *)v->value;
        if (site) {
            fprintf(arq_sites, "%d %s %s", v->label, site->url, site->nome);
            
            // Grava as palavras na mesma linha
            PalavraNode *atual = site->lista_palavras;
            while (atual) {
                fprintf(arq_sites, " %s", atual->palavra);
                atual = atual->prox;
            }
            fprintf(arq_sites, "\n");
        }
        v = v->next; // Pula pro próximo vértice
    }

    // 2. Salvar os Links varrendo as arestas do Graph.h
    v = g->first;
    while (v) {
        Edge *e = v->first;
        while (e) {
            fprintf(arq_links, "%d %d\n", v->label, e->head->label);
            e = e->next; // Pula pra próxima aresta
        }
        v = v->next;
    }

    fclose(arq_sites);
    fclose(arq_links);
    printf("[SUCESSO] Persistencia concluida! Arquivos atualizados.\n");
}

void Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino) {
    Graph_insertEdge(g, id_origem, id_destino, NULL);
}

void Buscador_removerLink(Graph *g, int id_origem, int id_destino) {
    // Na biblioteca do Graph.h, remover aresta pede o ponteiro do Vértice de origem
    Vertex *v_origem = Graph_findVertexByLabel(g, id_origem);
    if (v_origem) {
        Edge *aresta_removida = Graph_removeEdge(v_origem, id_destino);
        if (aresta_removida) {
            free(aresta_removida); // Libera a memória da aresta removida
        }
    }
}

void Buscador_removerSite(Graph *g, int id_site) {
    // Graph_removeVertex desvincula o vértice, mas retorna ele pra gente limpar a sujeira
    Vertex *v_removido = Graph_removeVertex(g, id_site);
    
    if (v_removido) {
        Site *site = (Site *)v_removido->value;
        if (site) {
            // Limpa as palavras-chave primeiro pra evitar vazamento
            PalavraNode *atual = site->lista_palavras;
            while (atual) {
                PalavraNode *prox = atual->prox;
                free(atual); 
                atual = prox;
            }
            free(site); // Libera a struct do Site
        }
        free(v_removido); // Libera a struct do Vértice que a biblioteca desvinculou
    }
}