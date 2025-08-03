#include <stdexcept>
#include <algorithm>

#include "general_ordered_partition.hpp"

GeneralOrderedPartition::GeneralOrderedPartition(std::size_t n)
    :_num_vertices(n)
{
    std::unordered_set<unsigned> all_vertices(n);
    for (unsigned i = 0; i < n; ++i) {
        all_vertices.insert(i);
    }
    _triples.push_back({all_vertices, 0, static_cast<unsigned>(n - 1)});
}

GeneralOrderedPartition::GeneralOrderedPartition(const std::vector<PartitionTriple>& t)
    :_triples(t)
{
    _num_vertices = 0;
    for (PartitionTriple triple : t) {
        _num_vertices += (triple.upper - triple.lower + 1);
    }
}

std::size_t GeneralOrderedPartition::size() const {
    return _triples.size();
}

std::string GeneralOrderedPartition::to_string() const {
    std::string s;
    s += "[";
    for (std::size_t i = 0; i < _triples.size(); i++) {
        s += _triples[i].to_string();
        if (i < _triples.size() - 1) {
            s += ", ";
        }
    }
    s += "]";
    return s;
}

Permutation GeneralOrderedPartition::current_permutation() const {
    std::vector<unsigned> pi(_num_vertices);
    for (const auto& [vertices, l, u] : _triples) {
        std::size_t j = 0;
        for (unsigned v : vertices) {
            pi[v] = l + j;
            j++;
        }
    }
    return pi;
}

PartitionTriple GeneralOrderedPartition::get_triple(std::size_t index) const {
    return _triples[index];
}

std::vector<PartitionTriple> GeneralOrderedPartition::get_triples() const {
    return _triples;
}

void GeneralOrderedPartition::split_triple(std::size_t index, const PartitionTriple& new_triple_1, const PartitionTriple& new_triple_2) {
    new_triple_1.check_correctness();
    new_triple_2.check_correctness();

    if (new_triple_1.is_empty()) {
        _triples[index] = new_triple_2;
        return;
    } else if (new_triple_2.is_empty()) {
        _triples[index] = new_triple_1;
        return;
    }

    std::size_t new_size = _triples.size() + 1;
    _triples.resize(new_size);
    for (std::size_t i = new_size - 1; i > index + 1; i--) {
        _triples[i] = _triples[i - 1];
    }
    _triples[index] = new_triple_1;
    _triples[index + 1] = new_triple_2;
}

void GeneralOrderedPartition::split_triple(const PartitionTriple& target_triple, const PartitionTriple& new_triple_1, const PartitionTriple& new_triple_2) {
    auto it = std::find(_triples.begin(), _triples.end(), target_triple);
    if (it != _triples.end()) {
        std::size_t index = std::distance(_triples.begin(), it);
        split_triple(index, new_triple_1, new_triple_2);
    } else {
        throw std::runtime_error("Target triple not found");
    }
}

void GeneralOrderedPartition::check_correctness() const {
    for (std::size_t i = 0; i < _triples.size(); i++) {
        try {
            _triples[i].check_correctness();
        } catch (const std::exception& e) {
            throw std::runtime_error("Incorrect state of GOP");
        }

        if (i != 0) {
            if (_triples[i].lower != _triples[i - 1].upper + 1) {
                throw std::runtime_error("Incorrect state of GOP");
            }
        }
    }
}

std::ostream& operator<<(std::ostream& out, const GeneralOrderedPartition& gop) {
    out << gop.to_string();
    return out;
}

