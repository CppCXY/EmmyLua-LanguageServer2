#pragma once

#include <memory>
#include <vector>

#include "LuaParser/Define/LuaOperatorType.h"
#include "LuaParser/Define/LuaSyntaxError.h"
#include "LuaParser/Kind/LuaSyntaxNodeKind.h"
#include "LuaParser/Kind/LuaTokenKind.h"
#include "LuaParser/ParseState/ParseState.h"

class LuaSource;

class LuaParser {
public:
    LuaParser(const LuaSource *file, std::vector<LuaToken> &&tokens);

    bool Parse();

    std::vector<LuaToken> &GetTokens();

    ParseState &GetParseState();

    std::vector<LuaSyntaxError> &GetErrors();

    static UnOpr GetUnaryOperator(LuaTokenKind op);

    static BinOpr GetBinaryOperator(LuaTokenKind op);

private:
    void Next();

    LuaTokenKind LookAhead();

    LuaTokenKind Current();

    void SkipTrivia();

    bool BlockFollow(bool rightbrace = false);

    void StatementList();

    CompleteMarker Statement();

    CompleteMarker ErrorStatement();

    CompleteMarker IfStatement();

    CompleteMarker WhileStatement();

    CompleteMarker DoStatement();

    CompleteMarker ForStatement();

    CompleteMarker RepeatStatement();

    CompleteMarker FunctionStatement();

    CompleteMarker LocalFunctionStatement();

    CompleteMarker LocalStatement();

    CompleteMarker LabelStatement();

    CompleteMarker ReturnStatement();

    CompleteMarker BreakStatement();

    CompleteMarker GotoStatement();

    CompleteMarker OtherStatement();

    CompleteMarker ForNumber();

    CompleteMarker ForList();

    CompleteMarker ForBody();

    void TestThenBlock();

    CompleteMarker NameDefList(bool supportAttribute);

    CompleteMarker NameDef(bool supportAttribute);

    CompleteMarker Body();

    CompleteMarker ExpressionList(LuaTokenKind stopToken = LuaTokenKind::TK_UNKNOWN);

    CompleteMarker Expression();

    CompleteMarker Subexpression(int limit);

    CompleteMarker SimpleExpression();

    CompleteMarker TableConstructor();

    CompleteMarker FieldList();

    CompleteMarker Field();

    void ListField();

    void RectField();

    CompleteMarker FunctionBody();

    CompleteMarker ParamList();

    CompleteMarker SuffixedExpression();

    CompleteMarker CallExpression();

    CompleteMarker FieldSel();

    CompleteMarker YIndex();

    void FunctionName();

    void CheckName();

    CompleteMarker LocalAttribute();

    void Check(LuaTokenKind c);

    CompleteMarker PrimaryExpression();

    /*
	 * 他是检查当前token的type是否与c相同
	 * 如果是就跳过当前,
	 * 否则会生成错误
	 */
    void CheckAndNext(LuaTokenKind kind);

    /*
	 * 他是检查当前token的type是否与c相同
	 * 如果是就跳过当前，并返回true
	 * 否则返回false
	 */
    bool TestAndNext(LuaTokenKind kind);

    void LuaExpectedError(std::string_view message);

    void LuaError(std::string_view message);

    const LuaSource *_source;
    ParseState _p;
    std::vector<LuaToken> _tokens;
    std::size_t _tokenIndex;
    bool _invalid;
    LuaTokenKind _current;
    std::vector<LuaSyntaxError> _errors;
};
