#ifndef __PERMUTATION_HPP__
#define __PERMUTATION_HPP__

#include <cstddef>
#include <vector>
#include <string>

struct Permutation {
    std::size_t num_elements;
    std::vector<unsigned> permuted_elements;

    Permutation(std::size_t n);
    Permutation(const std::vector<unsigned>& elems);
    std::string to_string() const;
    unsigned operator()(unsigned x) const;
    std::pair<unsigned, unsigned> operator()(const std::pair<unsigned, unsigned>& pair) const;
};

#endif

