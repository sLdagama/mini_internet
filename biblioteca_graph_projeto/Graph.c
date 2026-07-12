#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "Queue.h"
Graph *Graph_alloc() {
    Graph *g = malloc(sizeof(Graph));

    if (g) {
        g->n = 0;
        g->index = 0;
        g->first = NULL;
        g->last = NULL;
    }

    return g;
}

void Graph_insertVertex(Graph *g, void *value) {
    Vertex *new = NULL;

      if (g) {
        new = malloc(sizeof(Vertex));
        new->label = g->index;
        new->value = value;
        new->n = 0;
        new->first = NULL;
        new->next = NULL;

        if (g->first == NULL) {
            g->first = new;
        } else {
            g->last->next = new;
        }
        
        g->last = new;

        g->n++;
        g->index++;
    }

}

void Graph_print(Graph *g) {
    Vertex *v;
    Edge *e;

    if (g) {
        printf("Graph(%d)\n", g->n);
        
        v = g->first;
        while (v) {
            printf(" %d/ ", v->label);

            e = v->first;
            while(e) {
                printf("%d ", e->head->label);
                e = e->next;
            }
            printf("\n");

            v = v->next;
        }
    }
}

Vertex *Graph_findVertexByLabel(Graph *g, int label) {
    Vertex *v = NULL;

    if (g) {
        v = g->first;
        while (v) {
            if (v->label == label)
                return v;
            v = v->next;
        }
    }

    return NULL;
}

Vertex *Graph_findVertexByValue(Graph *g, 
                                    void *value, 
                                    int (*cmp)(void*,void*)) {

    Vertex *v = NULL;

    if (g) {
        v = g->first;
        while (v) {
            if (cmp(v->value, value) == 0)
                return v;
            v = v->next;
        }
    }

    return NULL;
}

void Graph_insertEdge(Graph *g, int lab0, int lab1, void *value) {
    Vertex *v0 = NULL, *v1 = NULL;
    Edge *new = NULL;

    if (g) {
        v0 = Graph_findVertexByLabel(g, lab0);
        v1 = Graph_findVertexByLabel(g, lab1);

        if (v0 && v1) {

            new = malloc(sizeof(Edge));
            new->label = v0->n;
            new->value = value;
            new->tail = v0;
            new->head = v1;
            new->next = NULL;

            new->next = v0->first;
            v0->first = new;
            v0->n++;
        }
    }
}

void Graph_free(Graph *g) {
    Vertex *v, *vnext;
    Edge *e, *enext;

    if (g) {
        v = g->first;
        while (v) {
            
            e = v->first;
            while (e) {
                enext = e->next;
                free(e);
                e = enext;
            }

            vnext = v->next;
            free(v);
            v = vnext;
        }

        free(g);
    }
}

Edge *Graph_removeEdge(Vertex *v, int label) {
    Edge *e = NULL, *ant = NULL;

    if (v && label >= 0) {

        e = v->first;

        while (e) {
            if (e->head->label == label) {
                if (ant == NULL)
                    v->first = e->next;
                else
                    ant->next = e->next;
                v->n--;
                return e;
            }
            ant = e;
            e = e->next;
        }
    }

    return NULL;
}

Vertex *Graph_removeVertex(Graph *g, int label) {
    Vertex *v = NULL, *ant = NULL, *vremoved = NULL;
    Edge *e = NULL, *enext;

    if (g && label >= 0) {

        v = g->first;

        while (v) {
            Vertex *vnext = v->next; // Guarda o próximo antes de mexer na estrutura

            if (v->label == label) {
                e = v->first;

                while (e) {
                    enext = e->next;
                    free(e);
                    e = enext;
                }

                if (ant == NULL) {
                    g->first = vnext;
                }
                else {
                    ant->next = vnext;
                }

                if (g->last == v) {
                    g->last = ant;
                }

                g->n--;
                vremoved = v;
                // Não avança o 'ant' aqui porque o 'v' atual foi deletado
            }
            else {
                // CORREÇÃO: Remove em loop até não restar NENHUM link duplicado
                // e libera a memória de cada aresta removida com free()
                Edge *removida;
                while ((removida = Graph_removeEdge(v, label)) != NULL) {
                    free(removida); 
                }
                ant = v; // Só avança o 'ant' se o vértice atual não foi removido
            }
            v = vnext;
        }
    }

    return vremoved;
}

Vertex *notVisited(Edge *e) {
    if (e) {
        while (e) {
            if (e->head->visited == false)
                return e->head;
            e = e->next;
        }
    }

    return NULL;
}

void dfs(Vertex *v, Vertex **path, int *count) {
    Vertex *k = NULL;

    if (v && path && count) {
        v->visited = true;
        path[*count] = v;

        while((k = notVisited(v->first))) {
            *count += 1;
            dfs(k, path, count);
        }
    }
}

Vertex **Graph_dfs(Graph *g, void *startValue, int (*cmp)(void*,void*)) {
    Vertex **path = NULL, *v = NULL;
    int i, count = 0;

    if (g && startValue && cmp) {
        v = Graph_findVertexByValue(g, startValue, cmp);

        if (v) {
            path = calloc(g->n, sizeof(Vertex*));
            dfs(v, path, &count);
        }
    }

    return path;
}


Vertex ** Graph_bfs ( Graph *g, void * startValue , int (* cmp )( void *, void *) ) {
    Vertex ** path = NULL;
    Vertex *v = NULL , *k = NULL;
    Queue *q = NULL;
    int count = 0;

    if (g && startValue && cmp) {
        Vertex *temp = g->first;
        while(temp) {
            temp->visited = false;
            temp = temp->next;
        }

        if ((v = Graph_findVertexByValue (g, startValue , cmp ))) {

            path = calloc(g->n, sizeof(Vertex*)) ;
            q = Queue_alloc(g->n) ;

            v->visited = true;
            path[count++] = v;
            Queue_push(q, v) ;

            while (!Queue_isEmpty(q)) {
                v = Queue_begin(q);


                Edge *e = v->first;
                while (e) {
                    k = e->head; 
                    
                    if (k->visited == false) {
                        k->visited = true;
                        path[count++] = k;
                        Queue_push(q, k);
                    }
                    e = e->next; 
                }

                Queue_pop(q);
            }
            Queue_free(q); 
        }
    }
    return path;
}

