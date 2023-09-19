/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

#ifndef _JSON_H_
#define _JSON_H_

#include <iostream>
#include <cstdint>
#include <string>
#include <list>
#include <map>
#include <initializer_list>
#include <type_traits>

namespace json {

enum class Type {
    Null,
    Object,
    Array,
    String,
    Double,
    Integer,
    Boolean
}; /* End of enum Type */

class Error : public std::runtime_error
{
public:
    Error(const std::string& message) : runtime_error(message)
    {}
}; /* End of class Error */

class Key
{
public:
    std::string key;
    Key(const char *str, std::size_t len) :
        key(std::string(str, len))
    {}
};

Key operator "" _(const char *str, std::size_t len)
{
    return Key(str, len);
}

//using json::operator "" _key;

class Object
{
public:
    class Parser
    {
    public:
        Parser()
        {}

        Object fromString(const std::string& input)
        {
            input_ = input;
            index_ = 0;
            return parseValue();
        }

    private:
        std::string input_;
        size_t index_;

        char peek() const
        {
            return (index_ < input_.size()) ? input_[index_] : '\0';
        }

        char get()
        {
            return (index_ < input_.size()) ? input_[index_++] : '\0';
        }

        void eatWhitespace()
        {
            while (std::isspace(peek())) { get(); }
        }

        Object parseValue()
        {
            eatWhitespace();
            char c = peek();
            if (c == '{') { return parseObject(); }
            if (c == '[') { return parseArray(); }
            if (c == '"') { return parseString(); }
            if (c == 't' || c == 'f') { return parseBoolean(); }
            if (c == 'n') { return parseNull(); }
            if (c == '-' || std::isdigit(c)) { return parseNumber(); }
            throw Error("Invalid JSON");
        }

        Object parseObject()
        {
            Object result(Type::Object);
            get(); // consume '{'
            for (;;)
            {
                eatWhitespace();
                if (peek() == '}')
                {
                    get(); // consume '}'
                    break;
                }
                std::string key = parseString().toString();
                eatWhitespace();
                if (get() != ':')
                {
                    throw Error("Invalid JSON");
                }
                Object value = parseValue();
                result[key] = value;
                eatWhitespace();
                if (peek() == ',')
                {
                    get(); // consume ','
                }
                else if (peek() == '}')
                {
                    get(); // consume '}'
                    break;
                }
                else
                {
                    throw Error("Invalid JSON");
                }
            }
            return result;
        }

        Object parseArray()
        {
            Object result(Type::Array);
            get(); // consume '['
            for (;;)
            {
                eatWhitespace();
                if (peek() == ']')
                {
                    get(); // consume ']'
                    break;
                }
                Object value = parseValue();
                result.append(value);
                eatWhitespace();
                if (peek() == ',')
                {
                    get(); // consume ','
                }
                else if (peek() == ']')
                {
                    get(); // consume ']'
                    break;
                }
                else
                {
                    throw Error("Invalid JSON");
                }
            }
            return result;
        }

        Object parseString()
        {
            std::string value;
            get(); // consume '"'
            for (;;)
            {
                char c = get();
                if (c == '"') { break; }
                value += c;
            }
            return Object(value);
        }

        Object parseNumber()
        {
            bool is_float = false;
            std::string value;
            while (std::isdigit(peek()) || peek() == '.' || peek() == '-' || std::tolower(peek()) == 'e')
            {
                char c = get();
                if (c == '.' || std::tolower(peek()) == 'e') { is_float = true; }
                value += c;
            }

            if (is_float)
            {
                return Object(std::stod(value));
            }

            return Object(std::stol(value));
        }

        Object parseBoolean()
        {
            if (peek() == 't')
            {
                if (input_.substr(index_, 4) == "true")
                {
                    index_ += 4;
                    return Object(true);
                }
                throw Error("Invalid JSON");
            }
            if (peek() == 'f')
            {
                if (input_.substr(index_, 5) == "false")
                {
                    index_ += 5;
                    return Object(false);
                }
                throw Error("Invalid JSON");
            }
            throw Error("Invalid JSON");
        }

        Object parseNull()
        {
            if (input_.substr(index_, 4) == "null")
            {
                index_ += 4;
                return Object(Type::Null);
            }
            throw Error("Invalid JSON");
        }
    }; /* End of subclass Parser */

    Object(Type type = Type::Null) :
        type_(type)
    {}

