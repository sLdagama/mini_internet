#include "Graph.h"
#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char url[150];
    char nome[100];
    double importancia; // Peso/Ranking calculado pelas arestas que chegam
} Site;

// O valor das arestas pode ser apenas um peso ou NULL se não carregar dados extras

typedef struct {
    char palavra[50];
    Vertex **sites; // Vetor de ponteiros para os Vértices do Grafo que contêm essa palavra
    int qtd_sites;
} EntradaIndice;
