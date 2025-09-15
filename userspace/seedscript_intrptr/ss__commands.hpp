#pragma once
#include "./ss__value.hpp"
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace SeedScript_intrptr {
    struct Context {
        
    };

    struct CmdCall {
        std::string name;
        std::vector<std::string> args;
        std::shared_ptr<Stream> in, out, err;
    };

    using CmdFn = std::function<CmdResult(CmdCall&, Context&)>;

    struct CmdRegistry {
        std::unordered_map<std::string, CmdFn> map;

        void add(const std::string& name, CmdFn fn) { map[name] = std::move(fn); }

        CmdFn* find(const std::string& name) {
            auto it = map.find(name);
            return (it==map.end()) ? nullptr : &it->second;
        }
    };

    void install_std(CmdRegistry& R);
}