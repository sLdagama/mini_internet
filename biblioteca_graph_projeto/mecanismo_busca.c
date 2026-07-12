#include <stdio.h>
#include <stdlib.h>
#include "mecanismo_busca.h"


void Buscador_recalcularRanking(Graph *g) {
    if (!g) return;

    Vertex *v = g->first;
    
    // Passo 1: Zerar a importância de todos os sites do grafo
    while (v) {
        Site *site = (Site *)v->value;
        if (site) {
            site->importancia = 0.0;
        }
        v = v->next;
    }

    // Passo 2: Percorrer cada vértice 'w' para analisar as arestas que saem dele
    Vertex *w = g->first;
    while (w) {
        Edge *e = w->first; // Pega a primeira aresta que sai de 'w'
        
        // Passo 3: Percorrer todas as arestas que saem de 'w'
        while (e) {
            Vertex *destino = e->head; // É o vértice onde a aresta chega
            
            if (destino) {
                Site *site_destino = (Site *)destino->value;
                if (site_destino) {
                    // Passo 4: Soma o valor da aresta na importância do destino.
                    // Se o projeto de vocês usar arestas ponderadas (com peso), mude para algo como:
                    // site_destino->importancia += *(double *)(e->value);
                    // Se não tiver peso nas arestas (cada link vale 1), usamos:
                    site_destino->importancia += 1.0; 
                }
            }
            e = e->next; // Vai para a próxima aresta que sai de 'w'
        }
        w = w->next; // Vai para o próximo vértice do grafo
    }
}