    Object(const Object& object)  = default;

    Object(std::initializer_list<std::pair<Key, Object>> attributes) :
            type_(Type::Object)
    {
        for (const auto& [key, value]: attributes)
        {
            map_[key.key] = value;
        }
    }

    Object(std::initializer_list<Object> elements) :
        type_(Type::Array)
    {
        vector_.insert(vector_.end(), elements.begin(), elements.end());
    }

    template <typename T>
    Object(T value, typename std::enable_if<std::is_same<T, bool>::value>::type* = 0) :
        type_(Type::Boolean), boolean_(value)
    {}

    template <typename T>
    Object(T value, typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value>::type* = 0) :
        type_(Type::Integer), integer_(value)
    {}

    template <typename T>
    Object(T value, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0) :
        type_(Type::Double), double_(value)
    {}

    template <typename T>
    Object(T value, typename std::enable_if<std::is_convertible<T, std::string>::value>::type* = 0) :
        type_(Type::String), string_(value)
    {}

    Object(std::nullptr_t) :
        type_(Type::Null )
    {}

    bool isNull() const
    {
        return type_ == Type::Null;
    }

    bool isEmpty() const
    {
        if (isNull()) { return true; }
        if (type_ == Type::Object) { return map_.size() == 0; }
        if (type_ == Type::Array) { return vector_.size() == 0; }
        return false;
    }

    std::size_t size() const
    {
        if (type_ == Type::Object)
        {
            return map_.size();
        }
        if (type_ == Type::Array)
        {
            return vector_.size();
        }
        throw Error("Invalid type");
    }

    Type type() const
    {
        return type_;
    }

    bool toBoolean() const
    {
        if (type_ != Type::Boolean) { throw Error("Invalid type"); }
        return boolean_;
    }

    std::int64_t toInteger() const
    {
        if (type_ != Type::Integer) { throw Error("Invalid type"); }
        return integer_;
    }

    double toDouble() const
    {
        if (type_ != Type::Double) { throw Error("Invalid type"); }
        return double_;
    }

    std::string toString() const
    {
        if (type_ != Type::String) { throw Error("Invalid type"); }
        return string_;
    }

    void clear()
    {
        type_ = Type::Null;
        string_.clear();
        vector_.clear();
        map_.clear();
    }

    template <typename T>
    typename std::enable_if<std::is_same<T, bool>::value, Object&>::type operator=(T value)
    {
        setType(Type::Boolean);
        boolean_ = value;
        return *this;
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value, Object&>::type operator=(T value)
    {
        setType(Type::Integer);
        integer_ = value;
        return *this;
    }

    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, Object&>::type operator=(T value)
    {
        setType(Type::Double);
        double_ = value;
        return *this;
    }

    template <typename T>
    typename std::enable_if<std::is_convertible<T, std::string>::value, Object&>::type operator=(T value)
    {
        setType(Type::String);
        string_ = std::string(value);
        return *this;
    }

    Object& operator[](const std::string& key)
    {
        if (type_ != Type::Object)
        {
            setType(Type::Object);
        }
        return map_[key];
    }

    Object& operator[](std::uint32_t index)
    {
        if (type_ != Type::Array)
        {
            setType(Type::Array);
        }
        if (index >= vector_.size())
        {
            vector_.resize(index + 1);
        }
        return vector_[index];
    }

    void append(const Object value)
    {
        if (type_ != Type::Array)
        {
            setType(Type::Array);
        }
        vector_.push_back(value);
    }

    bool contains(const std::string& key) const
    {
        if (type_ != Type::Object) { throw Error("invalid type"); }
        return map_.find(key) != map_.end();
    }

    void loads(const std::string data)
    {
        if (data.empty())
        {
            clear();
            return;
        }
        *this = Object::Parser().fromString(data);
    }

    const std::string dumps()
    {
        return "";
    }

private:
    Type type_;
    std::int64_t integer_;
    double double_;
    bool boolean_;
    std::string string_;
    std::vector<Object> vector_;
    std::map<std::string, Object> map_;

    void setType(Type type)
    {
        clear();
        type_ = type;
    }
}; /* End of class Object */

inline static Object parse(const std::string& data)
{
    if (data.empty())
    {
        return Object();
    }
    return Object::Parser().fromString(data);
}

} /* End of namespace json */

#endif /* !_JSON_H_ */
