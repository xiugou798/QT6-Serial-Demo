// EnumToString.h
#ifndef ENUM_TO_STRING_H
#define ENUM_TO_STRING_H


#include <string>
#include <unordered_map>
#include <type_traits>

// toString函数模板的正向声明
template <typename Enum>
std::string toString(Enum e);

// 将枚举映射到字符串的实用工具模板
template <typename Enum>
struct EnumToString
{
    static const std::unordered_map<Enum, std::string>& getEnumToStringMap()
    {
        static const std::unordered_map<Enum, std::string> enumToStringMap;
        return enumToStringMap;
    }
};

// 宏来帮助定义类作用域中每个枚举的映射
#define DEFINE_ENUM_TO_STRING(EnumType, ...)                         \
template <>                                                      \
    struct EnumToString<EnumType>                                    \
{                                                                \
        static const std::unordered_map<EnumType, std::string>& getEnumToStringMap() \
    {                                                            \
            static const std::unordered_map<EnumType, std::string> enumToStringMap = __VA_ARGS__; \
            return enumToStringMap;                                  \
    }                                                            \
};

//  toString函数模板的实现
template <typename Enum>
std::string toString(Enum e)
{
    static_assert(std::is_enum<Enum>::value, "Template parameter is not an enum type");
    const auto& map = EnumToString<Enum>::getEnumToStringMap();
    auto it = map.find(e);
    if (it != map.end())
    {
        return it->second;
    }
    return "Unknown";
}

#endif // ENUMTOSTRING_H
