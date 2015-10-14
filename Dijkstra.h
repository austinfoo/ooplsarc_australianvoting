
#ifndef Dijkstra_h
#define Dijkstra_h

// --------
// includes
// --------

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <vector>

using namespace std;

class VertexLength;

typedef std::vector<VertexLength> VertexLengthVector;
typedef std::vector<VertexLengthVector> Graph;
typedef std::vector<int> VertexVector;
typedef std::list<int> VertexList;

// ------------
// dijkstra_eval
// ------------

VertexList dijkstra_eval (const Graph& graph, int start_vertex_num, int end_vertex_num);

// -------------
// dijkstra_print
// -------------

void dijkstra_print (ostream& w, const VertexList& answer);

// -------------
// dijkstra_solve
// -------------

void dijkstra_solve (istream& r, ostream& w);

#endif
