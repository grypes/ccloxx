#ifndef ENV_HPP
#define ENV_HPP

#include <unordered_map>

namespace lox
{

    class Object;

    class Env
    {
        std::shared_ptr<Env> enclosing;

    public:
        std::unordered_map<std::string, std::unique_ptr<Object>> values;
        
        Env() : enclosing(nullptr) {}

        Env(std::shared_ptr<Env> enclosing_) : enclosing(enclosing_) {}

        void define(const std::string &name, std::unique_ptr<Object> value)
        {
            values[name] = std::move(value);
        }

        void assign(const std::string &name, std::unique_ptr<Object> value)
        {
            auto it = values.find(name);
            if (it != values.end())
            {
                values[name] = std::move(value);
                return;
            }

            if (enclosing != nullptr)
            {
                enclosing->assign(name, std::move(value));
                return;
            }
        }

        Object *get(const std::string &name) const
        {
            auto it = values.find(name);
            if (it != values.end())
                return it->second.get();

            if (enclosing != nullptr)
                return enclosing->get(name);

            return nullptr;
        }
    };

} // namespace lox

#endif