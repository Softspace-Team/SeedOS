#pragma once
#include "./ss__parser.hpp"
#include "./ss__commands.hpp"
#include <fstream>

namespace SeedScript_intrptr {
    struct Interpreter {
        Context ctx;
        CmdRegistry cmds;

        Interpreter() { install_std(cmds); }

        
        int runProgram(const ASTProgram& P) {
            int last = 0;
            for (auto& pipe : P.pipes) last = runPipeline(pipe).status;
            return last;
        }

        
        int runSource(const std::string& src) {
            Lexer L(src);
            auto toks = L.lex();
            Parser P(toks);
            auto prog = P.parseProgram();
            return runProgram(prog);
        }

    private:
        struct ExecResult { int status; std::shared_ptr<Stream> out, err; };

        ExecResult runPipeline(const ASTPipeline& p) {
            std::shared_ptr<Stream> prevOut; 
            ExecResult last{0, nullptr, nullptr};

            for (size_t i=0;i<p.cmds.size();++i) {
                const auto& c = p.cmds[i];
                CmdCall call;
                call.name = c.name;
                call.in  = prevOut;
                call.out = std::make_shared<Stream>();
                call.err = std::make_shared<Stream>();

                
                for (auto& w : c.args) call.args.push_back(evalWord(w));

                
                applyRedirsBefore(call, c.redir); 
                auto fn = cmds.find(call.name);
                if (!fn) {
                    
                    ExecResult er{127, call.out, call.err};
                    if (er.err) er.err->buf += "command not found: " + call.name + "\n";
                    return er;
                }
                auto res = (*fn)(call, ctx); 
                last.status = res.status;
                last.out = res.out;
                last.err = res.err;

                
                applyRedirsAfter(res, c.redir);

                
                prevOut = res.out;

                if (res.status != 0) break; 
            }
            return last;
        }

        std::string evalWord(const ASTWord& w) {
            std::string s;
            for (auto& part : w.parts) {
                if (std::holds_alternative<std::string>(part)) {
                    s += std::get<std::string>(part);
                } else {
                    
                    const auto& sub = std::get<ASTSubst>(part);
                    auto r = runPipeline(sub.pipe);
                    
                    std::string out = r.out ? r.out->buf : "";
                    if (!out.empty() && out.back()=='\n') out.pop_back();
                    s += out;
                }
            }
            return s;
        }

        void applyRedirsBefore(CmdCall& call, const std::vector<ASTRedir>& rs) {
            for (auto& r : rs) {
                if (r.kind != ASTRedir::In) continue;
                std::string path = evalWord(r.target);
                auto s = std::make_shared<Stream>();
                std::ifstream f(path, std::ios::binary);
                if (f) s->buf.assign(std::istreambuf_iterator<char>(f), {});
                call.in = s;
            }
        }
        void applyRedirsAfter(const CmdResult& res, const std::vector<ASTRedir>& rs) {
            for (auto& r : rs) {
                if (r.kind == ASTRedir::In) continue;
                std::string path = evalWord(r.target);
                std::ios::openmode mode = std::ios::binary | (r.kind==ASTRedir::Append ? std::ios::app : std::ios::trunc);
                std::ofstream f(path, mode);
                if (!f) continue;
                if (res.out) f << res.out->buf;
            }
        }
    };
}