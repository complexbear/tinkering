/*
 * A common task in DNA analysis is to align two fragments of DNA sequences against each other.
 * This lends itself to a Dynamic Programming style of solution where we are interested in
 * identifying the longest common subsequences of DNA bases.
 *
 * Inspired by IBM's DP articles here: http://ibm.com/developerworks/library/j-seqalign
 */


#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

template<class T>
struct Cell
{
	Cell() : score(0), prev(nullptr) {}
	
	int score;
	T val;
	Cell* prev;
};

typedef Cell<char> CCell;

std::string align(const std::string& s1, std::string& s2)
{
	/* Form an MxN matrix where M = length of s1, N = length of s2
	   This will hold the score as we try to align the sequences (s1, s2)

	        s1 ----> M
	 s2     ---G-----A-----T--------
	  |  A  |  0  |  2  |  2 
	  |  |  |------------------------
	  |  T  |  0  |  0  |  3
	  N     |
	*/
	const int M = static_cast<int>(s1.size()),
			  N = static_cast<int>(s2.size());
	CCell* scores = new CCell[M * N];

	// Helper function to index into the 2d results array
	// using row and column indexes
	auto idx = [&](const int& r, const int& c) -> CCell*
	{
		static CCell boundary;
		if (r < 0 || r >= M) return &boundary;
		if (c < 0 || c >= N) return &boundary;

		return (scores + (r * N) + c);
	};

	// Printer function for grid
	auto print_grid = [&]() 
	{
		std::stringstream s;
		for (int m = 0; m < M; ++m)
		{
			s << ' ' << std::string(M*5, '-') + "\n";
			for (int n = 0; n < N; ++n)
			{
				s << " | " << idx(m, n)->score;
			}
			s << " |\n";
		}
		s << ' ' << std::string(M * 5, '-') + "\n";
		std::cout << s.str() << std::endl;
	};

	// Score the alignment of the two sequences, working from top left
	// to bottom right accumulating the scores from previous cells
	for (int m = 0; m < M; ++m)
	{
		for (int n = 0; n < N; ++n)
		{
			// If the character in s1 and s2 at these indices are the same then +1 to the previous score (top left cell from this position)
			// Otherwise take the max score from the prev calcs
			CCell* target = idx(m, n);
			if (s1[m] == s2[n])
			{
				CCell* topleft = idx(m - 1, n - 1);
				target->score = topleft->score + 1;
				target->prev = topleft;
				target->val = s1[m];
			}
			else
			{
				CCell* left = idx(m - 1, n);
				CCell* top  = idx(m, n - 1);
				CCell* prev = (left->score > top->score) ? left : top;
				target->score = prev->score;
				target->prev = prev;
			}
		}
	}
	print_grid();

	// Follow a path from bottom right back to top left along the highest scores
	auto extract_seq = [&]() -> std::string {
		std::string result;
		CCell* current = idx(M - 1, N - 1);
		while (current->prev)
		{
			if (current->score > current->prev->score)
			{
				result += current->val;
			}
			current = current->prev;
		}
		return std::string(result.rbegin(), result.rend());
	};
	
	const std::string result = extract_seq();
	delete[] scores;
	return result;
}


int main(int argc, const char* argv[])
{
	std::string strand1 = "GAGGTCAG";
	std::string strand2 = "_GA_G_T_AG";

	std::string common_strand = align(strand1, strand2);
	std::cout << common_strand << std::endl;

    return 0;
}

