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

std::pair<unsigned, unsigned> Permutation::operator()(const std::pair<unsigned, unsigned>& pair) const {
    return {permuted_elements[pair.first], permuted_elements[pair.second]};
}

Permutation Permutation::get_inverse_permutation() const {
    std::vector<unsigned> inverse(num_elements);
    for (unsigned i = 0; i < permuted_elements.size(); i++) {
        inverse[permuted_elements[i]] = i;
    }
    return Permutation(inverse);
}

std::ostream& operator<<(std::ostream& out, const Permutation& permutation) {
    out << permutation.to_string();
    return out;
}

