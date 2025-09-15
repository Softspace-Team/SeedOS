#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

namespace SeedScript_intrptr {
    enum class TokenType {
        LParen, RParen, LBrace, RBrace, LBracket, RBracket,
        Comma, Semicolon, Colon, Dot,
        Pipe, PipePipe, AmpAmp,
        Greater, GreaterGreater, Less,
        Equal, EqualEqual, Bang, BangEqual,
        Plus, Minus, Star, Slash, Percent,
        Question,
        DoubleColon, Arrow,

        Dollar, DollarLParen,

        Ident, Number, String,

        KwLet, KwConst, KwIf, KwElse, KwWhile, KwFunction, KwReturn,
        KwTrue, KwFalse, KwNull,

        Newline, Comment, EndOfFile,

        RedirIn, RedirOut, RedirAppend,

        Error
    };

    struct SourcePosition {
        uint32_t line = 1, col = 1;
        uint32_t offset = 0;
    };

    struct Token {
        TokenType type;
        std::string lexeme;
        SourcePosition pos;
    };

    class Lexer {
    public:
        explicit Lexer(std::string src) : text(std::move(src)) {}

        std::vector<Token> lex() {
            std::vector<Token> out;
            while (!eof()) {
                skipSpacesAndTabs();
                if (eof()) break;

                SourcePosition start = pos;

                if (peek()=='\n' || (peek()=='\r' && peek(1)=='\n')) {
                    consumeNewline(out); 
                    continue;
                }

                char c = get();

                if (c=='#') { consumeLineComment(); emit(out, TokenType::Comment, start, ""); continue; }
                if (c=='/' && peek()=='/') { get(); consumeLineComment(); emit(out, TokenType::Comment, start, ""); continue; }
                if (c=='/' && peek()=='*') { get(); consumeBlockComment(); emit(out, TokenType::Comment, start, ""); continue; }

                if (c=='"' || c=='\'') {
                    auto s = consumeString(c, start);
                    emit(out, s ? TokenType::String : TokenType::Error, start, s.value_or(""));
                    continue;
                }

                if (isDigit(c) || (c=='.' && isDigit(peek()))) {
                    std::string num(1,c);
                    bool dot = (c=='.');
                    while (isDigit(peek()) || (!dot && peek()=='.' && isDigit(peek(1)))) {
                        char nc = get();
                        if (nc=='.') dot=true;
                        num.push_back(nc);
                    }
                    
                    emit(out, TokenType::Number, start, num);
                    continue;
                }

                if (isIdentStart(c)) {
                    std::string id(1,c);
                    while (isIdentContinue(peek())) id.push_back(get());
                    
                    TokenType kw = keywordOrIdent(id);
                    emit(out, kw, start, id);
                    continue;
                }

                
                if (c=='$') {
                    if (peek()=='(') { get(); emit(out, TokenType::DollarLParen, start, "$("); continue; }
                    emit(out, TokenType::Dollar, start, "$");
                    continue;
                }

                
                switch (c) {
                    case ':':
                        if (peek()==':') { get(); emit(out, TokenType::DoubleColon, start, "::"); }
                        else emit(out, TokenType::Colon, start, ":");
                        break;
                    case '-':
                        if (peek()=='>') { get(); emit(out, TokenType::Arrow, start, "->"); }
                        else emit(out, TokenType::Minus, start, "-");
                        break;
                    case '&':
                        if (peek()=='&') { get(); emit(out, TokenType::AmpAmp, start, "&&"); }
                        else emit(out, TokenType::Error, start, "&");
                        break;
                    case '|':
                        if (peek()=='|') { get(); emit(out, TokenType::PipePipe, start, "||"); }
                        else emit(out, TokenType::Pipe, start, "|");
                        break;
                    case '>':
                        if (peek()=='>') { get(); emit(out, TokenType::RedirAppend, start, ">>"); }
                        else emit(out, TokenType::RedirOut, start, ">"); 
                        break;
                    case '<':
                        emit(out, TokenType::RedirIn, start, "<"); 
                        break;
                    case '=':
                        if (peek()=='=') { get(); emit(out, TokenType::EqualEqual, start, "=="); }
                        else emit(out, TokenType::Equal, start, "=");
                        break;
                    case '!':
                        if (peek()=='=') { get(); emit(out, TokenType::BangEqual, start, "!="); }
                        else emit(out, TokenType::Bang, start, "!");
                        break;
                    case '+': emit(out, TokenType::Plus, start, "+"); break;
                    case '*': emit(out, TokenType::Star, start, "*"); break;
                    case '/': emit(out, TokenType::Slash, start, "/"); break;
                    case '%': emit(out, TokenType::Percent, start, "%"); break;
                    case '?': emit(out, TokenType::Question, start, "?"); break;

                    case '(': emit(out, TokenType::LParen, start, "("); break;
                    case ')': emit(out, TokenType::RParen, start, ")"); break;
                    case '{': emit(out, TokenType::LBrace, start, "{"); break;
                    case '}': emit(out, TokenType::RBrace, start, "}"); break;
                    case '[': emit(out, TokenType::LBracket, start, "["); break;
                    case ']': emit(out, TokenType::RBracket, start, "]"); break;
                    case ',': emit(out, TokenType::Comma, start, ","); break;
                    case ';': emit(out, TokenType::Semicolon, start, ";"); break;
                    case '.': emit(out, TokenType::Dot, start, "."); break;

                    default:
                        
                        emit(out, TokenType::Error, start, std::string(1,c));
                        break;
                }
            }

            emit(out, TokenType::EndOfFile, pos, "");
            return out;
        }

