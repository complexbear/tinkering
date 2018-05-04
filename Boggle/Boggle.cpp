// Boggle.cpp
// Given a Boggle board and a dictionary of words, find all possible words on the board
// This is an exercise in efficient search for known strings as we travel over a wordspace.
//
// Utilises a Trie data structure for tracking known word formation.
// Problem taken from geekforgeeks.com
//

#include <iostream>
#include <set>
#include <vector>

#include "Trie.h"

// A matrix containing our boggle board letters
typedef std::vector<std::vector<char>> BBoard;

// A mask so we know which cells in the board we have already visited
typedef std::vector<std::vector<bool>> Visited; 


/*
 * From the starting cell defined by rpos,cpos search the 
 * surrounding cells to find potential words in the dictionary.
 *
 * We avoid considering cells that have already been visited.
 *
 * The string s contains the letters we have gathered so far from 
 * previous calls to this function.
 *
 * The Node n represents the current position that we are at in 
 * the Trie structure which is representing our dictionary.
 */
std::set<std::string> find( const BBoard& board, 
						    Visited& visited, 
							const int rpos, 
							const int cpos, 
							Node* n, 
							std::string s = std::string()) 
{
	const int maxRows = board.size(),
			  maxCols = board[0].size();

	std::set<std::string> foundStrings;

	visited[rpos][cpos] = true;
	
	// Is this board cell in the dict?
	const char c = board[rpos][cpos];
	if (n->val == c)
	{
		std::cout << "found " << s << "+" << c << std::endl;
		s += c;
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

/*
 * Entry point for searching the board for words that appear in the dictionary.
 *
 * We try to find words starting at each cell in the board, and then combine
 * all the found words together for return.
 */
std::set<std::string> findWords(const BBoard& board, const Trie& dict)
{
	// A clear bool mask the same size of the board
	Visited clear_mask;
	for (size_t i = 0; i < board.size(); ++i)
		clear_mask.push_back(std::vector<bool>(board[0].size(), false));

	std::set<std::string> results;
	for (size_t r = 0; r < board.size(); ++r)
	{
		for (size_t c = 0; c < board[0].size(); ++c)
		{
			// Start with a clear mask
			Visited visited = clear_mask;
			Node* start = dict.root->get(board[r][c]);
			if (start)
			{
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
		{'O', 'G', 'L'},
		{'P', 'X', 'T'}
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

