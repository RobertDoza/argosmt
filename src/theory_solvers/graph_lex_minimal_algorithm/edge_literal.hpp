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
};

#endif

