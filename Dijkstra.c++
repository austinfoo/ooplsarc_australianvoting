// ----------------------------
// projects/twobuttons/Twobuttons.c++
// Copyright (C) 2015
// Glenn P. Downing
// ----------------------------

// --------
// includes
// --------

#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <queue>
#include <utility>
#include <cstdint>

#include "Dijkstra.h"

using namespace std;

// ------------
// dijkstra_eval
// ------------

class VertexLength
{
public:
  VertexLength (int vertex_num_, int length_) :
    vertex_num (vertex_num_),
    length (length_)
  {}

  int vertex_num = 0;
  int length = 0;
};

class QueueEntry
{
public:
  QueueEntry(int vertex_num_, int64_t cumm_length_, int64_t path_history_idx_) :
    vertex_num (vertex_num_),
    cumm_length (cumm_length_),
    path_history_idx (path_history_idx_)
  {}

  int vertex_num = 0;
  int64_t cumm_length = 0;
  int64_t path_history_idx = 0;
};

typedef std::queue<QueueEntry> Queue;

class VisitEntry
{
public:
  bool visited = false;
  int64_t cumm_length = 0;
};

class Solution {
public:
  int64_t cumm_length = INT64_MAX;
  int64_t path_history_idx = -1;
};


class BreadCrumb {
public:
  BreadCrumb (int64_t parent_path_history_idx_, int vertex_num_) :
    parent_path_history_idx (parent_path_history_idx_),
    vertex_num (vertex_num_)
  {}

  int64_t parent_path_history_idx = 0;
  int vertex_num = 0;
};

typedef std::vector<BreadCrumb> PathHistory;

// More optimization ideas:
// - We can remove the visited bool.  We really don't need it.
// - We copy the path a lot.  Maybe we can share it somehow.
// - Instead of a simple queue to manage what path we work on next,
//   maybe we should come up with some data structure to work on
//   the current shortest path next.

VertexList dijkstra_eval (const Graph& graph, int start_vertex_num, int end_vertex_num)
{
  // Create path history structure
  PathHistory path_history { {-1, start_vertex_num} };

  // Create the queue and seed it with the initial value.  Note that the stl queue 
  // uses funny list initialization with enclosing () because it is not a first class
  // container.  It is a container adaptor using a deque underneath.
  // The second 0 is the index into the path history structure for the seed entry.
  Queue q ({ {start_vertex_num, 0, 0} });

  // Create visit vector
  std::vector<VisitEntry> v (graph.size());

  // Create something to hold the solution
  Solution solution;

  // Do the search
  while (!q.empty()) {
    
    // Get the entry from the head of the queue
    const QueueEntry qe = q.front();
    q.pop();
    
    // If we found a solution keep it if it is shorter than any previous solution
    if (qe.vertex_num == end_vertex_num) 
    {
      if (qe.cumm_length < solution.cumm_length) {
	solution.cumm_length = qe.cumm_length;
	solution.path_history_idx = qe.path_history_idx;
      }
    }

    else 
    {
      for (const VertexLength& vl : graph[qe.vertex_num]) {

	int64_t cumm_length = qe.cumm_length + vl.length;

	// If the current cummulative length is already greater than our current solution then there is no need to keep it
	if (cumm_length >= solution.cumm_length) {
	  // Throw it away
	}
     
	// If we have seen this vertex before, don't keep it if it is longer than a previous visit
	else if (v[vl.vertex_num].visited && (cumm_length >= v[vl.vertex_num].cumm_length)) {
	  // Throw it away
	}

	else {

	  path_history.emplace_back(qe.path_history_idx, vl.vertex_num);
	  q.emplace (vl.vertex_num, cumm_length, path_history.size()-1);
	  v[vl.vertex_num].visited = true;
	  v[vl.vertex_num].cumm_length = cumm_length;
	}
      }
    }
  }

  // Now reconstruct the path traversing the path history from the back to the front
  VertexList answer;
  int64_t i = solution.path_history_idx;
  while (i != -1) {
      const BreadCrumb& crumb = path_history[i];
      answer.push_front(crumb.vertex_num);
      i = crumb.parent_path_history_idx;
  }

  return answer;
}

// -------------
// dijkstra_print
// -------------

void dijkstra_print (ostream& w, const VertexList& answer) {
  if (answer.size() == 0) {
    w << "-1" << std::endl;
  } else {
    for (int vertex : answer) {
      w << vertex+1 << " ";
    }
    w << std::endl;
  }
}

// -------------
// dijkstra_solve
// -------------

void dijkstra_solve (istream& r, ostream& w) {
  std::string s;

  int num_vertices = 0;
  int num_edges = 0;
  if (getline(r, s))
  {
    std::istringstream sin(s);
    sin >> num_vertices >> num_edges;
  }

  Graph graph (num_vertices);
  for (int i = 0; i < num_edges; ++i) {
    getline(r, s);
    std::istringstream sin(s);
    int vertex1 = 0;
    int vertex2 = 0;
    int length = 0;
    sin >> vertex1 >> vertex2 >> length;
    graph[vertex1-1].emplace_back(vertex2-1, length);
    graph[vertex2-1].emplace_back(vertex1-1, length);
  }

  const VertexList answer = dijkstra_eval(graph, 0, num_vertices-1);
  dijkstra_print(w, answer);
}
