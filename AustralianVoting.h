
#ifndef AustralianVoting_h
#define AustralianVoting_h

// --------
// includes
// --------

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <vector>

typedef std::list<int> Ballot;
typedef std::vector<Ballot> Ballots;
typedef std::vector<int> Winners;

// ------------
// australian_voting_eval
// ------------

Winners australian_voting_eval (const int num_candidates, Ballots& ballots);

// -------------
// australian_voting_print
// -------------

void australian_voting_print (std::ostream& w, const std::vector<int>& winners, const std::vector<std::string>& names);

// -------------
// australian_voting_solve
// -------------

void australian_voting_solve (std::istream& r, std::ostream& w);

#endif
