#pragma once
#include "./ss__ast.hpp"
#include <stdexcept>

namespace SeedScript_intrptr {
    struct ParseError : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct Parser {
        const std::vector<Token>& t;
        size_t i=0;

        explicit Parser(const std::vector<Token>& toks) : t(toks) {}

        ASTProgram parseProgram() {
            ASTProgram P;

            while (!match(TokenType::EndOfFile)) {
                skipNoise();

                if (peek().type==TokenType::EndOfFile) break;

                auto pipe = parsePipeline();

                P.pipes.push_back(std::move(pipe));

                while (accept(TokenType::Semicolon) || accept(TokenType::Newline) || accept(TokenType::Comment)) {}
            }

            return P;
        }

    private:
        ASTPipeline parsePipeline() {
            ASTPipeline p; p.pos = peek().pos;

            skipNoise();

            p.cmds.push_back(parseCommand());
            while (accept(TokenType::Pipe)) {
                skipNoise();
                p.cmds.push_back(parseCommand());
            }
            return p;
        }

        ASTCommand parseCommand() {
            ASTCommand c; c.pos = peek().pos;

            c.name = parseQualifiedName();

            for (;;) {
                skipNoise();
                TokenType k = peek().type;
                if (k==TokenType::Pipe || k==TokenType::Semicolon || k==TokenType::Newline ||
                    k==TokenType::RParen || k==TokenType::EndOfFile) break;

                if (k==TokenType::RedirIn || k==TokenType::RedirOut || k==TokenType::RedirAppend) {
                    c.redir.push_back(parseRedir());
                    continue;
                }

                if (auto mw = tryParseWord()) { c.args.push_back(std::move(*mw)); continue; }
                break;
            }

            return c;
        }

        std::optional<ASTWord> tryParseWord() {
            ASTWord w;
            bool any = false;

            auto append_str = [&](const std::string& s) {
                if (!w.parts.empty() && std::holds_alternative<std::string>(w.parts.back())) {
                    std::get<std::string>(w.parts.back()) += s;
                } else {
                    w.parts.emplace_back(s);
                }
            };
            
            if (accept(TokenType::String)) {
                append_str(prev().lexeme); any = true;
            } else if (accept(TokenType::Number)) {
                append_str(prev().lexeme); any = true;
            } else if (accept(TokenType::Ident)) {
                append_str(prev().lexeme); any = true;

                while (accept(TokenType::Dot)) {
                    
                    if (accept(TokenType::Ident)) {
                        append_str("." + prev().lexeme);
                    } else {
                        --i;
                        break;
                    }
                }
            } else if (accept(TokenType::DollarLParen)) {
                auto sub = parseSubst();
                w.parts.emplace_back(std::move(sub)); any = true;
            }

            
            bool appended = true;
            while (appended) {
                appended = false;
                if (accept(TokenType::String)) {
                    append_str(prev().lexeme); any = true; appended = true;
                } else if (accept(TokenType::DollarLParen)) {
                    auto sub = parseSubst();
                    w.parts.emplace_back(std::move(sub)); any = true; appended = true;
                }
            }

            if (!any) return std::nullopt;
            return w;
        }

        ASTWord parseWord() {
            auto w = tryParseWord();
            if (!w) throw ParseError("expected word");
            return *w;
        }

        ASTRedir parseRedir() {
            ASTRedir r;
            if (accept(TokenType::RedirIn)) r.kind = ASTRedir::In;
            else if (accept(TokenType::RedirAppend)) r.kind = ASTRedir::Append;
            else if (accept(TokenType::RedirOut)) r.kind = ASTRedir::Out;
            else throw ParseError("expected redirection token");

            auto mw = tryParseWord();
            if (!mw) throw ParseError("expected file after redirection");
            r.target = std::move(*mw);
            return r;
        }

        ASTSubst parseSubst() {
            ASTSubst s; s.pos = peek().pos;
            auto pipe = parsePipeline();
            s.pipe = std::move(pipe);
            expect(TokenType::RParen, "expected ')' after substitution");
            return s;
        }

        std::string parseQualifiedName() {

            expect(TokenType::Ident, "expected command name");
            std::string name = prev().lexeme;
            while (accept(TokenType::DoubleColon)) {
                expect(TokenType::Ident, "expected identifier after '::'");
                name += "::" + prev().lexeme;
            }
            return name;
        }

        const Token& peek(size_t k=0) const { return t[ i+k < t.size()? i+k : t.size()-1 ]; }
        const Token& prev() const { return t[i-1]; }

        bool match(TokenType tp) const { return peek().type==tp; }
        bool accept(TokenType tp) { if (match(tp)) { ++i; return true; } return false; }
        void expect(TokenType tp, const char* msg) {
            if (!accept(tp)) throw ParseError(msg);
        }

        void skipNoise() {
            while (peek().type==TokenType::Comment || peek().type==TokenType::Newline) ++i;
        }
    };
}