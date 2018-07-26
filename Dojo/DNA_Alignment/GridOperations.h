#pragma once
#include "Grid.h"

// Follow a path from bottom right back to top left along the highest scores
StrandPair extract_seq(const Grid& grid)
{
    std::string strand1, strand2;
    auto current = grid.last();
    while (current->prev)
    {
        // If prev is top left then it's either a match or a mismatch
        Cell* prev = current->prev;
        if (current->score > prev->score)
        {
            strand1 += current->val1;
            strand2 += current->val2;
        }
        // If prev is top or left from the current cell, then it's a gap
        else
        {
            if (prev->row < current->row)
            { 
                strand1 += SimilarityWeights::space;
            }
            if (prev->col < current->col)
            {
                strand2 += SimilarityWeights::space;
            }
        }
        current = prev;
    }
    return std::make_pair(std::string(strand1.rbegin(), strand1.rend()),
                          std::string(strand2.rbegin(), strand2.rend()));
};


// Printer function for grid debugging
void print(const Grid& grid)
{
    std::stringstream s;
    auto printLine = [&]() {
        s << ' ' << std::string(grid.rows() * 6, '-') + "\n";
    };

    printLine();
    s << "  ";
    for (size_t c = 0; c < grid.cols(); ++c)
        s << " |  " << grid.idx(1, c)->val2;
    s << '\n';
    printLine();
    for (size_t r = 0; r < grid.rows(); ++r)
    {
        s << ' ' << grid.idx(r, 1)->val1;
        for (size_t c = 0; c < grid.cols(); ++c)
        {
            const auto v = grid.idx(r, c)->score;
            s << " | ";
            if (v >= 0) s << ' ';
            s << v;
            if (std::abs(v) < 10) s << ' ';
        }
        s << " |\n";
    }
    printLine();
    std::cout << s.str() << std::endl;
};


// The function that does it all, with help from a user 
// supplied function
StrandPair align(Grid& grid,
    CellEditFunction initialize_function,
    CellEditFunction scoring_function)
{
    if (initialize_function)
        grid.for_each(initialize_function);

    if (scoring_function == nullptr)
        throw std::runtime_error("scoring function must be provided");

    print(grid);
    grid.for_each(scoring_function, 1);
    print(grid);

    return extract_seq(grid);
}

