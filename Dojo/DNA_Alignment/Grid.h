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
class SimilarityWeights
{
public:
    SimilarityWeights(int match = 1, int miss = 0, int insertDelete = -1)
    {
        // Default intialization is +1 for a match, 0 for a mismatch
        std::memset(weights, miss, sizeof(weights));
        for (int i = 0; i < BASE_COUNT; ++i)
            weights[i][i] = match;

        indel = insertDelete; // score for insertion or deletion
    }

    // Used to map base char value to index in weights array
    static const std::string baseIdx;
    static const char space;

    // Order of indexes into the array
    // A, G, C, T, U
    static const int BASE_COUNT = 5;
    int indel;

    int operator()(char c1, char c2) const
    {
        if (c1 == space || c2 == space)
            return indel;
        return weights[baseIdx.find(c1)][baseIdx.find(c2)];
    }

private:
    int weights[BASE_COUNT][BASE_COUNT];
};


/*
 * Represents one cell in the grid used to compute alignments
 */
struct Cell
{
    Cell() : score(0), prev(nullptr) {}

    int score;          // Alignment score
    char val1, val2;    // The characters of the two DNA sequences at this point
    size_t row, col;    // Position of the cell
    Cell* prev;         // Backtrace pointer for reconstructing solution sequences
};

class Grid;

typedef std::pair<std::string, std::string> StrandPair;
typedef std::function<void(Grid&, Cell*)> CellEditFunction;


/*
 * Provides encapsulation of a 2D grid used to store score results
 * from comparing DNA sequences for optimal alignment.
 */
class Grid
{
public:

    Grid(const StrandPair& strands)
        : ROWS(strands.first.size() + 1), // Need 1 extra row and column to hold the intial starting scores
          COLS(strands.second.size() + 1)
    {
        cells_ = new Cell[ROWS * COLS];
        // Initialize grid indices and cell values
        for (size_t i = 0; i < (ROWS * COLS); ++i)
        {
            auto& cell = cells_[i];
            size_t row = i / COLS;
            size_t col = i % COLS;
            cell.row = row;
            cell.col = col;
            cell.val1 = row > 0 ? strands.first[row - 1] : ' ';
            cell.val2 = col > 0 ? strands.second[col - 1] : ' ';
        }
    }

    ~Grid()
    {
        delete[] cells_;
    }

    // Helper function to index into the 2d results array
    // using row and column indexes
    Cell* idx(const size_t r, const size_t c)
    {
        return (cells_ + (r * COLS) + c);
    }
    const Cell* idx(const size_t r, const size_t c) const
    {
        return (cells_ + (r * COLS) + c);
    }

    // Commonly need to iterate over the grid, so this saves us some boiler plate
    void for_each(CellEditFunction f, size_t start_at = 0)
    {
        for (size_t r = start_at; r < ROWS; ++r)
            for (size_t c = start_at; c < COLS; ++c)
                f(*this, idx(r, c));
    }

    const Cell* last() const
    {
        return idx(ROWS - 1, COLS - 1);
    }

    const size_t rows() const { return ROWS; }
    const size_t cols() const { return COLS; }

private:
	const size_t ROWS, COLS;
	Cell* cells_;
};

