#ifndef __ADJACENCY_MATRIX_HPP__
#define __ADJACENCY_MATRIX_HPP__

#include <vector>
#include <string>

#include "adjacency_matrix_entry.hpp"
#include "permutation.hpp"

class AdjacencyMatrix {
    public:
        AdjacencyMatrix(const std::vector<std::vector<AdjacencyMatrixEntry>>& entries);
        AdjacencyMatrix(const std::vector<std::vector<unsigned>>& entries);
        AdjacencyMatrix(std::size_t dim);
        AdjacencyMatrixEntry get_entry(std::size_t i, std::size_t j) const;
        std::size_t get_dimension() const;
        std::string to_string() const;
        bool is_lexicographically_less(const AdjacencyMatrix& other) const;
        AdjacencyMatrix permute(const std::vector<unsigned>& permutation) const;
        AdjacencyMatrix permute(const Permutation& permutation) const;
        void set_entry(std::size_t i, std::size_t j, AdjacencyMatrixEntry entry);
    private:
        void clear_diagonal();
        void check_symmetry() const;
    private:
        const std::size_t _dimension;
        std::vector<std::vector<AdjacencyMatrixEntry>> _entries;
};

#endif

