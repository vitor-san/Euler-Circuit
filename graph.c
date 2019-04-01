#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

typedef struct edgeNode {
  struct edgeNode *next;
  int vertex;
  elemE weight;
} EdgeNode;

typedef struct vertexNode {
  EdgeNode *beg, *end;
  elemV data;
  int numConnec;
} VertexNode;

struct graph {
  VertexNode *list;
  int numVt;
  int numEd;
  int isDigraph;
};

// n = number of vertices, d = is digraph
Graph newGraph(int n, int d) {
  Graph g = malloc(sizeof(GRAPH));  // creates the graph
  if (g == NULL) return NULL; // error

  if (n <= 0) return NULL;  // error: number of vertices should be a natural number
  g->list = malloc(n*sizeof(VertexNode)); // instantializes the list (no edges included)
  if (g->list == NULL) return NULL; // error

  for (int i = 0; i < n; i++) {   // initializes all the begs and ends with NULL
    g->list[i].beg = NULL;
    g->list[i].end = NULL;
    g->list[i].data = 0;
    g->list[i].numConnec = 0;
  }

  g->numVt = n;
  g->numEd = 0;
  if (d < 0 || d > 1) d = 1;
  g->isDigraph = d;

  return g;
}

int adjacent(Graph g, int x, int y) {
  if (g == NULL || g->list == NULL) return -1;  // error
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return -1; // error

  if (g->list[x-1].beg != NULL) {
    EdgeNode *aux = g->list[x-1].beg;
    while (aux != NULL) {
      if (aux->vertex == y) return 1;
      aux = aux->next;
    }
  }
  else return 0;
}

// creates x -> y
int addEdge(Graph g, int x, int y) {
  if (g == NULL || g->list == NULL) return 0;
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return 0;
  if (adjacent(g, x, y)) return 0;

  EdgeNode *n = malloc(sizeof(EdgeNode)); // new edge
  if (n == NULL) return 0;
  n->next = NULL;
  n->vertex = y;
  n->weight = 1;

  if (g->list[x-1].beg == NULL) {
    g->list[x-1].beg = n;
    g->list[x-1].end = n;
  } else {
    g->list[x-1].end->next = n;
    g->list[x-1].end = n;
  }

  int scndRtrn = 0;   // second return (in case it's a digraph)
  if (!g->isDigraph && !adjacent(g, y, x)) scndRtrn = addEdge(g, y, x);

  g->list[x-1].numConnec++;
  g->numEd++;

  if (!g->isDigraph) return scndRtrn;
  else return 1;
}

// removes x -> y
int removeEdge(Graph g, int x, int y) {
  if (g == NULL || g->list == NULL) return 0;
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return 0;

  if (g->list[x-1].beg != NULL) {
    // tries to find y node
    EdgeNode *cur = g->list[x-1].beg;
    EdgeNode *prev = NULL;

    while (cur != NULL && cur->vertex != y) {
      prev = cur;
      cur = cur->next;
    }

    if (cur == NULL) return 0;  // there's no edge from x to y
    else {  // the edge exists!
      if (prev == NULL) {   // cur points to the beginning of the list
        g->list[x-1].beg = cur->next;  // updates value of begin
        if (cur->next == NULL) g->list[x-1].end = NULL;  // updates value of end
        free(cur);  // deletes node
      }
      else {   // cur is not on the beginning of the list
        if (cur == g->list[x-1].end) {  // y is in the last node of the list
          prev->next = NULL;  // cuts reference to cur
          g->list[x-1].end = prev;  // updates value of end
          free(cur);  // deletes node
        }
        else {  // y is in the "middle" of the list
          prev->next = cur->next;   // cuts reference to cur
          free(cur);  // deletes node
        }
      }
    }
  }
  else return 0;  // there's no edge from x to y

  if (!g->isDigraph && adjacent(g, y, x)) removeEdge(g, y, x);

  g->list[x-1].numConnec--;
  g->numEd--;

  return 1;
}

/* REMEMBER TO FREE() THE ARRAY RETURNED BY THIS FUNCTION!!! */
int *neighbors(Graph g, int x) {
  if (g == NULL || g->list == NULL) return NULL;
  if (x < 1 || x > g->numVt) return NULL;

  int connections = g->list[x-1].numConnec;
  int *neigh = malloc((connections+1)*sizeof(int));   // allocates space for all the neighbors + a '0' that indicates the end of the list
  if (neigh == NULL) return NULL;

  EdgeNode *aux = g->list[x-1].beg;
  for (int i = 0; i < connections; i++) {
    neigh[i] = aux->vertex;
    aux = aux->next;
  }
  neigh[connections] = 0;   // terminator

  return neigh;
}

