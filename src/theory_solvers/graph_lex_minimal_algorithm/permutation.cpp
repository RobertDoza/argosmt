#include "permutation.hpp"

Permutation::Permutation(std::size_t n)
    :num_elements(n), permuted_elements(n)
{
    for (std::size_t i = 0; i < n; i++) {
        permuted_elements[i] = i;
    }
}

Permutation::Permutation(const std::vector<unsigned>& elems)
    :num_elements(elems.size()), permuted_elements(elems)
{}

std::string Permutation::to_string() const {
    std::string s = "(";
    for (std::size_t i = 0; i < num_elements; i++) {
        s += " " + std::to_string(permuted_elements[i]) + " ";
    }
    s += ")";
    return s;
}

unsigned Permutation::operator()(unsigned x) const {
    return permuted_elements[x];
}

