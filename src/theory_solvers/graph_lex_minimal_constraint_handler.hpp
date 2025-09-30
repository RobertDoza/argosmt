#ifndef _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H
#define _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H

#include "backjump_stack.hpp"
#include "constraint_handler.hpp"
#include "csp_explanation_minimizer.hpp"
#include "domain_handler.hpp"
#include "hash_vector.hpp"
#include "history_saver.hpp"
#include "adjacency_matrix.hpp"
#include "edge_literal.hpp"

class GraphState {
    struct EdgeChangeAction {
        std::size_t row;
        std::size_t column;
        AdjacencyMatrixEntry old_value;
        AdjacencyMatrixEntry new_value;

        std::string to_string() const;
    };
    public:
        GraphState(std::size_t num_vertices);
        void new_level();
        void set_entry(std::size_t row, std::size_t column, AdjacencyMatrixEntry new_value);
        AdjacencyMatrixEntry get_entry(std::size_t i, std::size_t j) const;
        AdjacencyMatrix get_adjacency_matrix() const;
        void backjump(std::size_t level);
        std::string to_string() const;
        friend std::ostream& operator<<(std::ostream& out, const GraphState& graph_state);
        friend std::ostream& operator<<(std::ostream& out, const GraphState::EdgeChangeAction& action);
    private:
        void execute_reverse_edge_change_action(const EdgeChangeAction& edge_change_action);
        std::string action_history_to_string() const;
    private:
        std::vector<std::vector<EdgeChangeAction>> _action_history;
        AdjacencyMatrix _adjacency_matrix;
};

class LiteralToVertexPairMap {
    public:
        LiteralToVertexPairMap(const expression_vector& expressions);
        std::pair<std::size_t, std::size_t> get_vertices(const expression& expr) const;
        std::string to_string() const;
        friend std::ostream& operator<<(std::ostream& out, const LiteralToVertexPairMap& map);
    private:
        static std::map<expression, std::pair<std::size_t, std::size_t>> parse_expressions(const expression_vector& expressions);
        static std::pair<std::size_t, std::size_t> parse_edge_symbol_string(const std::string& edge_symbol_string);
    private:
        std::map<expression, std::pair<std::size_t, std::size_t>> _map;
};

class graph_lex_minimal_constraint_handler : public constraint_handler {
public:
    graph_lex_minimal_constraint_handler(csp_theory_solver* th, const expression& l_pos, const expression& l_neg);
    virtual void new_level();
    virtual void backjump(unsigned level);
    virtual void assert_literal(const expression& l);
    virtual void check_and_propagate(unsigned layer);
    virtual void explain_literal(const expression& l);
    virtual int get_variable_value_hint(const expression& var);
    virtual bool is_weaker_from_pos(const expression& c);
    virtual bool is_weaker_from_neg(const expression& c);
    virtual void check_implied();
private:
    void handle_edge_literal(const expression& l);
    expression edge_literal_to_expression(const EdgeLiteral& literal);
    explanation generate_propagation_explanation(const expression& l);
private:
    graph_lex_minimal_common_data* _common_data;
    sparse_backjump_stack<expression> _trail;
    unsigned _next_to_assert;
    GraphState _graph_state;
    const LiteralToVertexPairMap _literal_to_vertex_pair_map;
    std::map<std::pair<std::size_t, std::size_t>, expression> _responsibility_map;
};

#endif // _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H

