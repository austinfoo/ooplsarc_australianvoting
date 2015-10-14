
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "gtest/gtest.h"

#include "Dijkstra.h"

using namespace std;

// -----------
// TestDijkstra
// -----------

// ----
// read
// ----

TEST(DijkstraFixture, read) {
    string s ("5 10\n");
    const std::pair<int, int> val = dijkstra_read(s);
    EXPECT_EQ(5, val.first);
    EXPECT_EQ(10, val.second);}

// ----
// eval
// ----

TEST(DijkstraFixture, eval_1) {
  const int val = dijkstra_eval(4, 6);
    EXPECT_EQ(2, val);}

TEST(DijkstraFixture, eval_2) {
  const int val = dijkstra_eval(10, 1);
    EXPECT_EQ(9, val);}

// -----
// print
// -----

TEST(DijkstraFixture, print) {
    ostringstream val;
    dijkstra_print(val, 9);
    EXPECT_EQ("9\n", val.str());}

// -----
// solve
// -----

TEST(DijkstraFixture, solve) {
    istringstream r("4 6\n10 1\n");
    ostringstream w;
    dijkstra_solve(r, w);
    EXPECT_EQ("2\n9\n", w.str());}

