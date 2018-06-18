#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

/*
 * Lookup table for custom weighting of matches between DNA bases
 */
struct SimilarityWeights
{
    SimilarityWeights()
    {
        // Default intialization is +1 for a match, -1 for a mismatch
        std::memset(weights, 0, sizeof(weights));
        for (int i = 0; i < BASE_COUNT; ++i)
            weights[i][i] = 1;

        indel = -2; // score for insertion or deletion
    }

    // Used to map base char value to index in weights array
    static const std::string baseIdx;
    static const char space;

    // Order of indexes into the array
    // A, G, C, T, U
    static const int BASE_COUNT = 5;
    int weights[BASE_COUNT][BASE_COUNT];
    int indel;

    int operator()(char c1, char c2) const
    {
        if (c1 == space || c2 == space)
            return indel;
        return weights[baseIdx.find(c1)][baseIdx.find(c2)];
    }
};


/*
 * Provides encapsulation of a 2D grid used to store score results
 * from comparing DNA sequences for optimal alignment.
 * 
 * The score initialization and computation is done
 * via external functions passed to the class.
 */
class Grid
{
public:
    typedef std::pair<std::string, std::string> StrandPair;
    
	struct Cell
	{
		Cell() : score(0), prev(nullptr) {}

        int score;
        size_t row, col;
		Cell* prev;
	};

    typedef std::function<void(const size_t, const size_t, Cell*)> CellEditFunction;

	Grid(const StrandPair& strands)	
	:   ROWS(strands.first.size() + 1),
		COLS(strands.second.size() + 1),
		rowData_(" " + strands.first),
		colData_(" " + strands.second)
	{
		// Need 1 extra row and column to hold the intial starting scores
		cells_ = new Cell[ROWS * COLS];
        // Initialize grid indices
        for_each([](size_t r, size_t c, Cell* cell) {
            cell->row = r;
            cell->col = c;
        });
	}

	~Grid()
	{
		delete[] cells_;
	}

    // The function that does it all, with help from a user 
    // supplied function
    StrandPair align()
    {
        if (initialize_function)
            for_each(initialize_function);

        if (scoring_function == nullptr)
            throw std::runtime_error("scoring function must be provided");
        
        print();
        for_each(scoring_function, 1);         
        print();

        return extract_seq();
    }

	// Helper function to index into the 2d results array
	// using row and column indexes
	Cell* idx(const size_t r, const size_t c)
	{
		return (cells_ + (r * COLS) + c);
	}

	// Printer function for grid debugging
	void print()
	{
		std::stringstream s;
		auto printLine = [&]() {
			s << ' ' << std::string(ROWS * 5, '-') + "\n";
		};

		printLine();
		s << "  ";
		for (size_t c = 0; c < COLS; ++c)
			s << " | " << colData_[c];
		s << '\n';
		printLine();
		for (size_t r = 0; r < ROWS; ++r)
		{
			s << ' ' << rowData_[r];
			for (size_t c = 0; c < COLS; ++c)
			{
				s << " | " << idx(r, c)->score;
			}
			s << " |\n";
		}
		printLine();
		std::cout << s.str() << std::endl;
	};

	// Follow a path from bottom right back to top left along the highest scores
    StrandPair extract_seq()
	{
		std::string strand1, strand2;
		auto current = idx(ROWS - 1, COLS - 1);
		while (current->prev)
		{
			// If prev is top left then it's either a match or a mismatch
            Cell* prev = current->prev;
            if (prev->row < current->row && prev->col < current->col)
            {
                strand1 += rowData_[current->row];
                strand2 += colData_[current->col];
            }
            // If prev is top or left from the current cell, then it's a gap
            else if(prev->row < current->row)
            {
                strand1 += '_';
                strand2 += colData_[current->col];
            }
            else
            {
                strand1 += rowData_[current->row];
                strand2 += '_';
            }
            current = prev;
		}
        return std::make_pair(std::string(strand1.rbegin(), strand1.rend()),
                              std::string(strand2.rbegin(), strand2.rend()));
	};

    // To be intialized by the user
    CellEditFunction initialize_function;
    CellEditFunction scoring_function;
    
private:
    // Commonly need to iterate over the grid, so this saves us
    // some boiler plate
    void for_each(CellEditFunction f, size_t start_at = 0) {
        for (size_t r = start_at; r < ROWS; ++r)
            for (size_t c = start_at; c < COLS; ++c)
                f(r, c, idx(r, c));
    }
    
	const size_t ROWS, COLS;
	const std::string rowData_, colData_;
	Cell* cells_;
};

