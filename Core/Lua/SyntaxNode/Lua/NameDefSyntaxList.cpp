#include "NameDefSyntaxList.h"

NameDefSyntaxList::NameDefSyntaxList(LuaNodeOrToken n) : LuaBaseSyntax(n) {
}

std::vector<NameDefSyntax> NameDefSyntaxList::GetList(const LuaSyntaxTree &t) const {
    return GetMembers<NameDefSyntax>(t);
}

std::vector<std::string_view> NameDefSyntaxList::GetNames(const LuaSyntaxTree &t) const {
    std::vector<std::string_view> result;
    for(auto m: GetList(t)){
        result.push_back(m.GetName(t));
    }
    return result;
}
