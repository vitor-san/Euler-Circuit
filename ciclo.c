#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

int *cicloEuleriano(Graph g);

int main (void) {
  srand(time(NULL));

  //aqui voce cria e adiciona o seu grafo:
  Graph g = newGraph(6, 0);
  addEdge(g, 1, 2);
  addEdge(g, 1, 3);
  addEdge(g, 1, 4);
  addEdge(g, 1, 5);
  addEdge(g, 2, 4);
  addEdge(g, 3, 4);
  addEdge(g, 3, 5);
  addEdge(g, 3, 6);
  addEdge(g, 4, 6);
  printGraph(g, 0);
  printf("\n\n");

  int *vet = cicloEuleriano(g);

  if (vet == NULL) printf("Este grafo não possui um ciclo Euleriano!\n");
  else {
    int i = 0;
    printf("Um dos ciclos Eulerianos possiveis deste grafo eh: ");
    while (vet[i] != 0) printf("%d ", vet[i++]);
    printf("\n\n");
  }

  return 0;
}

void dfs(Graph g, int v, int *visitado) {
  visitado[v-1] = 1;
  int *vizinhos = neighbors(g, v);
  int grau = getVertexDegree(g, v);

  for (int i = 0; i < grau; i++) {
    if (!visitado[vizinhos[i]-1]) dfs(g, vizinhos[i], visitado);
  }
  free(vizinhos);
}

int isBridge(Graph g, int v) {
  int numV = getNumVertices(g);
  int *reachable = calloc(numV, sizeof(int));

  dfs(g, v, reachable);

  for (int i = 0; i < numV; i++) if (!reachable[i]) return 1;
  return 0;
}

int findCiclo(int *caminho, Graph g, int verAtual, int posNoCam) {
  int v2 = 0;   // inicializado com nada
  int grau = getVertexDegree(g, verAtual);
  int *conectados = neighbors(g, verAtual);

  // aqui so vou escolher o v2
  if (grau == 1) {
    v2 = conectados[0];
  }
  else {
    int i;
    for (i = 0; i < grau; i++) {
      if (!isBridge(g, conectados[i])) {
        v2 = conectados[i];
        break;
      }
    }
    //nao achou nenhum caminho que nao quebrasse o grafo
    if (i == grau) {
      free(conectados);
      return 0;
    }
  }
  free(conectados);

  removeEdge(g, verAtual, v2);
  caminho[posNoCam] = verAtual;
  caminho[posNoCam+1] = v2;

  int retorno;
  if (getNumEdges(g) != 0) retorno = findCiclo(caminho, g, v2, posNoCam+1);
  addEdge(g, verAtual, v2);
  return retorno;
}

int *cicloEuleriano(Graph g) {
  int v = getNumVertices(g);
  int a = getNumEdges(g);

  for (int i = 1; i <= v; i++) {
    if (getVertexDegree(g, i)%2) return NULL;   //se algum dos vertices nao for par, nao existe ciclo euleriano
  }

  // a partir daqui, eh possivel que exista um ciclo euleriano

  int v1 = (rand()%v)+1;
  int *caminho = malloc((a+2)*sizeof(int));   // alocando espaço para as "a" arestas + o terminador "0"

  while (!findCiclo(caminho, g, v1, 0)) {
    v1 = (rand()%v)+1;
    findCiclo(caminho, g, v1, 0);
  }
  return caminho;
}
