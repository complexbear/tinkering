// Boggle.cpp
// Given a Boggle board and a dictionary of words, find all possible words on the board
//

#include <iostream>
#include <set>
#include <vector>

#include "Trie.h"

typedef std::vector<std::vector<char>> BBoard;
typedef std::vector<std::vector<bool>> Visited;

std::set<std::string> find(const BBoard& board, Visited& visited, const int rpos, const int cpos, Node* n, std::string s = std::string())
{
	const int maxRows = board.size(),
			  maxCols = board[0].size();

	std::set<std::string> foundStrings;

	visited[rpos][cpos] = true;
	
	// Is this board cell in the dict?
	if (n->val == board[rpos][cpos])
	{
		std::cout << "found " << s << "+" << board[rpos][cpos] << std::endl;
		s += board[rpos][cpos];
		if (n->marker) foundStrings.insert(s);
	}

	// Search neighbouring board cells
	for (int r = rpos-1; r <= rpos+1 && r<maxRows; ++r)
	{
		for (int c = cpos-1; c <= cpos+1 && c<maxCols; ++c)
		{
			if(r>=0 && c>=0 && !visited[r][c])
			{
				Node* next = n->get(board[r][c]);
				if (next)
				{
					auto moreStr = find(board, visited, r, c, next, s);
					foundStrings.insert(moreStr.begin(), moreStr.end());
				}				
			}
		}
	}
	return foundStrings;
}

std::set<std::string> findWords(const BBoard& board, const Trie& dict)
{
	std::set<std::string> results;
	
	// When moving to a neighbour cell, append it to the parent cell to build words
	for (size_t r = 0; r < board.size(); ++r)
	{
		for (size_t c = 0; c < board.size(); ++c)
		{
			Visited visited;
			for (size_t i = 0; i < board.size(); ++i)
				visited.push_back(std::vector<bool>(board[0].size(), false));

			Node* start = dict.root->get(board[r][c]);
			if (start)
			{
				std::string s;
				auto strings = find(board, visited, r, c, start);
				results.insert(strings.begin(), strings.end());
			}			
		}
	}
	return results;
}


int main()
{
	BBoard board{
		{'G', 'I', 'P'},
		{'O', 'T', 'E'},
		{'O', 'G', 'L'}
	};
	
	// Dictionary
	std::set<std::string> d{
		"GIT",
		"GOOGLE",
		"GOO"
	};
	
	// Convert dictionary to Trie for better search performance
	Trie t;
	for (const auto& w : d) t.addString(w);

	// Trie test
	std::cout << "Search GOO: " << t.search("GOO") << std::endl;
	std::cout << "Search GOT: " << t.search("GOT") << std::endl;
	std::cout << "Search GOOGLE: " << t.search("GOOGLE") << std::endl;

	std::cout << "Searching boggle board..." << std::endl;
	for (const auto& r : findWords(board, t))
		std::cout << r << std::endl;

    return 0;
}