int addVertex(Graph g, int x) {     // adds the vertex x, if it is not there
  if (g == NULL || g->list == NULL) return 0;
  if (x < 1) return 0;  // number of vertex shall be at least one

  if (x > g->numVt) {
    g->list = realloc(g->list, 2*(g->numVt)*sizeof(VertexNode));
    if (g->list == NULL) return -1;   // super error: you lost the graph you had previously (probably will never happen)
    g->list[x-1].data = 0;
    g->list[x-1].beg = NULL;
    g->list[x-1].end = NULL;
    g->numVt++;
    return 1;   // success
  }
  else {
    if (g->list[x-1].beg != NULL || g->list[x-1].data != 0) return 0;
    else {
      g->numVt++;   // I don't know if this is needed, yet
      return 1;
    }
  }
}

int removeVertex(Graph g, int x) {     // removes the vertex x, if it is there
  if (g == NULL || g->list == NULL) return 0;
  if (x < 1 || x > g->numVt) return 0;  // you shall remove a vertex that already exists

  // first, I will remove all edges that ends on x
  for (int i = 0; i < g->numVt; i++) {
    if (adjacent(g, i+1, x)) removeEdge(g, i+1, x);
  }
  // now, I will remove all edges that starts on x (if g is a digraph; otherwise, they are already removed)
  if (g->isDigraph) {
    int *neigh = neighbors(g, x);
    int i = 0;
    while (neigh[i] != 0) removeEdge(g, x, neigh[i++]);
    free(neigh);
  }
  // erases the data in vertex x
  g->list[x-1].data = 0;

  return 1;   // success
}

int getNumVertices(Graph g) {
  if (g == NULL) return -1;  // error
  return g->numVt;
}

int getNumEdges(Graph g) {
  if (g == NULL) return -1;  // error
  return g->numEd;
}

elemE getEdgeValue(Graph g, int x, int y) {
  if (g == NULL || g->list == NULL) return -1;  // error
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return -1;  // error

  EdgeNode *aux = g->list[x-1].beg;
  while (aux != NULL && aux->vertex != y) aux = aux->next;
  if (aux == NULL) return -1;   // error: there is no edge from x to y
  else return aux->weight;
}

void setEdgeValue(Graph g, int x, int y, elemE val) {
  if (g == NULL || g->list == NULL) return;  // error
  if (x < 1 || x > g->numVt || y < 1 || y > g->numVt) return;  // error

  EdgeNode *aux = g->list[x-1].beg;
  while (aux != NULL && aux->vertex != y) aux = aux->next;
  if (aux == NULL) return;   // error: there is no edge from x to y
  else aux->weight = val;
}

elemV getVertexValue(Graph g, int x) {
  if (g == NULL || g->list == NULL) return 0;  // error
  if (x < 1 || x > g->numVt) return 0;  // error

  return g->list[x-1].data;
}

int getVertexDegree(Graph g, int x) {
  if (g == NULL || g->list == NULL) return -1;  // error
  if (x < 1 || x > g->numVt) return -1;  // error

  return g->list[x-1].numConnec;
}

void setVertexValue(Graph g, int x, elemV val) {
  if (g == NULL || g->list == NULL) return;  // error
  if (x < 1 || x > g->numVt) return;  // error

  g->list[x-1].data = val;
}

void printGraph(Graph g, int verbose) {
  if (g == NULL || g->list == NULL) return;

  EdgeNode *aux;
  for (int i = 0; i < g->numVt; i++) {
    if (g->list[i].beg != NULL) {
      aux = g->list[i].beg;
      if (verbose) printf("Vertex %d (%c) -> ", i+1, g->list[i].data);
      else printf("%d -> ", i+1);
      while (aux->next != NULL) {
        if (verbose) printf("(%d, %.2lf), ", aux->vertex, aux->weight);
        else printf("%d, ", aux->vertex);
        aux = aux->next;
      }
      if (verbose) printf("(%d, %.2lf)\n", aux->vertex, aux->weight);
      else printf("%d\n", aux->vertex);
    }
    else {
      if (verbose) printf("Vertex %d (%c) -> NONE\n", i+1, g->list[i].data);
      else printf("%d -> NONE\n", i+1);
    }
  }
}

void printNeighbors(Graph g, int x) {
  int *vet = neighbors(g, x), i = 0;

  printf("Neighbors of %d: ", x);
  while (vet[i] != 0) printf("%d ", vet[i++]);
  printf("\n");

  free(vet);
}

void deleteGraph(Graph g) {
  if (g == NULL) return;

  // free list:
  EdgeNode *aux;
  if (g->list != NULL) {
    for (int i = 0; i < g->numVt; i++) {
      if (g->list[i].beg != NULL) {
        aux = g->list[i].beg;
        while (g->list[i].beg->next != NULL) {
          g->list[i].beg = g->list[i].beg->next;
          free(aux);
          aux = g->list[i].beg;
        }
        free(aux);
      }
    }
    free(g->list);
  }

  // free graph:
  free(g);
}
