#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <sstream>
#include <string>
#include <unordered_map>

#include "lexer.h"
#include "parser.h"


class CodeGen {
    std::unordered_map<std::string, int> var_offsets;

public:
    std::string EvaluateExpression(Node* node, int reg_index);
};

#endif  // CODE_GEN_H
