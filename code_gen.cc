#include "code_gen.h"


std::vector<std::string> SCRATCH_REGS = {"r8", "r9", "r10", "r11", "r12",
                                         "r13", "r14", "r15"};

std::string CodeGen::EvaluateExpression(Node* node, int reg_index) {
    Token token = node->token;
    std::string out_reg = SCRATCH_REGS[reg_index];
    reg_index++;
    std::ostringstream out;
    if (token.type == TokenType::NUMBER) {
        out << "mov " << out_reg << ", " << token.lexeme;
    }
    return out.str();
}
