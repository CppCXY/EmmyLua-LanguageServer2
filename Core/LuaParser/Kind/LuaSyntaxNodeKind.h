#pragma once

#include <fmt/format.h>
#include <string_view>

enum class LuaSyntaxNodeKind {
    None = 0,

    File,

    Body,

    EmptyStatement,

    LocalStatement,

    LocalFunctionStatement,

    IfStatement,

    WhileStatement,

    DoStatement,

    ForStatement,

    RepeatStatement,

    FunctionStatement,

    LabelStatement,

    BreakStatement,

    ReturnStatement,

    GotoStatement,

    ExpressionStatement,

    AssignStatement,

    SuffixedExpression,

    ParExpression,

    LiteralExpression,

    StringLiteralExpression,

    ClosureExpression,

    UnaryExpression,

    BinaryExpression,

    TableExpression,

    CallExpression,

    IndexExpression,

    NameExpression,

    FunctionNameExpression,

    VarList,

    TableFieldList,

    TableField,

    TableFieldSep,

    FunctionBody,

    ParamList,

    NameDefList,

    Attribute,

    ExpressionList,

    ForNumber,

    ForList,

    ForBody,

    Error,

    Comment,

    ShortComment,

    LongComment,

    ShebangComment,

    // emmylua doc
    DocClass,

    DocInterface,

    DocAlias,

    DocEnum,

    DocType,

    DocField,

    DocParam,

    DocGeneric,

    DocOverload,

    DocSee,

    DocOverride,

    DocPublic,

    DocPrivate,

    DocProtected,

    DocDiagnostic,

    DocSince,

    DocVersion,

    DocDeprecated,

    GenericDefList,

    TypeList,

    Type,

    TableType,

    TableFieldType,

    ParType,

    IdType,

    StringType,

    FunctionType,

    FunctionTypeParamTypeList,

    FunctionParam,

    UnionType,

    ArrayType,

    GenericType,

    GenericParamTypeList,

    GenericDef,

    FieldIndex,

    EnumField,

    NormalComment
};

