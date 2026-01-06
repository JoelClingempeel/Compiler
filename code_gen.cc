#include "code_gen.h"


std::vector<std::string> SCRATCH_REGS = {"r8", "r9", "r10", "r11", "r12",
                                         "r13", "r14", "r15"};

std::string asm_prefix = R"(
bits 64
section .text
global _main

_main:
push rbp
mov rbp, rsp
sub rsp, A
)";

std::string asm_postfix = R"(
mov rax, 0x02000001  ; sysexit
mov rdi, 0
syscall
)";

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
            return std::to_string(total_offset);
        } else {
            // Existing variable
            sstream << "mov " << out_reg << ", [rbp - " << var_offsets[lexeme] << "]\n";
            code += sstream.str();
            return out_reg;
        }
    } else if (token.type == TokenType::ADD) {
        std::string left_reg = EvaluateExpression(node->children[0].get());
        std::string right_reg = EvaluateExpression(node->children[1].get());
        sstream << "add " << left_reg << ", " << right_reg << "\n";
        code += sstream.str();
        return left_reg;
    } else if (token.type == TokenType::EQUALS) {
        std::string offset_str = EvaluateExpression(node->children[0].get());
        // std::string offset_str = "?";

        std::string reg = EvaluateExpression(node->children[1].get());
        sstream << "mov [rbp - " << offset_str << "], " << reg << "\n";
        code += sstream.str();
        return "";
    } else if (token.type == TokenType::LEFT_BRACE) {
        for (const auto& child : node->children) {
            EvaluateExpression(child.get());
            reg_index = 0;
        }
        return "";
    } else {
        throw std::runtime_error("Unsupported node");
    }
}

std::string CodeGen::GetCode() {
    std::string offset_str = std::to_string(total_offset);
    std::string modified_asm_prefix = asm_prefix.replace(asm_prefix.size() - 2, 1, offset_str);
    return modified_asm_prefix + "\n" + code + asm_postfix;
}
