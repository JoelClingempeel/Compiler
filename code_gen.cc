#include "code_gen.h"


std::vector<std::string> SCRATCH_REGS = {"r8", "r9", "r10", "r11", "r12",
                                         "r13", "r14", "r15"};

std::unordered_map<TokenType, std::string> COMPARISONS = {
    {TokenType::DOUBLE_EQUALS, "sete"},
    {TokenType::LESS, "setl"},
    {TokenType::LESS_EQUALS, "setle"},
    {TokenType::GREATER, "setg"},
    {TokenType::GREATER_EQUALS, "setge"},
};

std::string asm_prefix = R"(
bits 64
section .text
global _main

_print:
    xor rcx, rcx
    mov rbx, 10
_print_loop:
    xor rdx, rdx
    div rbx
    add dl, '0'
    lea r8, [rel _buffer]
    mov [r8 + rcx], dl
    inc rcx
    test rax, rax
    jnz _print_loop
    mov r9, rcx
    lea rdi, [rel _buffer]
    lea rsi, [rel _buffer]
    add rsi, rcx
    dec rsi
_reverse_loop:
    cmp rdi, rsi
    jae _append_newline
    mov al, [rdi]
    mov bl, [rsi]
    mov [rdi], bl
    mov [rsi], al
    inc rdi
    dec rsi
    jmp _reverse_loop
_append_newline:
    lea r8, [rel _buffer]
    mov byte [r8 + r9], 10
    inc r9
_do_print:
    mov rax, 0x2000004
    mov rdi, 1
    lea rsi, [rel _buffer]
    mov rdx, r9
    syscall
    ret

_main:
push rbp
mov rbp, rsp
sub rsp, A
)";

std::string asm_postfix = R"(
mov rax, [rbp - 8]
call _print

mov rax, 0x02000001  ; sysexit
mov rdi, 0
syscall

section .data
_buffer:
    times 21 db 0
)";

std::string CodeGen::EvaluateRValue(Node* node) {
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
            throw std::runtime_error("Variable used before assignment");
        }
        // Existing variable
        sstream << "mov " << out_reg << ", [rbp - " << var_offsets[lexeme] << "]\n";
        code += sstream.str();
        return out_reg;
    } else if (token.type == TokenType::ADD) {
        std::string left_reg = EvaluateRValue(node->children[0].get());
        std::string right_reg = EvaluateRValue(node->children[1].get());
        sstream << "add " << left_reg << ", " << right_reg << "\n";
        code += sstream.str();
        return left_reg;
    } else if (token.type == TokenType::SUBTRACT) {
        std::string left_reg = EvaluateRValue(node->children[0].get());
        std::string right_reg = EvaluateRValue(node->children[1].get());
        sstream << "sub " << left_reg << ", " << right_reg << "\n";
        code += sstream.str();
        return left_reg;
    }  else if (token.type == TokenType::MULTIPLY) {
        std::string left_reg = EvaluateRValue(node->children[0].get());
        std::string right_reg = EvaluateRValue(node->children[1].get());
        sstream << "imul " << left_reg << ", " << right_reg << "\n";
        code += sstream.str();
        return left_reg;
    }  else if (token.type == TokenType::DIVIDE) {
        std::string left_reg = EvaluateRValue(node->children[0].get());
        std::string right_reg = EvaluateRValue(node->children[1].get());
        sstream << "mov rax, " << left_reg << "\n";
        sstream << "cqo\n";
        sstream << "idiv " << right_reg << "\n";
        sstream << "mov " << left_reg << ", rax\n";
        code += sstream.str();
        return left_reg;
    } else if (COMPARISONS.find(token.type) != COMPARISONS.end()) {
        std::string left_reg = EvaluateRValue(node->children[0].get());
        std::string right_reg = EvaluateRValue(node->children[1].get());
        sstream << "cmp " << left_reg << ", " << right_reg << "\n";
        sstream << COMPARISONS[token.type] << " al\n";
        sstream << "movzx " << left_reg << ", al\n";
        code += sstream.str();
        return left_reg;
    } else {
        throw std::runtime_error("Unsupported RValue");
    }
}

std::string CodeGen::EvaluateLValue(Node* node) {
    Token token = node->token;
    auto lexeme = std::string(token.lexeme);
    std::ostringstream sstream;
    if (token.type == TokenType::IDENTIFIER) {
        if (var_offsets.find(lexeme) == var_offsets.end()) {
            // New variable
            total_offset += 8;
            var_offsets[lexeme] = total_offset;
        }
        return std::to_string(var_offsets[lexeme]);
    } else {
        throw std::runtime_error("Expected identifier for LValue");
    }
}

std::string CodeGen::EvaluateAssignment(Node* node) {
    Token token = node->token;
    auto lexeme = std::string(token.lexeme);
    std::ostringstream sstream;
    if (token.type == TokenType::EQUALS) {
        std::string offset_str = EvaluateLValue(node->children[0].get());
        std::string reg = EvaluateRValue(node->children[1].get());
        sstream << "mov [rbp - " << offset_str << "], " << reg << "\n";
        code += sstream.str();
        return "";
    } else {
        throw std::runtime_error("Expected =");
    }
}

std::string CodeGen::EvaluateIfStatement(Node* node) {
    Token token = node->token;
    auto lexeme = std::string(token.lexeme);
    std::ostringstream sstream;
    std::string cond_reg = EvaluateRValue(node->children[0].get());
    sstream << "cmp " << cond_reg << ", 0\n";
    sstream << "jne _label" << next_label_index << "\n";
    code += sstream.str();
    sstream.str("");
    EvaluateStatements(node->children[1].get());
    sstream << "_label" << next_label_index << ":\n";
    code += sstream.str();
    next_label_index += 1;
    return "";
}

std::string CodeGen::EvaluateWhileLoop(Node* node) {
    Token token = node->token;
    auto lexeme = std::string(token.lexeme);
    std::ostringstream sstream;
    sstream << "_label" << next_label_index << ":\n";
    code += sstream.str();
    sstream.str("");
    std::string cond_reg = EvaluateRValue(node->children[0].get());
    sstream << "cmp " << cond_reg << ", 0\n";
    sstream << "je _label" << next_label_index + 1 << "\n";
    code += sstream.str();
    sstream.str("");
    EvaluateStatements(node->children[1].get());
    sstream << "jmp _label" << next_label_index << "\n";
    sstream << "_label" << next_label_index + 1 << ":\n";
    code += sstream.str();
    next_label_index += 2;
    return "";
}

std::string CodeGen::EvaluateStatements(Node* node) {
    Token token = node->token;
    auto lexeme = std::string(token.lexeme);
    std::ostringstream sstream;
    for (const auto& child : node->children) {
        if (child->token.type == TokenType::EQUALS) {
            EvaluateAssignment(child.get());
        } else if (child->token.type == TokenType::IF) {
            EvaluateIfStatement(child.get());
        } else if (child->token.type == TokenType::WHILE) {
            EvaluateWhileLoop(child.get());
        }
        reg_index = 0;
    }
    return "";
}

std::string CodeGen::GetCode() {
    std::string offset_str = std::to_string(total_offset);
    std::string modified_asm_prefix = asm_prefix.replace(asm_prefix.size() - 2, 1, offset_str);
    return modified_asm_prefix + "\n" + code + asm_postfix;
}
