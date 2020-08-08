#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <memory>
#include <string>

#include "ast.hpp"

namespace lox
{

    class Env;
    class Interpreter;

    enum class ObjectType
    {
        BoolType,
        NumType,
        NilType,
        StrType,
        FuncType,
    };

    class Object
    {
    public:
        ObjectType type;

        Object(ObjectType type_) : type(type_) {}

        virtual bool isTrue() const { return true; }

        virtual bool equals(Object *other) const = 0;

        virtual std::unique_ptr<Object> clone() const = 0;

        virtual std::string toString() const = 0;
    };

    class NumObj : public Object
    {
    public:
        double value;

        NumObj(double value_) : Object(ObjectType::NumType), value(value_) {}

        bool equals(Object *other) const override
        {
            if (other->type != ObjectType::NumType)
                return false;
            return value == static_cast<NumObj *>(other)->value;
        }

        std::unique_ptr<Object> clone() const override
        {
            return std::unique_ptr<NumObj>(new NumObj(value));
        }

        std::string toString() const override
        {
            return std::string(std::to_string(value));
        }
    };

    class StrObj : public Object
    {
    public:
        std::string value;

        StrObj(const std::string &value_) : Object(ObjectType::StrType), value(value_) {}

        bool equals(Object *other) const override
        {
            if (other->type != ObjectType::StrType)
                return false;
            return value == static_cast<StrObj *>(other)->value;
        }

        std::unique_ptr<Object> clone() const override
        {
            return std::unique_ptr<StrObj>(new StrObj(value));
        }

        std::string toString() const override
        {
            return value;
        }
    };

    class BoolObj : public Object
    {
    public:
        bool value;

        BoolObj(bool value_) : Object(ObjectType::BoolType), value(value_) {}

        bool isTrue() const override { return value; }

        bool equals(Object *other) const override
        {
            if (other->type != ObjectType::BoolType)
                return false;
            return value == static_cast<BoolObj *>(other)->value;
        }

        std::unique_ptr<Object> clone() const override
        {
            return std::unique_ptr<BoolObj>(new BoolObj(value));
        }

        std::string toString() const override
        {
            return std::to_string(value);
        }
    };

    class NilObj : public Object
    {
    public:
        NilObj() : Object(ObjectType::NilType) {}

        bool isTrue() const override { return false; }

        bool equals(Object *other) const override { return other->type == ObjectType::NilType; }

        std::unique_ptr<Object> clone() const override
        {
            return std::unique_ptr<NilObj>();
        }

        std::string toString() const override
        {
            return "Nil";
        }
    };

    class FuncObj : public Object
    {
    public:
        FuncStmt *declaration;

        std::shared_ptr<Env> closure;

        FuncObj(FuncStmt *declare_, std::shared_ptr<Env> closure_) : Object(ObjectType::FuncType),
                                                                     declaration(declare_),
                                                                     closure(closure_) {}

        bool isTrue() const override { return false; }

        bool equals(Object *other) const override { return other->type == ObjectType::FuncType; }

        std::unique_ptr<Object> clone() const override
        {
            return std::unique_ptr<FuncObj>(new FuncObj(declaration, closure));
        }

        std::string toString() const override
        {
            return "<fn " + declaration->name->lexeme + ">";
        }

        size_t arity()
        {
            return declaration->params.size();
        };
    };

} // namespace lox

#endif