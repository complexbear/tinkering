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
#include <functional>
#include <vector>

template<class T>
class Grid
{
public:
	typedef T Value;

	struct Cell
	{
		Cell() : score(0), prev(nullptr) {}

		int score;
		Value val;
		Cell* prev;
	};

	Grid(const int rows, const int cols)
		: ROWS(rows), COLS(cols)
	{
		cells_ = new Cell[ROWS * COLS];
	}

	Grid(const std::vector<Value>& rowData, const std::vector<Value>& colData)
		: ROWS(rowData.size()), 
		  COLS(colData.size()),
		  rowData_(rowData),
		  colData_(colData)
	{
		cells_ = new Cell[ROWS * COLS];
	}

	~Grid()
	{
		delete[] cells_;
	}

	// Initialize the grid scores
	void init_scores(std::binary_function<int, int, int> func)
	{
		for (int r = 0; r < ROWS; ++r)
			for (int c = 0; c < COLS; ++c)
				idx(r, c).score = func(r, c);
	}

	// Helper function to index into the 2d results array
	// using row and column indexes
	Cell* idx(const int r, const int c)
	{
		static Cell boundary;
		if (r < 0 || r >= ROWS) return &boundary;
		if (c < 0 || c >= COLS) return &boundary;

		return (cells_ + (r * COLS) + c);
	}

	// Printer function for grid
	void print() 
	{
		std::stringstream s;
		auto printLine = [&]() {
			s << ' ' << std::string(ROWS * 5, '-') + "\n";
		};

		printLine();
		s << "  ";
		for (int c = 0; c < COLS; ++c)
			s << " | " << colData_[c];
		s << '\n';
		printLine();
		for (int r = 0; r < ROWS; ++r)
		{
			s << ' ' << rowData_[r];
			for (int c = 0; c < COLS; ++c)
			{
				s << " | " << idx(r, c)->score;
			}
			s << " |\n";
		}
		printLine();
		std::cout << s.str() << std::endl;
	};

	void fill(std::function<void(const int, const int, Cell*)> fillFunc)
	{
		for (int r = 0; r < ROWS; ++r)
		{
			for (int c = 0; c < COLS; ++c)
			{
				fillFunc(r, c, idx(r, c));
			}
		}
	}

	// Follow a path from bottom right back to top left along the highest scores
	std::vector<Value> extract_seq() 
	{
		std::vector<Value> result;
		auto current = idx(ROWS - 1, COLS - 1);
		while (current->prev)
		{
			if (current->score > current->prev->score)
			{
				result.push_back(current->val);
			}
			current = current->prev;
		}
		return std::vector<T>(result.rbegin(), result.rend());
	};
	
private:
	const int ROWS, COLS;
	const std::vector<Value> rowData_, colData_;
	Cell* cells_;
};



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
	Grid<char> grid(std::vector<char>(s1.begin(), s1.end()), 
				    std::vector<char>(s2.begin(), s2.end()));
	
	
	// Score the alignment of the two sequences, working from top left
	// to bottom right accumulating the scores from previous cells
	auto fillCell = [&](const int row, const int col, Grid<char>::Cell* target)
	{
		// If the character in s1 and s2 at these indices are the same then +1 to the previous score (top left cell from this position)
		// Otherwise take the max score from the prev calcs
		if (s1[row] == s2[col])
		{
			auto topleft = grid.idx(row - 1, col - 1);
			target->score = topleft->score + 1;
			target->prev = topleft;
			target->val = s1[row];
		}
		else
		{
			auto left = grid.idx(row - 1, col);
			auto top = grid.idx(row, col - 1);
			auto prev = (left->score > top->score) ? left : top;
			target->score = prev->score;
			target->prev = prev;
		}
	};

	grid.fill(fillCell);
	grid.print();

	auto result = grid.extract_seq();
	return std::string(result.begin(), result.end());
}


int main(int argc, const char* argv[])
{
	std::string strand1 = "GAGGTCAG";
	std::string strand2 = "xGAxGxTxAG";

	std::string common_strand = align(strand1, strand2);
	
	std::cout << strand1 << std::endl;
	std::cout << strand2 << std::endl;
	std::cout << common_strand << std::endl;

    return 0;
}

