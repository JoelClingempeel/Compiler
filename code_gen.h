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
    std::string code = "";

public:
    std::string EvaluateExpression(Node* node);
    std::string GetCode();
};

#endif  // CODE_GEN_H
