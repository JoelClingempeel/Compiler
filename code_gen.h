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
    int next_label_index = 0;

public:
    std::string EvaluateRValue(Node* node);
    std::string EvaluateLValue(Node* node);
    std::string EvaluateAssignment(Node* node);
    std::string EvaluateStatements(Node* node);
    std::string EvaluateIfStatement(Node* node);
    std::string EvaluateWhileLoop(Node* node);
    void EvaluateFunction(FunctionNode* node);
    std::string GetCode();
};

#endif  // CODE_GEN_H
