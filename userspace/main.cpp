#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>

#include "./userspace/seedscript_intrptr/ss__lexer.hpp"
#include "./userspace/seedscript_intrptr/ss__ast.hpp"
#include "./userspace/seedscript_intrptr/ss__parser.hpp"
#include "./userspace/seedscript_intrptr/ss__commands.hpp"
#include "./userspace/seedscript_intrptr/ss__interp.hpp"

#include "./userspace/seedscript_intrptr/ss__commands_std.cpp"

static std::string load_file(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("cannot open: " + path);
    std::ostringstream SeedScript_intrptr; SeedScript_intrptr << f.rdbuf();
    return SeedScript_intrptr.str();
}

static const char* tokName(SeedScript_intrptr::TokenType t) {
    using T = SeedScript_intrptr::TokenType;
    switch (t) {
        case T::LParen: return "LParen";
        case T::RParen: return "RParen";
        case T::LBrace: return "LBrace";
        case T::RBrace: return "RBrace";
        case T::LBracket: return "LBracket";
        case T::RBracket: return "RBracket";
        case T::Comma: return "Comma";
        case T::Semicolon: return "Semicolon";
        case T::Colon: return "Colon";
        case T::Dot: return "Dot";
        case T::Pipe: return "Pipe";
        case T::PipePipe: return "PipePipe";
        case T::AmpAmp: return "AmpAmp";
        case T::Greater: return "Greater"; 
        case T::GreaterGreater: return "GreaterGreater"; 
        case T::Less: return "Less"; 
        case T::Equal: return "Equal";
        case T::EqualEqual: return "EqualEqual";
        case T::Bang: return "Bang";
        case T::BangEqual: return "BangEqual";
        case T::Plus: return "Plus";
        case T::Minus: return "Minus";
        case T::Star: return "Star";
        case T::Slash: return "Slash";
        case T::Percent: return "Percent";
        case T::Question: return "Question";
        case T::DoubleColon: return "DoubleColon";
        case T::Arrow: return "Arrow";
        case T::Dollar: return "Dollar";
        case T::DollarLParen: return "DollarLParen";
        case T::Ident: return "Ident";
        case T::Number: return "Number";
        case T::String: return "String";
        case T::KwLet: return "KwLet";
        case T::KwConst: return "KwConst";
        case T::KwIf: return "KwIf";
        case T::KwElse: return "KwElse";
        case T::KwWhile: return "KwWhile";
        case T::KwFunction: return "KwFunction";
        case T::KwReturn: return "KwReturn";
        case T::KwTrue: return "KwTrue";
        case T::KwFalse: return "KwFalse";
        case T::KwNull: return "KwNull";
        case T::Newline: return "Newline";
        case T::Comment: return "Comment";
        case T::EndOfFile: return "EOF";
        case T::RedirIn: return "RedirIn(<)";
        case T::RedirOut: return "RedirOut(>)";
        case T::RedirAppend: return "RedirAppend(>>)";
        case T::Error: return "Error";
        default: return "?";
    }
}

static void dumpTokens(const std::vector<SeedScript_intrptr::Token>& toks) {
    std::cout << "=== TOKENS ===\n";
    for (auto& tk : toks) {
        std::cout << std::left << std::setw(18) << tokName(tk.type)
                  << " lex='" << tk.lexeme << "'"
                  << " @" << tk.pos.line << ":" << tk.pos.col << "\n";
    }
    std::cout << "==============\n\n";
}


static void dumpWord(const SeedScript_intrptr::ASTWord& w) {
    std::cout << "\"";
    for (auto& part : w.parts) {
        if (std::holds_alternative<std::string>(part)) {
            std::cout << std::get<std::string>(part);
        } else {
            const auto& sub = std::get<SeedScript_intrptr::ASTSubst>(part);
            std::cout << "$(";
            
            for (size_t i=0; i<sub.pipe.cmds.size(); ++i) {
                if (i) std::cout << " | ";
                std::cout << sub.pipe.cmds[i].name;
                for (auto& a : sub.pipe.cmds[i].args) {
                    std::cout << " ";
                    
                    if (!a.parts.empty() && std::holds_alternative<std::string>(a.parts[0]))
                        std::cout << std::get<std::string>(a.parts[0]);
                    else
                        std::cout << "<expr>";
                }
            }
            std::cout << ")";
        }
    }
    std::cout << "\"";
}

static void dumpAST(const SeedScript_intrptr::ASTProgram& P) {
    std::cout << "=== AST ===\n";
    for (size_t pi=0; pi<P.pipes.size(); ++pi) {
        const auto& pl = P.pipes[pi];
        std::cout << "Pipeline[" << pi << "] @"
                  << pl.pos.line << ":" << pl.pos.col << "\n";
        for (size_t ci=0; ci<pl.cmds.size(); ++ci) {
            const auto& cmd = pl.cmds[ci];
            std::cout << "  Cmd[" << ci << "]: " << cmd.name
                      << " @" << cmd.pos.line << ":" << cmd.pos.col << "\n";
            
            for (size_t ai=0; ai<cmd.args.size(); ++ai) {
                std::cout << "    Arg[" << ai << "]: ";
                dumpWord(cmd.args[ai]);
                std::cout << "\n";
            }
            
            for (size_t ri=0; ri<cmd.redir.size(); ++ri) {
                const auto& r = cmd.redir[ri];
                std::cout << "    Redir[" << ri << "]: "
                          << (r.kind==SeedScript_intrptr::ASTRedir::In ? "<" : (r.kind==SeedScript_intrptr::ASTRedir::Out ? ">" : ">>"))
                          << " target=";
                dumpWord(r.target);
                std::cout << "\n";
            }
        }
    }
    std::cout << "===========\n\n";
}

static void showOutTxtIfExists() {
    std::ifstream f("out.txt", std::ios::binary);
    if (!f) return;
    std::cout << "--- out.txt ---\n"
              << std::string(std::istreambuf_iterator<char>(f), {})
              << "----------------\n";
}



int main(int argc, char** argv) {
    try {
        const std::string script_path = (argc > 1) ? argv[1] : "demo.seeds";
        std::string src = load_file(script_path);

        std::cout << "=== SOURCE (" << script_path << ") ===\n"
                  << src << "\n"
                  << "===============================\n\n";

        
        SeedScript_intrptr::Lexer L(src);
        auto toks = L.lex();
        dumpTokens(toks);

        
        SeedScript_intrptr::Parser P(toks);
        auto prog = P.parseProgram();
        dumpAST(prog);

        
        SeedScript_intrptr::Interpreter I;          
        int status = I.runProgram(prog);

        std::cout << "[EXEC] exit status = " << status << "\n";

        
        showOutTxtIfExists();

        return status;
    } catch (const SeedScript_intrptr::ParseError& e) {
        std::cerr << "Parse error: " << e.what() << "\n";
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
