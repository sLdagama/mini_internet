#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include "Graph.h"
typedef struct PalavraNode {
    char palavra[50];
    struct PalavraNode *prox;
} PalavraNode;

typedef struct {
    char url[150];
    char nome[100];
    double importancia; // Peso/Ranking calculado pelas arestas que chegam
    PalavraNode *lista_palavras; // Ponteiro para o início da lista de palavras
} Site;

void Buscador_carregarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links);
void Buscador_salvarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links);

void Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino);
void Buscador_removerLink(Graph *g, int id_origem, int id_destino);
void Buscador_removerSite(Graph *g, int id_site);

#endif