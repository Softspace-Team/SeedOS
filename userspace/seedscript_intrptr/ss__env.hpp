#pragma once
#include "./ss__value.hpp"
#include <optional>

namespace SeedScript_intrptr {
    struct Env : std::enable_shared_from_this<Env> {
        std::unordered_map<std::string, Value> table;
        std::shared_ptr<Env> parent;

        explicit Env(std::shared_ptr<Env> p=nullptr) : parent(std::move(p)) {}

        std::optional<Value> get(const std::string& k) const {
            auto it = table.find(k);
            if (it != table.end()) return it->second;
            if (parent) return parent->get(k);
            return std::nullopt;
        }
        void set(const std::string& k, Value v) { table[k] = std::move(v); }
    };
}