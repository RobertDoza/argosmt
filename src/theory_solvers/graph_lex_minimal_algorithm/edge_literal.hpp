#ifndef __EDGE_LITERAL_HPP__
#define __EDGE_LITERAL_HPP__

#include <utility>
#include <string>

struct EdgeLiteral {
    enum class Sign {
        Positive,
        Negative
    } sign;
    std::pair<unsigned, unsigned> vertex_pair;

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& out, const EdgeLiteral& edge_literal);
};

bool operator==(const EdgeLiteral& lhs, const EdgeLiteral& rhs);
bool operator!=(const EdgeLiteral& lhs, const EdgeLiteral& rhs);

#endif

