#ifndef MECANISMO_BUSCA_H
#define MECANISMO_BUSCA_H

#include "Graph.h"


typedef struct {
    char url[150];
    char nome[100];
    double importancia;
    
    char **palavras;     
    int qtd_palavras;    
} Site;


//Nó de uma lista de sites que possuem uma certa palavra
typedef struct OcorrenciaNode {
    Vertex *vertex;                 
    struct OcorrenciaNode *next;
} OcorrenciaNode;

//nó da lista de palavras
typedef struct WordNode {
    char palavra[50];
    OcorrenciaNode *sites_first;    //Início da lista de sites que possuem uma certa palavra
    int qtd_sites;                
    struct WordNode *next;     
} WordNode;

//Indice invertido para facilitar a busca de sites
typedef struct {
    WordNode *first;    //Início da lista de palavras
    int qtd_palavras;
} IndiceInvertido;




IndiceInvertido *Indice_alloc();

//Associa uma palavra a um determinado site do grafo
void Indice_inserirPalavra(IndiceInvertido *ind, char *palavra, Vertex *v);

//Remove as referências de um determinado site no índice (útil quando Samuel for remover um site)
void Indice_removerReferenciasVertice(IndiceInvertido *ind, Vertex *v);

void Indice_free(IndiceInvertido *ind);

//Percorre o grafo todo e soma o número de de arestas que chegam em vertice para determinar a importância do site
void Buscador_recalcularRanking(Graph *g);





//Retorna um vetor de Vertex* contendo os sites com a palavra informada
Vertex **Buscador_buscaSimples(IndiceInvertido *ind, char *palavra, int *qtd_resultados);

Vertex **Buscador_operacaoAND(Vertex **resA, int qtdA, Vertex **resB, int qtdB, int *qtdFinal);

Vertex **Buscador_operacaoOR(Vertex **resA, int qtdA, Vertex **resB, int qtdB, int *qtdFinal);

//Função de comparação para ordenar os sites por importância e ordem alfabética
int Buscador_compararSites(const void *a, const void *b);

//Processa uma string vê se tem AND ou OR para chamar as funções acima e retorna um vetor de vertex* ordernado
Vertex **Buscador_realizarConsultaCompleta(Graph *g, IndiceInvertido *ind, char *expressao, int *qtd_final);

#endif 