#ifndef MECANISMO_BUSCA_H
#define MECANISMO_BUSCA_H

#include "Graph.h"


typedef struct {
    char url[150];
    char nome[100];
    double importancia; // Peso calculado pela soma das arestas que chegam
} Site;

// 2. Estruturas para o Índice Invertido
// Nó de uma lista encadeada que guarda os vértices onde a palavra aparece
typedef struct OcorrenciaNode {
    Vertex *vertex;                 // Ponteiro para o vértice (site) real dentro do Grafo
    struct OcorrenciaNode *next;
} OcorrenciaNode;

// Estrutura principal de cada palavra no Índice
typedef struct WordNode {
    char palavra[50];
    OcorrenciaNode *sites_first;    // Lista encadeada de sites que contêm esta palavra
    int qtd_sites;                  // Quantidade de sites que possuem essa palavra
    struct WordNode *next;          // Próxima palavra do índice (Lista Encadeada de Palavras)
} WordNode;

// Tipo opaco ou facilitador para o Índice Invertido
typedef struct {
    WordNode *first;                // Início da lista de palavras cadastradas
    int qtd_palavras;
} IndiceInvertido;



/* --- GERENCIAMENTO DO ÍNDICE --- */

// Aloca e inicializa a estrutura do índice invertido
IndiceInvertido *Indice_alloc();

// Associa uma palavra a um determinado vértice (site) do grafo
void Indice_inserirPalavra(IndiceInvertido *ind, char *palavra, Vertex *v);

// Remove todas as referências de um determinado vértice no índice (útil quando Samuel remover um site)
void Indice_removerReferenciasVertice(IndiceInvertido *ind, Vertex *v);

// Libera toda a memória alocada para o índice invertido
void Indice_free(IndiceInvertido *ind);


/* --- RANKING E IMPORTÂNCIA --- */

// Percorre o Grafo todo, zera a importância dos sites e recalcula
// somando os valores/pesos de todas as arestas que chegam a cada vértice
void Buscador_recalcularRanking(Graph *g);


/* --- MECANISMO DE CONSULTA E OPERAÇÕES LÓGICAS --- */

// Retorna um vetor dinâmico (alocado com calloc) de Vertex* contendo os sites com a palavra informada
Vertex **Buscador_buscaSimples(IndiceInvertido *ind, char *palavra, int *qtd_resultados);

// Faz a interseção matemática (AND) de dois vetores de resultados
Vertex **Buscador_operacaoAND(Vertex **resA, int qtdA, Vertex **resB, int qtdB, int *qtdFinal);

// Faz a união matemática (OR) de dois vetores de resultados sem duplicatas
Vertex **Buscador_operacaoOR(Vertex **resA, int qtdA, Vertex **resB, int qtdB, int *qtdFinal);

// Função de comparação (estilo callback do qsort) para ordenar os sites por importância e depois alfabética
int Buscador_compararSites(const void *a, const void *b);

// Processa uma string complexa (ex: "computador AND ceara"), realiza os filtros e devolve o vetor ordenado
Vertex **Buscador_realizarConsultaCompleta(Graph *g, IndiceInvertido *ind, char *expressao, int *qtd_final);


#endif // MECANISMO_BUSCA_H