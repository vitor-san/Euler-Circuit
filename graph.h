/*
  A simple Graph abstract data type (ADT)
  Author: Vitor Santana Cordeiro
  ICMC, USP
  São Carlos, Brazil
*/

#ifndef GRAPH_H
#define GRAPH_H

typedef double elemE;
typedef char elemV;
typedef struct graph GRAPH;
typedef GRAPH* Graph;

/*
  Instantializes a new Graph.
  @Parameters:
      int n -> the number of vertices the graph should be inicialized with.
      int d -> tells the function if you are creating a Digraph (in which case you must pass 1 as it's value), or not (in which case you pass 0). If d is a number diferent from 0 or 1, the function assumes that you're trying to create a Digraph.
  @Return:
      Graph -> a pointer to the newly created Graph.
If any error ocurred, the function will return NULL.
*/
Graph newGraph(int n, int d);

/*
  Tests whether there is an edge from vertex x to vertex y.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
      int y -> vertex y.
  @Return:
      int -> a boolean-like return value: if they are adjacent, returns 1; if not, returns 0.
If any error ocurred, the function will return -1.
*/
int adjacent(Graph g, int x, int y);

/*
  Adds an edge from vertex x to vertex y.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
      int y -> vertex y.
  @Return:
      int -> 0 for failure, 1 for success.
*/
int addEdge(Graph g, int x, int y);

/*
  Removes the edge from vertex x to vertex y.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
      int y -> vertex y.
  @Return:
      int -> 0 for failure, 1 for success.
*/
int removeEdge(Graph g, int x, int y);

/*
  Lists all vertices y such that there is an edge from the vertex x to the vertex y.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
  @Return:
      (int *) -> an array of integers, containing all vertex that are connected to x.
If any error ocurred, the function will return NULL.

>> REMEMBER TO FREE THE ARRAY RETURNED BY THIS FUNCTION! Otherwise, you may experience memory leak. <<
*/
int *neighbors(Graph g, int x);

/*
  Adds the vertex x, if it is not there.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex to be added.
  @Return:
      int -> 0 for failure, 1 for success.
*/
int addVertex(Graph g, int x);

/*
  Removes the vertex x, if it is there.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex to be removed.
  @Return:
      int -> 0 for failure, 1 for success.
*/
int removeVertex(Graph g, int x);

/*
  Returns the number of vertices in the graph.
  @Parameters:
      Graph g -> graph to be considered.
  @Return:
      int -> number of vertices in graph g.
If any error occured, the function will return -1.
*/
int getNumVertices(Graph g);

/*
  Returns the number of edges in the graph.
  @Parameters:
      Graph g -> graph to be considered.
  @Return:
      int -> number of edges in graph g.
If any error occured, the function will return -1.
*/
int getNumEdges(Graph g);

/*
  Returns the value associated with the edge (x, y).
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
      int y -> vertex y.
  @Return:
      elemE -> weight/cost/lenght of the edge (x, y).
If any error occured, the function will return -1.
*/
elemE getEdgeValue(Graph g, int x, int y);

/*
  Sets the value associated with the edge (x, y) to val.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
      int y -> vertex y.
      elemE val -> new edge (x, y) weight/cost/lenght.
*/
void setEdgeValue(Graph g, int x, int y, elemE val);

/*
  Returns the value associated with the vertex x.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
  @Return:
      elemV -> data stored on the vertex x.
If any error occured, the function will return 0.
*/
elemV getVertexValue(Graph g, int x);

/*
  Returns the degree of the vertex x.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
  @Return:
      int -> number of vertices linked with x.
If any error occured, the function will return -1.
*/
int getVertexDegree(Graph g, int x);

/*
  Sets the value associated with the vertex x to val.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
      elemV val -> new vertex value.
*/
void setVertexValue(Graph g, int x, elemV val);

/*
  Prints the graph.
  @Parameters:
      Graph g -> graph to be printed.
      int verbose -> tells the function if you want a more verbose showing of the graph or not. Any value different from 0 will make the function print the graph with more details.
If any error occured, the function does not print anything.
*/
void printGraph(Graph g, int verbose);

/*
  Prints all vertices y such that there is an edge from the vertex x to the vertex y.
  @Parameters:
      Graph g -> graph to be considered.
      int x -> vertex x.
If any error occured, the function does not print anything.
*/
void printNeighbors(Graph g, int x);

/*
  Deletes graph from memory, avoiding memory leak.
  @Parameters:
      Graph g -> graph to be deleted.
*/
void deleteGraph(Graph g);

#endif
