#include "token.hpp"
#include <fstream>
#include <set>

class AsmGenerator {
    int labelCounter = 0;

    void collectVariables(const std::vector<Node>& ast, std::set<std::string>& vars) {
        for (const auto& node : ast) {
            if (!node.varName.empty()) vars.insert(node.varName);
            collectVariables(node.trueBranch, vars);
            collectVariables(node.falseBranch, vars);
        }
    }

    void generateBlock(const std::vector<Node>& ast, std::ofstream& out) {
        for (const auto& node : ast) {
            if (node.kind == NodeKind::SET) {
                out << "    mov qword [" << node.varName << "], " << node.value << "\n";
            } 
            else if (node.kind == NodeKind::ADD) {
                out << "    add qword [" << node.varName << "], " << node.value << "\n";
            } 
            else if (node.kind == NodeKind::SUB) {
                out << "    sub qword [" << node.varName << "], " << node.value << "\n";
            } 
            else if (node.kind == NodeKind::MUL) {
                out << "    mov rax, [" << node.varName << "]\n";
                out << "    imul rax, " << node.value << "\n";
                out << "    mov [" << node.varName << "], rax\n";
            } 
            else if (node.kind == NodeKind::DIV) {
                out << "    mov rax, [" << node.varName << "]\n";//load varibale into rax for division
                out << "    xor rdx, rdx\n"; // Clear rdx for 64-bit division   //rax is dividend,  rbx is divisor, rdx will hold remainder
                out << "    mov rbx, " << node.value << "\n";
                out << "    idiv rbx\n";
                out << "    mov [" << node.varName << "], rax\n";
            } 
            else if (node.kind == NodeKind::SHOW) {
                out << "    mov rdi, fmt\n";
                out << "    mov rsi, [" << node.varName << "]\n";
                out << "    xor rax, rax\n";
                out << "    call printf\n";
            } 
            else if (node.kind == NodeKind::IF) {
                int currentLabel = labelCounter++;
                out << "    mov rax, [" << node.varName << "]\n";
                out << "    cmp rax, " << node.value << "\n";
                out << "    jne .L_ELSE_" << currentLabel << "\n";
                generateBlock(node.trueBranch, out);
                out << "    jmp .L_END_" << currentLabel << "\n";
                out << ".L_ELSE_" << currentLabel << ":\n";
                generateBlock(node.falseBranch, out);
                out << ".L_END_" << currentLabel << ":\n";
            } 
            else if (node.kind == NodeKind::WHILE) {
                int currentLabel = labelCounter++;
                out << ".L_WHILE_START_" << currentLabel << ":\n";
                out << "    mov rax, [" << node.varName << "]\n";
                out << "    cmp rax, " << node.value << "\n";
                out << "    jge .L_WHILE_END_" << currentLabel << "\n";
                generateBlock(node.trueBranch, out);
                out << "    jmp .L_WHILE_START_" << currentLabel << "\n";
                out << ".L_WHILE_END_" << currentLabel << ":\n";
            }
        }
    }

public:
    void generate(const std::vector<Node>& ast, const std::string& filename) {
        std::ofstream out(filename);
        std::set<std::string> variables;
        collectVariables(ast, variables);

        out << "section .data\n";
        out << "    fmt db \"%d\", 10, 0\n";
        
        out << "section .bss\n";
        for (const auto& var : variables) {
            out << "    " << var << " resq 1\n";
        }

        out << "section .text\n";
        out << "    global main\n";
        out << "    extern printf\n";
        out << "main:\n";
        out << "    push rbp\n"; 

        generateBlock(ast, out);

        out << "    pop rbp\n";
        out << "    mov rax, 60\n";
        out << "    xor rdi, rdi\n";
        out << "    syscall\n";
        
        out << "\nsection .note.GNU-stack noalloc noexec nowait progbits\n";
    }
};