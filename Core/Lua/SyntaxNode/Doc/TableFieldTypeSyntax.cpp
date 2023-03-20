#include "TableFieldTypeSyntax.h"
TableFieldTypeSyntax::TableFieldTypeSyntax(LuaNodeOrToken n) : DocBaseSyntax(n) {

}
std::string_view TableFieldTypeSyntax::GetStringKey(const LuaSyntaxTree &t) {
    return "";
}

int64_t TableFieldTypeSyntax::GetIndexKey(const LuaSyntaxTree& t) {
    return 0;
}
