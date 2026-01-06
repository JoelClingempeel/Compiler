#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <sstream>
#include <string>
#include <unordered_map>

#include "lexer.h"
#include "parser.h"


class CodeGen {
    std::unordered_map<std::string, int> var_offsets;
    int total_offset = 0;
    int reg_index = 0;

public:
    std::string code = "";
    std::string EvaluateExpression(Node* node);
};

#endif  // CODE_GEN_H
