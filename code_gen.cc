#include "code_gen.h"


std::vector<std::string> SCRATCH_REGS = {"r8", "r9", "r10", "r11", "r12",
                                         "r13", "r14", "r15"};

std::string CodeGen::EvaluateExpression(Node* node) {
    Token token = node->token;
    auto lexeme = std::string(token.lexeme);
    std::ostringstream sstream;
    if (token.type == TokenType::NUMBER) {
        std::string out_reg = SCRATCH_REGS[reg_index];
        reg_index++;
        sstream << "mov " << out_reg << ", " << lexeme << "\n";
        code += sstream.str();
        return out_reg;
    } else if (token.type == TokenType::IDENTIFIER) {
        std::string out_reg = SCRATCH_REGS[reg_index];
        reg_index++;
        if (var_offsets.find(lexeme) == var_offsets.end()) {
            // New variable
            total_offset += 8;
            var_offsets[lexeme] = total_offset;
        } else {
            // Existing variable
            sstream << "mov " << out_reg << ", [rsp + " << var_offsets[lexeme] << "]\n";
            code += sstream.str();
        }
        return out_reg;
    } else if (token.type == TokenType::ADD) {
        std::string left_reg = EvaluateExpression(node->children[0].get());
        std::string right_reg = EvaluateExpression(node->children[1].get());
        sstream << "add " << left_reg << ", " << right_reg << "\n";
        code += sstream.str();
        return left_reg;
    } else if (token.type == TokenType::EQUALS) {
        std::string var = std::string(node->children[0].get()->token.lexeme);
        int offset = var_offsets[var];
        std::string reg = EvaluateExpression(node->children[1].get());
        sstream << "mov [rbp - " << offset << "], " << reg << "\n";
        code += sstream.str();
        return "";
    } else {
        throw std::runtime_error("Unsupported node");
    }
}