namespace detail::debug {
constexpr std::string_view GetSyntaxKindDebugName(LuaSyntaxNodeKind kind) {
    switch (kind) {
        case LuaSyntaxNodeKind::None:
            return "None";
        case LuaSyntaxNodeKind::File:
            return "File";
        case LuaSyntaxNodeKind::Body:
            return "Body";
        case LuaSyntaxNodeKind::EmptyStatement:
            return "EmptyStatement";
        case LuaSyntaxNodeKind::LocalStatement:
            return "LocalStatement";
        case LuaSyntaxNodeKind::LocalFunctionStatement:
            return "localFunctionStatement";
        case LuaSyntaxNodeKind::IfStatement:
            return "IfStatement";
        case LuaSyntaxNodeKind::WhileStatement:
            return "WhileStatement";
        case LuaSyntaxNodeKind::DoStatement:
            return "DoStatement";
        case LuaSyntaxNodeKind::ForStatement:
            return "FormatStatement";
        case LuaSyntaxNodeKind::RepeatStatement:
            return "RepeatStatement";
        case LuaSyntaxNodeKind::FunctionStatement:
            return "FunctionStatement";
        case LuaSyntaxNodeKind::LabelStatement:
            return "LabelStatement";
        case LuaSyntaxNodeKind::BreakStatement:
            return "BreakStatement";
        case LuaSyntaxNodeKind::ReturnStatement:
            return "ReturnStatement";
        case LuaSyntaxNodeKind::GotoStatement:
            return "GotoStatement";
        case LuaSyntaxNodeKind::ExpressionStatement:
            return "ExpressionStatement";
        case LuaSyntaxNodeKind::AssignStatement:
            return "AssignStatement";
        case LuaSyntaxNodeKind::SuffixedExpression:
            return "SuffixedExpression";
        case LuaSyntaxNodeKind::ParExpression:
            return "ParExpression";
        case LuaSyntaxNodeKind::LiteralExpression:
            return "LiteralExpression";
        case LuaSyntaxNodeKind::StringLiteralExpression:
            return "StringLiteralExpression";
        case LuaSyntaxNodeKind::ClosureExpression:
            return "ClosureExpression";
        case LuaSyntaxNodeKind::UnaryExpression:
            return "UnaryExpression";
        case LuaSyntaxNodeKind::BinaryExpression:
            return "BinaryExpression";
        case LuaSyntaxNodeKind::TableExpression:
            return "TableExpression";
        case LuaSyntaxNodeKind::CallExpression:
            return "CallExpression";
        case LuaSyntaxNodeKind::IndexExpression:
            return "IndexExpression";
        case LuaSyntaxNodeKind::NameExpression:
            return "NameExpression";
        case LuaSyntaxNodeKind::FunctionNameExpression:
            return "FunctionNameExpression";
        case LuaSyntaxNodeKind::VarList:
            return "VarList";
        case LuaSyntaxNodeKind::TableFieldList:
            return "TableFieldList";
        case LuaSyntaxNodeKind::TableField:
            return "TableField";
        case LuaSyntaxNodeKind::FunctionBody:
            return "FunctionBody";
        case LuaSyntaxNodeKind::ParamList:
            return "ParamList";
        case LuaSyntaxNodeKind::NameDefList:
            return "NameDefList";
        case LuaSyntaxNodeKind::Attribute:
            return "Attribute";
        case LuaSyntaxNodeKind::ExpressionList:
            return "ExpressionList";
        case LuaSyntaxNodeKind::ForNumber:
            return "ForNumber";
        case LuaSyntaxNodeKind::ForList:
            return "ForList";
        case LuaSyntaxNodeKind::ForBody:
            return "ForBody";
        case LuaSyntaxNodeKind::Error:
            return "Error";
        case LuaSyntaxNodeKind::Comment:
            return "Comment";
        case LuaSyntaxNodeKind::ShortComment:
            return "ShortComment";
        case LuaSyntaxNodeKind::LongComment:
            return "LongComment";
        case LuaSyntaxNodeKind::ShebangComment:
            return "ShebangComment";
        case LuaSyntaxNodeKind::TableFieldSep:
            return "TableFieldSep";
        case LuaSyntaxNodeKind::DocClass:
            return "DcoClass";
        case LuaSyntaxNodeKind::DocInterface:
            return "DocInterface";
        case LuaSyntaxNodeKind::DocAlias:
            return "DocAlias";
        case LuaSyntaxNodeKind::DocEnum:
            return "DocEnum";
        case LuaSyntaxNodeKind::DocType:
            return "DocType";
        case LuaSyntaxNodeKind::DocField:
            return "DocField";
        case LuaSyntaxNodeKind::DocParam:
            return "DocParam";
        case LuaSyntaxNodeKind::DocGeneric:
            return "DocGeneric";
        case LuaSyntaxNodeKind::DocOverload:
            return "DocOverload";
        case LuaSyntaxNodeKind::DocSee:
            return "DocSee";
        case LuaSyntaxNodeKind::DocOverride:
            return "DocOverride";
        case LuaSyntaxNodeKind::DocPublic:
            return "DocPublic";
        case LuaSyntaxNodeKind::DocPrivate:
            return "DocPrivate";
        case LuaSyntaxNodeKind::DocProtected:
            return "DocProtected";
        case LuaSyntaxNodeKind::DocDiagnostic:
            return "DocDiagnostic";
        case LuaSyntaxNodeKind::DocSince:
            return "DocSince";
        case LuaSyntaxNodeKind::DocVersion:
            return "DocVersion";
        case LuaSyntaxNodeKind::DocDeprecated:
            return "DocDeprecated";
        case LuaSyntaxNodeKind::GenericDefList:
            return "GenericDefList";
        case LuaSyntaxNodeKind::TypeList:
            return "TypeList";
        case LuaSyntaxNodeKind::Type:
            return "Type";
        case LuaSyntaxNodeKind::TableType:
            return "TableType";
        case LuaSyntaxNodeKind::TableFieldType:
            return "TableFieldType";
        case LuaSyntaxNodeKind::ParType:
            return "ParType";
        case LuaSyntaxNodeKind::IdType:
            return "IdType";
        case LuaSyntaxNodeKind::StringType:
            return "StringType";
        case LuaSyntaxNodeKind::FunctionType:
            return "FunctionType";
        case LuaSyntaxNodeKind::FunctionTypeParamTypeList:
            return "FunctionTypeParamTypeList";
        case LuaSyntaxNodeKind::FunctionParam:
            return "FunctionParam";
        case LuaSyntaxNodeKind::UnionType:
            return "UnionType";
        case LuaSyntaxNodeKind::ArrayType:
            return "ArrayType";
        case LuaSyntaxNodeKind::GenericType:
            return "GenericType";
        case LuaSyntaxNodeKind::GenericParamTypeList:
            return "GenericParamTypeList";
        case LuaSyntaxNodeKind::GenericDef:
            return "GenericDef";
        case LuaSyntaxNodeKind::FieldIndex:
            return "FieldIndex";
        case LuaSyntaxNodeKind::EnumField:
            return "EnumField";
        case LuaSyntaxNodeKind::NormalComment:
            return "NormalComment";
    }
    return "Unknown";
}
}

template<>
struct fmt::formatter<LuaSyntaxNodeKind> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
        // Parse the presentation format and store it in the formatter:
        auto it = ctx.begin(), end = ctx.end();

        // Check if reached the end of the range:
        while (it != end && *it != '}') { it++; }

        // Return an iterator past the end of the parsed range:
        return it;
    }

    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template<typename FormatContext>
    auto format(const LuaSyntaxNodeKind &p, FormatContext &ctx) const -> decltype(ctx.out()) {
        // ctx.out() is an output iterator to write to.
        return fmt::format_to(ctx.out(), "{}", ::detail::debug::GetSyntaxKindDebugName(p));
    }
};