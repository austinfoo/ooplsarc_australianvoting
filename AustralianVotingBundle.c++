
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
#include <utility>
#include <cstdint>
#include <limits>



// ------------
// australian_voting_eval
// ------------

Winners australian_voting_eval (const int num_candidates, Ballots& ballots)
{
  // Total up the initial votes
  std::vector<int> votes (num_candidates, 0);
  for (const auto& ballot : ballots) {
    int can = ballot.front();
    ++votes[can];
  }

  // Initially none of the candidates are eliminated
  std::vector<bool> eliminated (num_candidates, false);
  int num_remaining_candidates = num_candidates;

  // How many votes do we need to outright win
  const int fifty_percent = ballots.size() / 2 + 1;

  while (true)
  {
    // Get the top and bottom vote candidates
    std::vector<int> top_vote_cans;
    int top_votes = std::numeric_limits<int>::min();
    std::vector<int> bottom_vote_cans;
    int bottom_votes = std::numeric_limits<int>::max();
    for (int c = 0; c < num_candidates; ++c) {
      if (!eliminated[c]) {
	if (votes[c] > top_votes) {
	  top_vote_cans.clear();
	  top_vote_cans.push_back(c);
	  top_votes = votes[c];
	} else if (votes[c] == top_votes) {
	  top_vote_cans.push_back(c);
	}
	if (votes[c] < bottom_votes) {
	  bottom_vote_cans.clear();
	  bottom_vote_cans.push_back(c);
	  bottom_votes = votes[c];
	} else if (votes[c] == bottom_votes) {
	  bottom_vote_cans.push_back(c);
	}
      }
    }

    // See if we have a winner
    if (top_vote_cans.size() == 1 && top_vote_cans[0] >= fifty_percent) {
      return top_vote_cans;
    }

    // See if all the candidates are tied
    if (top_vote_cans.size() == num_remaining_candidates) {
      return top_vote_cans;
    }

    // Find the lowest vote getting candidates and mark them eliminated and clear their vote count
    for (int can : bottom_vote_cans) {
      eliminated[can] = true;
      votes[can] = 0;
      --num_remaining_candidates;
    }
    
    // Now go through each ballot and remove eliminated candidates from the front of the ballot and add in new votes
    for (auto& ballot : ballots) 
    {
      if (eliminated[ballot.front()]) {
	bool done = false;
	while (!done) {
	  if (eliminated[ballot.front()]) {
	    ballot.pop_front();
	  } else {
	    ++votes[ballot.front()];
	    done = true;
	  }
	}
      }
    }

  }
}

// -------------
// australian_voting_print
// -------------

void australian_voting_print (std::ostream& w, const std::vector<int>& winners, const std::vector<std::string>& names) {
  for (int winner : winners) {
    w << names[winner] << std::endl;
  }
}

// -------------
// australian_voting_solve
// -------------

void australian_voting_solve (std::istream& r, std::ostream& w) 
{
  std::string s;

  // Get the number of elections
  int num_elections = 0;
  if (std::getline(r, s)) {
    std::istringstream sin(s);
    sin >> num_elections;
  }

  // Consume the obligatory blank line
  if (std::getline(r, s)) {
    assert (s.empty());
  }

  // Loop through the elections
  for (int e = 0; e < num_elections; ++e) {

    // Get the number of candidates
    int num_candidates = 0;
    if (std::getline(r, s)) {
      std::istringstream sin(s);
      sin >> num_candidates;
    }

    // Get the candidate names
    std::vector<std::string> names (num_candidates);
    for (int c = 0; c < num_candidates; ++c) {
      if (std::getline(r, names[c])) {
	assert(!names[c].empty());
      }
    }

    // Get the ballots
    Ballots ballots; 
    bool done = false;
    do
    {
      if (!std::getline(r, s)) {
	done = true;
      }	else if (s.empty()) {
	done = true;
      } else {
	ballots.emplace_back();
	Ballot& ballot = ballots.back();
	std::istringstream sin(s);
	for (int i = 0; i < num_candidates; ++i) {
	  int can = 0;
	  sin >> can;
	  ballot.emplace_back(can-1);
	}
      }
    }
    while (!done);

    //std::cout << num_elections << std::endl;
    //std::cout << num_candidates << std::endl;
    //std::cout << ballots.size() << std::endl;

    // Determine the winners and print the answer
    const Winners winners = australian_voting_eval(num_candidates, ballots);

    //std::cout << "Eval finished" << std::endl;

    australian_voting_print(w, winners, names);
    
    bool last = e == (num_elections-1);
    if (!last) {
      w << std::endl;
    }
  }
}

#include <iostream>



// ----
// main
// ----

int main () {
    using namespace std;
    australian_voting_solve(cin, cout);
    return 0;
}