    private:
        std::string text;
        SourcePosition pos{};
        size_t i = 0;

        inline bool eof() const { return i >= text.size(); }
        inline char peek(size_t k=0) const { return (i+k<text.size()) ? text[i+k] : '\0'; }
        inline char get() {
            char c = text[i++];
            pos.offset++;
            if (c=='\n') { pos.line++; pos.col=1; }
            else pos.col++;
            return c;
        }

        static inline bool isSpace(char c){ return c==' '||c=='\t'||c=='\v'||c=='\f'; }
        static inline bool isDigit(char c){ return c>='0' && c<='9'; }
        static inline bool isIdentStart(char c){
            return (c=='_' || c=='@' || (c>='a'&&c<='z') || (c>='A'&&c<='Z'));
        }
        static inline bool isIdentContinue(char c){
            return isIdentStart(c) || isDigit(c) || c=='-';
        }

        void skipSpacesAndTabs() {
            while (!eof() && isSpace(peek())) get();
        }

        void consumeNewline(std::vector<Token>& out) {
            SourcePosition start = pos;
            if (peek()=='\r' && peek(1)=='\n') { get(); get(); }
            else if (peek()=='\n') { get(); }
            emit(out, TokenType::Newline, start, "\\n");
        }

        void consumeLineComment() { while (!eof() && peek()!='\n') get(); }

        void consumeBlockComment() {
            while (!eof()) {
                if (peek()=='*' && peek(1)=='/') { get(); get(); break; }
                get();
            }
        }

        std::optional<std::string> consumeString(char quote, const SourcePosition& startAt) {
            std::string s;
            
            while (!eof()) {
                char c = get();
                if (c==quote) return s;
                if (c=='\\') {
                    char e = get();
                    switch (e) {
                        case 'n': s.push_back('\n'); break;
                        case 'r': s.push_back('\r'); break;
                        case 't': s.push_back('\t'); break;
                        case '\\': s.push_back('\\'); break;
                        case '\'': s.push_back('\''); break;
                        case '"': s.push_back('"'); break;
                        default:  s.push_back(e); break;
                    }
                } else {
                    s.push_back(c);
                }
            }

            (void)startAt; 
            return std::nullopt;
        }

        TokenType keywordOrIdent(const std::string& id) {
            if (id == "let") return TokenType::KwLet;
            if (id == "const") return TokenType::KwConst;
            if (id == "if") return TokenType::KwIf;
            if (id == "else") return TokenType::KwElse;
            if (id == "while") return TokenType::KwWhile;
            if (id == "function") return TokenType::KwFunction;
            if (id == "return") return TokenType::KwReturn;
            if (id == "true") return TokenType::KwTrue;
            if (id == "false") return TokenType::KwFalse;
            if (id == "null") return TokenType::KwNull;
            return TokenType::Ident;
        }

        inline void emit(std::vector<Token>& out, TokenType t, const SourcePosition& start, std::string lex) {
            out.push_back(Token{t, std::move(lex), start});
        }
    };
}