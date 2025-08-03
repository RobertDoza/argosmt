#include "graph_lex_minimal_constraint_handler.hpp"

#define GRAPH_LEX_MIN_LOG

namespace {
#ifdef GRAPH_LEX_MIN_LOG
    void log_message(const std::string& msg) {
        std::cout << msg << std::endl;
    }
#endif // GRAPH_LEX_MIN_LOG
} // namespace

graph_lex_minimal_constraint_handler::graph_lex_minimal_constraint_handler(csp_theory_solver* th, const expression& l_pos, const expression& l_neg)
    : constraint_handler(th, l_pos, l_neg), _common_data(&th->_graph_lex_minimal_common_data), _next_to_assert(0),
      _graph_state(l_pos->get_symbol().get_indices()[0].get_unsigned_value()),
      _literal_to_vertex_pair_map(l_pos->get_operands()) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler constructor called");
    log_message(_graph_state.to_string());
#endif // GRAPH_LEX_MIN_LOG
    _common_data->_instance_count++;
    const expression_vector& ops = _constraint->get_operands();
    csp_theory_solver::csp_theory_solver_data* cons_data = _theory_solver->get_theory_solver_data(_constraint);
    for (unsigned i = 0; i < ops.size(); i++) {
        csp_theory_solver::csp_theory_solver_data* var_data = _theory_solver->add_variable(ops[i]);
        var_data->add_variable_constraint(_constraint);
        cons_data->add_constraint_variable(ops[i]);
    }

    for (unsigned i = 0; i < th->_current_level; i++)
        _trail.new_level();

#ifdef GRAPH_LEX_MIN_LOG
    log_message(_literal_to_vertex_pair_map.to_string());
#endif // GRAPH_LEX_MIN_LOG
}

void graph_lex_minimal_constraint_handler::new_level() {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler new_level() called");
#endif // GRAPH_LEX_MIN_LOG
    _trail.new_level();
    // TODO
    _graph_state.new_level();
#ifdef GRAPH_LEX_MIN_LOG
    log_message(_graph_state.to_string());
#endif // GRAPH_LEX_MIN_LOG
}

void graph_lex_minimal_constraint_handler::backjump(unsigned level) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler backjump() called");
#endif // GRAPH_LEX_MIN_LOG

#ifdef GRAPH_LEX_MIN_LOG
    std::stringstream message_before_backjump;
    message_before_backjump << "Before backjump: " << std::endl;
    _trail.out(message_before_backjump);
    message_before_backjump << std::endl;
    log_message(message_before_backjump.str());
#endif // GRAPH_LEX_MIN_LOG

    assert(level < _trail.current_level());
    _trail.backjump(level);
    _next_to_assert = _trail.size();
    // TODO
    _graph_state.backjump(level);

#ifdef GRAPH_LEX_MIN_LOG
    std::stringstream message_after_backjump;
    message_after_backjump << "After backjump: " << std::endl;
    _trail.out(message_after_backjump);
    message_after_backjump << std::endl;
    log_message(message_after_backjump.str());
#endif // GRAPH_LEX_MIN_LOG
}

void graph_lex_minimal_constraint_handler::assert_literal(const expression& l) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler assert_literal() called");
#endif // GRAPH_LEX_MIN_LOG
    if (_theory_solver->get_solver().is_conflict())
        return;

    if (_theory_solver->get_solver().get_trail().get_source_theory_solver(l) != _theory_solver ||
        _theory_solver->get_theory_solver_data(l)->get_explanation_handler() != this) {
        _trail.push(l);
    }
}

void graph_lex_minimal_constraint_handler::check_and_propagate(unsigned layer) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler check_and_propagate() called");
#endif // GRAPH_LEX_MIN_LOG
    if (_theory_solver->get_solver().is_conflict()) {
        return;
    }

    extended_boolean eb = _theory_solver->get_solver().get_trail().get_value(_constraint);
    if (eb == EB_UNDEFINED)
        return; // never happens

    if (_just_activated) {
        if (eb == EB_TRUE)
            propagate_weaker_from_pos_constraints();
        else
            propagate_weaker_from_neg_constraints();
        _just_activated = false;
    }

    if (eb == EB_FALSE) {
        return;
    }

    // TODO
}

void graph_lex_minimal_constraint_handler::explain_literal(const expression& l) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler explain_literal() called");
#endif // GRAPH_LEX_MIN_LOG
       // TODO
}

int graph_lex_minimal_constraint_handler::get_variable_value_hint(const expression& var) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler get_variable_value_hint() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
    return 0;
}

bool graph_lex_minimal_constraint_handler::is_weaker_from_pos(const expression& c) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler is_weaker_from_pos() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
    return false;
}

bool graph_lex_minimal_constraint_handler::is_weaker_from_neg(const expression& c) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler is_weaker_from_neg() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
    return false;
}

void graph_lex_minimal_constraint_handler::check_implied() {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler check_implied() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
}

