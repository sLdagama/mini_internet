#include <stdbool.h>

typedef struct Edge Edge;
typedef struct Vertex Vertex;
typedef struct Graph Graph;

struct Edge {
    int label;
    void *value;
    Vertex *tail, *head;
    
    Edge *next;
};

struct Vertex {
    int label;
    void *value;
    bool visited;

    int n;
    Edge *first;
    Vertex *next;
};

struct Graph {
    int n;
    int index;
    Vertex *first;
};

Graph  *Graph_alloc();
void    Graph_free(Graph *g);

void    Graph_insertVertex(Graph *g, void *value);
Vertex *Graph_removeVertex(Graph *g, int label);

void    Graph_insertEdge(Graph *g, int lab0, int lab1, void *value);
Edge   *Graph_removeEdge(Vertex *v, int label);

Vertex *Graph_findVertexByLabel(Graph *g, int label);
Vertex *Graph_findVertexByValue(Graph *g, void *value, int (*cmp)(void*,void*));

Edge   *Graph_findEdgeByLabel(Graph *g, int label);
Edge   *Graph_findEdgeByValue(Graph *g, void *value, int (*cmp)(void*,void*));
void    Graph_print(Graph *g);

Vertex **Graph_findAllVertexByValue(Graph *g, void *value, int (*cmp)(void*,void*));

Vertex **Graph_dfs(Graph *g, void *startValue, int (*cmp)(void*,void*));
Vertex **Graph_bfs(Graph *g, void *startValue, int (*cmp)(void*,void*));