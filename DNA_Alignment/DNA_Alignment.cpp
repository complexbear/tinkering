/*
 * A common task in DNA analysis is to align two fragments of DNA sequences against each other.
 * This lends itself to a Dynamic Programming style of solution where we are interested in
 * identifying the longest common subsequences of DNA bases.
 *
 * Inspired by IBM's DP articles here: http://ibm.com/developerworks/library/j-seqalign
 */


#include <string>
#include <iostream>
#include <algorithm>


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
	const int M = s1.size(),
			  N = s2.size();
	int* scores = new int[M * N];

	// Helper function to index into the 2d results array
	// using row and column indexes
	auto idx = [&](const size_t& r, const size_t& c) -> int&
	{
		static int boundary = 0;
		if (r < 0 || r >= M) return boundary;
		if (c < 0 || c >= N) return boundary;

		return *(scores + (c * M) + r);
	};

	// Score the alignment of the two sequences, working from top left
	// to bottom right accumulating the scores from previous cells
	for (int m = 0; m < M; ++m)
	{
		for (int n = 0; n < N; ++n)
		{
			int score = s1[std::max(0, m-1)] == s2[std::max(0, n-1)] ? 1 : 0;
			int top = idx(m, n - 1),
				left = idx(m - 1, n),
				topleft = idx(m - 1, n - 1) + score;

			idx(m, n) = std::max(std::max(top, left), topleft);
		}
	}

	// Now follow a path from bottom right back to top left along the highest scores
	std::string result;
	int m = M - 1,
		n = N - 1;
	while (m > 0 && n > 0)
	{
		if (s1[m] == s2[n]) 
			result += s1[m];

		int top = idx(m, n-1),
			left = idx(m-1, n),
			topleft = idx(m-1, n-1);
		if (top > left)
			--n;
		else if (left > top)
			--m;
		else
			--m, --m;
	}

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

