#pragma once
#include <string>
#include <vector>
#include <variant>
#include "./ss__lexer.hpp"

namespace SeedScript_intrptr {
    struct ASTSubst;

    struct ASTWord {
        using Node = std::variant<std::string, ASTSubst>;
        std::vector<Node> parts;
    };

    struct ASTRedir {
        enum Kind { In, Out, Append } kind;
        ASTWord target;
    };

    struct ASTCommand {
        std::string name;
        std::vector<ASTWord> args;
        std::vector<ASTRedir> redir;
        SourcePosition pos{};
    };

    struct ASTPipeline {
        std::vector<ASTCommand> cmds;
        SourcePosition pos{};
    };

    struct ASTSubst {
        ASTPipeline pipe;
        SourcePosition pos{};
    };

    struct ASTProgram {
        std::vector<ASTPipeline> pipes;
    };
}