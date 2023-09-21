/**
 * Copyright (c) 2023 by Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

#ifndef _JSON_H_
#define _JSON_H_

#include <sstream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <list>
#include <map>
#include <initializer_list>
#include <type_traits>

namespace json {

enum class Type : std::uint16_t
{
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
    Key(const std::string& str) :
        str_(str)
    {}

    Key(const char *str, std::size_t len) :
        Key(std::string(str, len))
    {}

    const std::string& str() const
    {
        return str_;
    }
private:
    std::string str_;
}; /* End of class Key */

Key operator "" _(const char *str, std::size_t len)
{
    return Key(str, len);
}

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

        [[nodiscard]] char peek() const
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
            return value;
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
                return std::stod(value);
            }
            return std::stol(value);
        }

        Object parseBoolean()
        {
            if (peek() == 't')
            {
                if (input_.substr(index_, 4) == "true")
                {
                    index_ += 4;
                    return true;
                }
                throw Error("Invalid JSON");
            }
            if (peek() == 'f')
            {
                if (input_.substr(index_, 5) == "false")
                {
                    index_ += 5;
                    return false;
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
                return nullptr;
            }
            throw Error("Invalid JSON");
        }
    }; /* End of subclass Parser */

    explicit Object(Type type = Type::Null) :
        type_(type)
    {}

    Object(std::initializer_list<std::pair<Key, Object>> attributes) :
            type_(Type::Object)
    {
        for (const auto& [key, value]: attributes)
        {
            map_[key.str()] = value;
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

    [[nodiscard]] bool isNull() const
    {
        return type_ == Type::Null;
    }

    [[nodiscard]] bool isEmpty() const
    {
        if (isNull()) { return true; }
        if (type_ == Type::Object) { return map_.size() == 0; }
        if (type_ == Type::Array) { return vector_.size() == 0; }
        return false;
    }

    [[nodiscard]] std::size_t size() const
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

    [[nodiscard]] Type type() const
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

    void append(const Object& value)
    {
        if (type_ != Type::Array)
        {
            setType(Type::Array);
        }
        vector_.push_back(value);
    }

    bool contains(const std::string& key) const
    {
        if (type_ != Type::Object) { throw Error("Invalid Type"); }
        return map_.find(key) != map_.end();
    }

    void loads(const std::string& data)
    {
        if (data.empty())
        {
            clear();
            return;
        }
        *this = Object::Parser().fromString(data);
    }

    std::string dumps(bool pretty = true, std::string ident = "") const
    {
        if (type_ == Type::Null)
        {
            return "null";
        }
        if (type_ == Type::Boolean) { return boolean_ ? "true" : "false"; }
        if (type_ == Type::Integer) { return std::to_string(integer_); }
        if (type_ == Type::Double)
        {
            std::ostringstream oss;
            oss << std::setprecision(8) << std::noshowpoint << double_;
            return oss.str();
        }
        if (type_ == Type::String) { return std::string("\"") + string_ + std::string("\""); }
        if (type_ == Type::Array)
        {
            std::ostringstream oss;
            oss << "[";
            bool first_element = true;
            for (auto& object: vector_)
            {
                if (first_element) { first_element = false; }
                else
                {
                    oss << ",";
                    if (pretty) { oss << " "; }
                }
                oss << object.dumps(pretty, ident);
            }
            oss << "]";
            return oss.str();
        }
        if (type_ == Type::Object)
        {
            std::ostringstream oss;
            oss << "{";
            if (pretty) { oss << "\n"; }
            auto new_ident = ident + "  ";
            bool first_element = true;
            for (auto& [key, object]: map_)
            {
                if (first_element) { first_element = false; }
                else
                {
                    oss << ",";
                    if (pretty) { oss << "\n"; }
                }
                if (pretty) { oss << new_ident; }
                oss << "\"" << key << "\":";
                if (pretty) { oss << " "; }
                oss << object.dumps(pretty, new_ident);
            }
            if (pretty) { oss << "\n" << ident; }
            oss <<  "}";
            return oss.str();
        }
        throw Error("Invalid Type");
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

    bool operator==(const Object& rhs) const
    {
        if (type_ != rhs.type_)
        {
            return false;
        }

        switch (type_) {
            case Type::Boolean: return boolean_ == rhs.boolean_;
            case Type::Integer: return integer_ == rhs.integer_;
            case Type::Double: return double_ == rhs.double_;
            case Type::String: return string_ == rhs.string_;
            case Type::Array: return vector_ == rhs.vector_;
            case Type::Object: return map_ == rhs.map_;
            case Type::Null: return true;
            default: return false;
        }
    }

    bool operator!=(const Object& rhs) const
    {
        return !(*this == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const Object& object)
    {
        os << object.dumps();
        return os;
    }

private:
    Type type_;
    std::int64_t integer_{};
    double double_{};
    bool boolean_{};
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

using json::operator "" _;

#endif /* !_JSON_H_ */
