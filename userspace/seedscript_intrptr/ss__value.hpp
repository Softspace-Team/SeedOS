#pragma once
#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace SeedScript_intrptr {
    struct Stream {
        std::string buf;
    };

    struct Value;
    using Array  = std::vector<Value>;
    using Object = std::unordered_map<std::string, Value>;

    struct Value : std::variant<std::nullptr_t, bool, double, std::string, Array, Object, std::shared_ptr<Stream>> {
        using Base = std::variant<std::nullptr_t, bool, double, std::string, Array, Object, std::shared_ptr<Stream>>;
        using Base::Base;

        bool isString() const { return std::holds_alternative<std::string>(*this); }
        const std::string& asString() const { return std::get<std::string>(*this); }
        static Value stream() { return Value(std::make_shared<Stream>()); }
    };

    struct CmdResult {
        int status = 0;
        std::shared_ptr<Stream> out, err;
    };
}