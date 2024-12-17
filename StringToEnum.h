#ifndef STRING_TO_ENUM_H
#define STRING_TO_ENUM_H

#include <iostream>
#include <unordered_map>

template<typename EnumType>
class EnumParser {
public:
    EnumParser(std::initializer_list<std::pair<std::string, EnumType>> list) {
        for (const auto& pair : list) {
            stringToEnumMap[pair.first] = pair.second;
        }
    }

    EnumType stringToEnum(const std::string& str) const {
        auto it = stringToEnumMap.find(str);
        if (it != stringToEnumMap.end()) {
            return it->second;
        } else {
            throw std::invalid_argument("Invalid string for enum conversion");
        }
    }

private:
    std::unordered_map<std::string, EnumType> stringToEnumMap;
};

#endif // STRING_TO_ENUM
