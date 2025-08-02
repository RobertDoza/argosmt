#include <stdexcept>

#include "adjacency_matrix.hpp"

AdjacencyMatrix::AdjacencyMatrix(const std::vector<std::vector<AdjacencyMatrixEntry>>& entries)
    :_dimension(entries.size()), _entries(entries)
{
    clear_diagonal();
    check_symmetry();
}

AdjacencyMatrix::AdjacencyMatrix(const std::vector<std::vector<unsigned>>& entries)
    :_dimension(entries.size()), _entries(_dimension, std::vector<AdjacencyMatrixEntry>(_dimension, AdjacencyMatrixEntry::Star))
{
    for (std::size_t i = 0; i < entries.size(); i++) {
        for (std::size_t j = 0; j < entries[i].size(); j++) {
            switch (entries[i][j]) {
                case 1:
                    _entries[i][j] = AdjacencyMatrixEntry::One;
                    break;
                case 0:
                    _entries[i][j] = AdjacencyMatrixEntry::Zero;
                    break;
                default:
                    _entries[i][j] = AdjacencyMatrixEntry::Star;
            }
        }
    }
}

AdjacencyMatrix::AdjacencyMatrix(std::size_t dim)
    :_dimension(dim), _entries(dim, std::vector<AdjacencyMatrixEntry>(dim, AdjacencyMatrixEntry::Star))
{
    clear_diagonal();
    check_symmetry();
}

AdjacencyMatrixEntry AdjacencyMatrix::get_entry(std::size_t i, std::size_t j) const {
    return _entries[i][j];
}

AdjacencyMatrixEntry AdjacencyMatrix::get_entry(const std::pair<std::size_t, std::size_t>& vertex_pair) const {
    return _entries[vertex_pair.first][vertex_pair.second];
}

std::size_t AdjacencyMatrix::get_dimension() const {
    return _dimension;
}

std::string AdjacencyMatrix::to_string() const {
    std::string s;

    for (const std::vector<AdjacencyMatrixEntry>& row : _entries) {
        for (AdjacencyMatrixEntry entry : row) {
            s += value_char(entry);
            s += " ";
        }
        s += "\n";
    }
    
    return s;
}

bool AdjacencyMatrix::is_lexicographically_less(const AdjacencyMatrix& other) const {
    for (std::size_t i = 0; i < _dimension; i++) {
        for (std::size_t j = i + 1; j < _dimension; j++) {
            if (this->_entries[i][j] != other._entries[i][j]) {
                return this->_entries[i][j] < other._entries[i][j];
            }
        }
    }
    return false;
}

AdjacencyMatrix AdjacencyMatrix::permute(const std::vector<unsigned>& permutation) const {
    if (permutation.size() != this->_dimension) {
        // TODO: rethink what type of error should be thrown
        throw std::runtime_error("Permutation of inadequate size!");
    }

    AdjacencyMatrix permuted_matrix(_dimension);

    for (std::size_t i = 0; i < _dimension; i++) {
        for (std::size_t j = i + 1; j < _dimension; j++) {
            permuted_matrix._entries[permutation[i]][permutation[j]] = this->_entries[i][j];
            permuted_matrix._entries[permutation[j]][permutation[i]] = this->_entries[j][i];
        }
    }

    return permuted_matrix;
}

AdjacencyMatrix AdjacencyMatrix::permute(const Permutation& permutation) const {
    return permute(permutation.permuted_elements);
}

void AdjacencyMatrix::set_entry(std::size_t i, std::size_t j, AdjacencyMatrixEntry entry) {
    _entries[i][j] = entry;
    _entries[j][i] = entry;
}

void AdjacencyMatrix::clear_diagonal() {
    for (std::size_t i = 0; i < _dimension; i++) {
        _entries[i][i] = AdjacencyMatrixEntry::Zero;
    }
}

void AdjacencyMatrix::check_symmetry() const {
    for (std::size_t i = 0; i < _dimension; i++) {
        for (std::size_t j = i + 1; j < _dimension; j++) {
            if (_entries[i][j] != _entries[j][i]) {
                // TODO: rethink type of exception that should be thrown
                throw std::runtime_error("Matrix not symmetric!");
            }
        }
    }
}