void graph_lex_minimal_constraint_handler::handle_edge_literal(const expression& l) {
    auto symbol = l->get_symbol();
    auto operands = l->get_operands();
    auto edge_i_j = operands[0];
    auto [i, j] = _literal_to_vertex_pair_map.get_vertices(edge_i_j);
    i--;
    j--;
    auto value = operands[1]->get_special_constant().get_u_value();

    if (symbol == function_symbol::EQ) {
        if (value == 0) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::Zero);
            _responsibility_map[{i, j}] = l;
        } else if (value == 1) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::One);
            _responsibility_map[{i, j}] = l;
        } else {
            // TODO: ERROR
        }
        return;
    }

    if (symbol == function_symbol::DISTINCT) {
        if (value == 0) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::One);
            _responsibility_map[{i, j}] = l;
        } else if (value == 1) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::Zero);
            _responsibility_map[{i, j}] = l;
        } else {
            // TODO: ERROR
        }
        return;
    }

    if (symbol == function_symbol::GE) {
        if (value == 1) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::One);
            _responsibility_map[{i, j}] = l;
        } else if (value == 0) {
            // DO NOTHING
        }
        return;
    }

    if (symbol == function_symbol::LE) {
        if (value == 0) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::Zero);
            _responsibility_map[{i, j}] = l;
        } else if (value == 1) {
            // DO NOTHING
        }
        return;
    }

    if (symbol == function_symbol::GT) {
        if (value == 0) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::One);
            _responsibility_map[{i, j}] = l;
        } else if (value == 1) {
            // TODO: ERROR
        } else {
            // TODO: ERROR
        }
        return;
    }

    if (symbol == function_symbol::LT) {
        if (value == 0) {
            // TODO: ERROR
        } else if (value == 1) {
            _graph_state.set_entry(i, j, AdjacencyMatrixEntry::Zero);
            _responsibility_map[{i, j}] = l;
        } else {
            // TODO: ERROR
        }
        return;
    }
}

LiteralToVertexPairMap::LiteralToVertexPairMap(const expression_vector& expressions)
    :_map(parse_expressions(expressions))
{}

std::pair<std::size_t, std::size_t> LiteralToVertexPairMap::get_vertices(const expression& expr) const {
    auto it = _map.find(expr);
    if (it == _map.end()) {
        throw std::runtime_error("Expression not found in map");
    }
    return it->second;
}

std::string LiteralToVertexPairMap::to_string() const {
    std::stringstream buffer;
    for (auto &[expr, pair] : _map) {
        buffer << expr->to_string() << " -> " << pair.first << " " << pair.second << std::endl;
    }
    return buffer.str();
}

std::map<expression, std::pair<std::size_t, std::size_t>> LiteralToVertexPairMap::parse_expressions(const expression_vector& expressions) {
    if (expressions == EMPTY_EXPRESSION_VECTOR) {
        throw std::runtime_error("Empty expression vector");
    }

    std::map<expression, std::pair<std::size_t, std::size_t>> result;
    for (expression expr : expressions) {
        result[expr] = parse_edge_symbol_string(expr->to_string());
    }
    return result;
}

std::pair<std::size_t, std::size_t> LiteralToVertexPairMap::parse_edge_symbol_string(const std::string& edge_symbol_string) {
    const std::string prefix = "edge_";
    std::size_t prefix_len = prefix.length();

    if (edge_symbol_string.compare(0, prefix_len, prefix) != 0) {
        throw std::invalid_argument("Invalid format: expected prefix 'edge_'");
    }

    std::size_t sep_pos = edge_symbol_string.find('_', prefix_len);
    if (sep_pos == std::string::npos) {
        throw std::invalid_argument("Invalid format: expected two indices separated by '_'");
    }

    std::string i_str = edge_symbol_string.substr(prefix_len, sep_pos - prefix_len);
    std::string j_str = edge_symbol_string.substr(sep_pos + 1);

    try {
        std::size_t i = std::stoull(i_str);
        std::size_t j = std::stoull(j_str);
        return {i, j};
    } catch (...) {
        throw std::invalid_argument("Invalid format: indices must be non-negative integers");
    }
}

std::string GraphState::EdgeChangeAction::to_string() const {
    return "(" + std::to_string(row) + ", " + std::to_string(column) + "): " + value_char(old_value) + " --> " + value_char(new_value);
}

GraphState::GraphState(std::size_t num_vertices)
    :_adjacency_matrix(num_vertices)
{}

void GraphState::new_level() {
    _action_history.push_back(std::vector<EdgeChangeAction>());
}

void GraphState::set_entry(std::size_t row, std::size_t column, AdjacencyMatrixEntry new_value) {
    AdjacencyMatrixEntry old_value = _adjacency_matrix.get_entry(row, column);
    if (old_value == new_value) {
        return;
    }
    _adjacency_matrix.set_entry(row, column, new_value);
    _action_history.back().push_back(EdgeChangeAction{row, column, old_value, new_value});
}

AdjacencyMatrixEntry GraphState::get_entry(std::size_t i, std::size_t j) const {
    return _adjacency_matrix.get_entry(i, j);
}

AdjacencyMatrix GraphState::get_adjacency_matrix() const {
    return _adjacency_matrix;
}

void GraphState::backjump(std::size_t level) {
    std::size_t current_level = _action_history.size() - 1;
    assert(level < current_level);
    while (level != current_level) {
        while (!_action_history.back().empty()) {
            const EdgeChangeAction& action = _action_history.back().back();
            execute_reverse_edge_change_action(action);
            _action_history.back().pop_back();
        }
        _action_history.pop_back();
        current_level--;
    }
}

std::string GraphState::to_string() const {
    std::stringstream buffer;
    if (_action_history.empty()) {
        buffer << "<no actions>" << std::endl;
    } else {
        for (const auto& level : _action_history) {
            for (const auto& action : level) {
                buffer << action.to_string() << " ";
            }
            buffer << std::endl;
        }
    }
    buffer << _adjacency_matrix.to_string();
    return buffer.str();
}

void GraphState::execute_reverse_edge_change_action(const EdgeChangeAction& edge_change_action) {
    std::size_t row = edge_change_action.row;
    std::size_t column = edge_change_action.column;
    AdjacencyMatrixEntry old_value = edge_change_action.old_value;
    AdjacencyMatrixEntry new_value = edge_change_action.new_value;
    if (_adjacency_matrix.get_entry(row, column) != new_value) {
        throw std::runtime_error("Action is irreversible");
    }
    _adjacency_matrix.set_entry(row, column, old_value);
}

