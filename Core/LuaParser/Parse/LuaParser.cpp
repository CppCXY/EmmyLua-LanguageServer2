#include "LuaParser.h"
#include "LuaOperatorType.h"
#include "LuaParser/File/LuaFile.h"
#include "LuaParser/Lexer/LuaDefine.h"
#include "LuaParser/Lexer/LuaTokenTypeDetail.h"
#include "LuaParser/exception/LuaParseException.h"
#include <fmt/format.h>

LuaParser::LuaParser(std::shared_ptr<LuaFile> luaFile, std::vector<LuaToken> &&tokens)
    : _tokens(tokens),
      _tokenIndex(0),
      _file(luaFile),
      _events(),
      _invalid(true),
      _current(TK_EOF) {
}

std::vector<MarkEvent> &LuaParser::GetEvents() {
    return _events;
}

std::vector<LuaParseError> &LuaParser::GetErrors() {
    return _errors;
}

bool LuaParser::HasError() const {
    return !_errors.empty();
}

std::shared_ptr<LuaFile> LuaParser::GetLuaFile() {
    return _file;
}

bool LuaParser::Parse() {
    try {
        Body();
    } catch (LuaParseException &e) {
        auto text = _file->GetSource();
        _errors.emplace_back(e.what(), TextRange(text.size(), text.size()));
    }

    if (_tokenIndex < _tokens.size()) {
        _errors.emplace_back("parsing did not complete",
                             _tokens[_tokenIndex].Range);
    }

    return true;
}

Marker LuaParser::Mark() {
    auto pos = _events.size();
    _events.emplace_back();
    return Marker(pos);
}

void LuaParser::Next() {
    auto tk = Current();
    auto me = MarkEvent(MarkEventType::EatToken);
    me.U.Token.Kind = tk;
    me.U.Token.Index = _tokenIndex;
    _events.push_back(me);
    _tokenIndex++;
    _invalid = true;
}

LuaTokenKind LuaParser::LookAhead() {
    std::size_t nextIndex = _tokenIndex + 1;

    while (nextIndex < _tokens.size()) {
        auto tk = _tokens[nextIndex].TokenType;
        switch (tk) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                nextIndex++;
                break;
            }
            default: {
                return tk;
            }
        }
    }

    return TK_EOF;
}

LuaTokenKind LuaParser::Current() {
    if (_invalid) {
        _invalid = false;
        SkipComment();
        if (_tokenIndex < _tokens.size()) {
            _current = _tokens[_tokenIndex].TokenType;
        } else {
            _current = TK_EOF;
        }
    }

    if (_errors.size() > 100) {
        std::string_view error = "too many errors, parse fail";
        throw LuaParseException(error);
    }

    return _current;
}

void LuaParser::SkipComment() {
    for (; _tokenIndex < _tokens.size(); _tokenIndex++) {
        switch (_tokens[_tokenIndex].TokenType) {
            case TK_SHORT_COMMENT:
            case TK_LONG_COMMENT:
            case TK_SHEBANG: {
                // ignore
                break;
            }
            default: {
                return;
            }
        }
    }
}

bool LuaParser::BlockFollow(bool rightbrace) {
    switch (Current()) {
        case TK_ELSE:
        case TK_ELSEIF:
        case TK_END:
        case TK_EOF:
        case TK_UNTIL:
            return true;
        case ')':
            return rightbrace;
        default:
            return false;
    }
}

void LuaParser::StatementList() {
    while (!BlockFollow(true)) {
        if (Statement().IsNone()) {
            ErrorStatement();
        }
    }
}

CompleteMarker LuaParser::Statement() {
    switch (Current()) {
        case ';': {
            auto m = Mark();
            Next();
            return m.Complete(*this, LuaSyntaxNodeKind::EmptyStatement);
        }
        case TK_IF: {
            return IfStatement();
        }
        case TK_WHILE: {
            return WhileStatement();
        }
        case TK_DO: {
            return DoStatement();
        }
        case TK_FOR: {
            return ForStatement();
        }
        case TK_REPEAT: {
            return RepeatStatement();
        }
        case TK_FUNCTION: {
            return FunctionStatement();
        }
        case TK_LOCAL: {
            if (LookAhead() == TK_FUNCTION) {
                return LocalFunctionStatement();
            } else {
                return LocalStatement();
            }
        }
        case TK_DBCOLON: {
            return LabelStatement();
        }
        case TK_RETURN: {
            return ReturnStatement();
        }
        case TK_BREAK: {
            return BreakStatement();
        }
        case TK_GOTO: {
            return GotoStatement();
        }
        default: {
            return ExpressionStatement();
        }
    }
}

CompleteMarker LuaParser::ErrorStatement() {
    auto m = Mark();

    LuaError("the statement could not be parsed");

    Next();

    //    while (Current() != EOZ) {
    //        Next();
    //    }

    return m.Complete(*this, LuaSyntaxNodeKind::Error);
}

/* ifstat -> IF cond THEN block {ELSEIF cond THEN block} [ELSE block] END */
CompleteMarker LuaParser::IfStatement() {
    auto m = Mark();

    TestThenBlock();
    while (Current() == TK_ELSEIF) {
        TestThenBlock();
    }
    if (TestAndNext(TK_ELSE)) {
        Body();
    }
    CheckAndNext(TK_END);

    return m.Complete(*this, LuaSyntaxNodeKind::IfStatement);
}

/* whilestat -> WHILE cond DO block END */
CompleteMarker LuaParser::WhileStatement() {
    auto m = Mark();

    CheckAndNext(TK_WHILE);

    Expression();

    CheckAndNext(TK_DO);

    Body();

    CheckAndNext(TK_END);

    return m.Complete(*this, LuaSyntaxNodeKind::WhileStatement);
}

CompleteMarker LuaParser::DoStatement() {
    auto m = Mark();

    CheckAndNext(TK_DO);

    Body();

    CheckAndNext(TK_END);

    return m.Complete(*this, LuaSyntaxNodeKind::DoStatement);
}

/* forstat -> FOR (fornum | forlist) END */
CompleteMarker LuaParser::ForStatement() {
    // forstatement 只有一个 for 的token 节点 加上 forNumber或者forList 节点
    auto m = Mark();

    CheckAndNext(TK_FOR);

    Check(TK_NAME);

    switch (LookAhead()) {
        case '=': {
            ForNumber();
            break;
        }
        case ',':
        case TK_IN: {
            ForList();
            break;
        }
        default: {
            LuaExpectedError("'=' or 'in' expected");
        }
    }

    return m.Complete(*this, LuaSyntaxNodeKind::ForStatement);
}

CompleteMarker LuaParser::ForNumber() {
    auto m = Mark();

    CheckName();

    CheckAndNext('=');

    Expression();

    CheckAndNext(',');

    Expression();

    if (TestAndNext(','))// optional step
    {
        Expression();
    }

    ForBody();

    return m.Complete(*this, LuaSyntaxNodeKind::ForNumber);
}

/* forlist -> NAME {,NAME} IN explist forbody */
CompleteMarker LuaParser::ForList() {
    auto m = Mark();

    NameDefList();

    CheckAndNext(TK_IN);

    ExpressionList();

    ForBody();

    return m.Complete(*this, LuaSyntaxNodeKind::ForList);
}

CompleteMarker LuaParser::ForBody() {
    auto m = Mark();

    CheckAndNext(TK_DO);

    Body();

    CheckAndNext(TK_END);

    return m.Complete(*this, LuaSyntaxNodeKind::ForBody);
}

/* repeatstat -> REPEAT block UNTIL cond */
CompleteMarker LuaParser::RepeatStatement() {
    auto m = Mark();

    CheckAndNext(TK_REPEAT);

    Body();

    CheckAndNext(TK_UNTIL);

    Expression();

    return m.Complete(*this, LuaSyntaxNodeKind::RepeatStatement);
}

CompleteMarker LuaParser::FunctionStatement() {
    auto m = Mark();

    CheckAndNext(TK_FUNCTION);

    FunctionName();

    FunctionBody();

    return m.Complete(*this, LuaSyntaxNodeKind::FunctionStatement);
}

CompleteMarker LuaParser::LocalFunctionStatement() {
    auto m = Mark();

    CheckAndNext(TK_LOCAL);

    CheckAndNext(TK_FUNCTION);

    CheckName();

    FunctionBody();

    return m.Complete(*this, LuaSyntaxNodeKind::LocalFunctionStatement);
}

/* stat -> LOCAL ATTRIB NAME {',' ATTRIB NAME} ['=' explist] */
CompleteMarker LuaParser::LocalStatement() {
    auto m = Mark();

    CheckAndNext(TK_LOCAL);

    auto nm = Mark();
    do {
        CheckName();
        LocalAttribute();
    } while (TestAndNext(','));
    nm.Complete(*this, LuaSyntaxNodeKind::NameDefList);

    if (TestAndNext('=')) {
        ExpressionList();
    }
    // 如果有一个分号则加入到localstatement
    TestAndNext(';');

    return m.Complete(*this, LuaSyntaxNodeKind::LocalStatement);
}

CompleteMarker LuaParser::LabelStatement() {
    auto m = Mark();

    CheckAndNext(TK_DBCOLON);

    CheckName();

    CheckAndNext(TK_DBCOLON);

    return m.Complete(*this, LuaSyntaxNodeKind::LabelStatement);
}

CompleteMarker LuaParser::ReturnStatement() {
    auto m = Mark();

    CheckAndNext(TK_RETURN);

    if (!(BlockFollow() || Current() == ';')) {
        ExpressionList();
    }

    TestAndNext(';');

    return m.Complete(*this, LuaSyntaxNodeKind::ReturnStatement);
}

CompleteMarker LuaParser::BreakStatement() {
    auto m = Mark();

    CheckAndNext(TK_BREAK);

    TestAndNext(';');

    return m.Complete(*this, LuaSyntaxNodeKind::BreakStatement);
}

CompleteMarker LuaParser::GotoStatement() {
    auto m = Mark();

    CheckAndNext(TK_GOTO);

    CheckName();

    TestAndNext(';');

    return m.Complete(*this, LuaSyntaxNodeKind::GotoStatement);
}

// exprStat -> call | assignment
// assignment -> varList '=' exprList
CompleteMarker LuaParser::ExpressionStatement() {
    auto m = Mark();
    auto cm = SuffixedExpression();
    if (cm.IsNone()) {
        return cm;
    }

    if (Current() == '=' || Current() == ',') {
        while (TestAndNext(',')) {
            SuffixedExpression();
        }
        cm = m.Complete(*this, LuaSyntaxNodeKind::VarList);
        m = cm.Precede(*this);

        CheckAndNext('=');

        ExpressionList();

        // 如果发现一个分号，会认为分号为该语句的结尾
        TestAndNext(';');
        return m.Complete(*this, LuaSyntaxNodeKind::AssignStatement);
    } else {
        TestAndNext(';');
        return m.Complete(*this, LuaSyntaxNodeKind::ExpressionStatement);
    }
}

/* test_then_block -> [IF | ELSEIF] cond THEN block */
void LuaParser::TestThenBlock() {
    if (Current() == TK_IF || Current() == TK_ELSEIF) {
        Next();
    }
    Expression();
    CheckAndNext(TK_THEN);
    Body();
}

CompleteMarker LuaParser::Body() {
    auto m = Mark();

    StatementList();

    return m.Complete(*this, LuaSyntaxNodeKind::Body);
}

/* explist -> expr { ',' expr } */
CompleteMarker LuaParser::ExpressionList(LuaTokenKind stopToken) {
    auto m = Mark();

    Expression();
    while (TestAndNext(',')) {
        if (Current() == stopToken) {
            break;
        }

        Expression();
    }

    return m.Complete(*this, LuaSyntaxNodeKind::ExpressionList);
}

CompleteMarker LuaParser::Expression() {
    return Subexpression(0);
}

/*
* subexpr -> (simpleexp | unop subexpr) { binop subexpr }
*/
CompleteMarker LuaParser::Subexpression(int limit) {
    CompleteMarker cm;
    UnOpr uop = GetUnaryOperator(Current());
    if (uop != UnOpr::OPR_NOUNOPR) /* prefix (unary) operator? */
    {
        auto m = Mark();
        Next();
        Subexpression(UNARY_PRIORITY);
        cm = m.Complete(*this, LuaSyntaxNodeKind::UnaryExpression);
    } else {
        cm = SimpleExpression();
    }

    auto op = GetBinaryOperator(Current());
    /* expand while operators have priorities higher than 'limit' */
    while (op != BinOpr::OPR_NOBINOPR && priority[static_cast<int>(op)].left > limit) {
        auto m = cm.Precede(*this);

        Next();

        Subexpression(priority[static_cast<int>(op)].right);

        cm = m.Complete(*this, LuaSyntaxNodeKind::BinaryExpression);
        // next op
        op = GetBinaryOperator(Current());
    }

    return cm;
}

/* simpleexp -> FLT | INT | STRING | NIL | TRUE | FALSE | ... |
				constructor | FUNCTION body | suffixedexp */
CompleteMarker LuaParser::SimpleExpression() {
    switch (Current()) {
        case TK_FLT:
        case TK_NUMBER:
        case TK_NIL:
        case TK_TRUE:
        case TK_FALSE:
        case TK_DOTS: {
            auto m = Mark();
            Next();
            return m.Complete(*this, LuaSyntaxNodeKind::LiteralExpression);
        }
        case TK_LONG_STRING:
        case TK_STRING: {
            auto m = Mark();
            Next();
            return m.Complete(*this, LuaSyntaxNodeKind::StringLiteralExpression);
        }
        case '{': {
            return TableConstructor();
        }
        case TK_FUNCTION: {
            auto m = Mark();

            Next();
            FunctionBody();

            return m.Complete(*this, LuaSyntaxNodeKind::ClosureExpression);
        }
        default: {
            return SuffixedExpression();
        }
    }
}

/* constructor -> '{' [ field { sep field } [sep] ] '}'
   sep -> ',' | ';'
*/
CompleteMarker LuaParser::TableConstructor() {
    auto m = Mark();
    CheckAndNext('{');

    FieldList();

    CheckAndNext('}');

    return m.Complete(*this, LuaSyntaxNodeKind::TableExpression);
}

CompleteMarker LuaParser::FieldList() {
    auto m = Mark();

    while (Current() != '}') {
        Field();
        if (Current() == TK_EOF) {
            break;
        }
    }

    return m.Complete(*this, LuaSyntaxNodeKind::TableFieldList);
}

/* field -> listfield | recfield */
CompleteMarker LuaParser::Field() {
    auto m = Mark();

    switch (Current()) {
        case TK_NAME: {
            if (LookAhead() != '=') {
                ListField();
            } else {
                RectField();
            }
            break;
        }
        case '[': {
            RectField();
            break;
        }
        default: {
            ListField();
            break;
        }
    }

    if (Current() == ',' || Current() == ';') {
        auto m2 = Mark();
        Next();
        m2.Complete(*this, LuaSyntaxNodeKind::TableFieldSep);
    }

    return m.Complete(*this, LuaSyntaxNodeKind::TableField);
}

void LuaParser::ListField() {
    Expression();
}

/* recfield -> (NAME | '['exp']') = exp */
void LuaParser::RectField() {
    if (Current() == TK_NAME) {
        Next();
    } else {
        YIndex();
    }

    CheckAndNext('=');

    Expression();
}

/* body ->  '(' parlist ')' block END */
CompleteMarker LuaParser::FunctionBody() {
    auto m = Mark();

    CheckAndNext('(');

    ParamList();

    CheckAndNext(')');

    Body();

    CheckAndNext(TK_END);

    return m.Complete(*this, LuaSyntaxNodeKind::FunctionBody);
}

CompleteMarker LuaParser::ParamList() {
    auto m = Mark();

    bool isVararg = false;
    if (Current() != ')') {
        do {
            switch (Current()) {
                case TK_NAME: {
                    Next();
                    break;
                }
                case TK_DOTS: {
                    isVararg = true;
                    Next();
                    break;
                }
                case ')': {
                    break;
                }
                default: {
                    LuaExpectedError("<name> or '...' expected");
                    goto endLoop;
                }
            }
        } while (!isVararg && TestAndNext(','));
    endLoop:
        // empty stat
        void(0);
    }

    return m.Complete(*this, LuaSyntaxNodeKind::ParamList);
}

/* suffixedexp ->
	 primaryexp { '.' NAME | '[' exp ']' | ':' NAME funcargs | funcargs }+ */
CompleteMarker LuaParser::SuffixedExpression() {
    auto m = Mark();
    auto cm = PrimaryExpression();
    bool suffix = false;
    for (;;) {
        switch (Current()) {
            case '.': {
                FieldSel();
                break;
            }
            case '[': {
                YIndex();
                break;
            }
            case ':': {
                FieldSel();
                CallExpression();
                break;
            }
            case '(':
            case TK_LONG_STRING:
            case TK_STRING:
            case '{': {
                CallExpression();
                break;
            }
            default:
                goto endLoop;
        }
        suffix = true;
    }
endLoop:
    if (suffix) {
        return m.Complete(*this, LuaSyntaxNodeKind::SuffixedExpression);
    } else {
        m.Undo(*this);
        return cm;
    }
}

CompleteMarker LuaParser::CallExpression() {
    auto m = Mark();

    switch (Current()) {
        case '(': {
            Next();
            if (Current() != ')') {
                // extend grammar, allow pcall(1,2,)
                ExpressionList(')');
            }

            CheckAndNext(')');
            break;
        }
        case '{': {
            TableConstructor();
            break;
        }
        case TK_LONG_STRING:
        case TK_STRING: {
            auto sm = Mark();
            Next();
            sm.Complete(*this, LuaSyntaxNodeKind::StringLiteralExpression);
            break;
        }
        default: {
            LuaExpectedError("function arguments expected");
        }
    }

    return m.Complete(*this, LuaSyntaxNodeKind::CallExpression);
}

/* fieldsel -> ['.' | ':'] NAME */
CompleteMarker LuaParser::FieldSel() {
    auto m = Mark();

    Next();
    CheckAndNext(TK_NAME);

    return m.Complete(*this, LuaSyntaxNodeKind::IndexExpression);
}

/* index -> '[' expr ']' */
CompleteMarker LuaParser::YIndex() {
    auto m = Mark();

    CheckAndNext('[');
    Expression();
    CheckAndNext(']');

    return m.Complete(*this, LuaSyntaxNodeKind::IndexExpression);
}

void LuaParser::FunctionName() {
    auto m = Mark();

    CheckAndNext(TK_NAME);

    while (Current() == '.') {
        FieldSel();
    }

    if (Current() == ':') {
        FieldSel();
    }

    m.Complete(*this, LuaSyntaxNodeKind::FunctionNameExpression);
}

void LuaParser::CheckName() {
    if (Current() == TK_NAME) {
        Next();
    }
}

/* ATTRIB -> ['<' Name '>'] */
CompleteMarker LuaParser::LocalAttribute() {
    auto m = Mark();
    if (TestAndNext('<')) {
        if (Current() == TK_NAME) {
            auto range = _tokens[_tokenIndex].Range;
            auto attr = _file->Slice(range.StartOffset, range.EndOffset);
            if (attr != "const" && attr != "close") {
                LuaExpectedError(fmt::format("unknown attribute '{}'", attr));
            }
            Next();
        } else {
            LuaExpectedError(fmt::format("expected attribute 'const' or 'close'"));
        }

        CheckAndNext('>');
        return m.Complete(*this, LuaSyntaxNodeKind::Attribute);
    }
    m.Undo(*this);
    return m.Complete(*this, LuaSyntaxNodeKind::None);
}

void LuaParser::Check(LuaTokenKind c) {
    if (Current() != c) {
        LuaExpectedError(fmt::format("{} expected", c));
    }
}

/* primaryexp -> NAME | '(' expr ')' */
CompleteMarker LuaParser::PrimaryExpression() {
    auto m = Mark();
    switch (Current()) {
        case '(': {
            Next();
            Expression();
            CheckAndNext(')');
            return m.Complete(*this, LuaSyntaxNodeKind::ParExpression);
        }
        case TK_NAME: {
            Next();
            return m.Complete(*this, LuaSyntaxNodeKind::NameExpression);
        }
        default:
            LuaExpectedError("unexpected symbol");
    }
    m.Undo(*this);
    return m.Complete(*this, LuaSyntaxNodeKind::None);
}

UnOpr LuaParser::GetUnaryOperator(LuaTokenKind op) {
    switch (op) {
        case TK_NOT: {
            return UnOpr::OPR_NOT;
        }
        case '-': {
            return UnOpr::OPR_MINUS;
        }
        case '~': {
            return UnOpr::OPR_BNOT;
        }
        case '#': {
            return UnOpr::OPR_LEN;
        }
        default: {
            return UnOpr::OPR_NOUNOPR;
        }
    }
}

BinOpr LuaParser::GetBinaryOperator(LuaTokenKind op) {
    switch (op) {
        case '+':
            return BinOpr::OPR_ADD;
        case '-':
            return BinOpr::OPR_SUB;
        case '*':
            return BinOpr::OPR_MUL;
        case '%':
            return BinOpr::OPR_MOD;
        case '^':
            return BinOpr::OPR_POW;
        case '/':
            return BinOpr::OPR_DIV;
        case TK_IDIV:
            return BinOpr::OPR_IDIV;
        case '&':
            return BinOpr::OPR_BAND;
        case '|':
            return BinOpr::OPR_BOR;
        case '~':
            return BinOpr::OPR_BXOR;
        case TK_SHL:
            return BinOpr::OPR_SHL;
        case TK_SHR:
            return BinOpr::OPR_SHR;
        case TK_CONCAT:
            return BinOpr::OPR_CONCAT;
        case TK_NE:
            return BinOpr::OPR_NE;
        case TK_EQ:
            return BinOpr::OPR_EQ;
        case '<':
            return BinOpr::OPR_LT;
        case TK_LE:
            return BinOpr::OPR_LE;
        case '>':
            return BinOpr::OPR_GT;
        case TK_GE:
            return BinOpr::OPR_GE;
        case TK_AND:
            return BinOpr::OPR_AND;
        case TK_OR:
            return BinOpr::OPR_OR;
        default:
            return BinOpr::OPR_NOBINOPR;
    }
}

void LuaParser::CheckAndNext(LuaTokenKind kind) {
    if (Current() != kind) {
        LuaExpectedError(fmt::format("token type {} expected", kind), kind);
        return;
    }

    Next();
}

bool LuaParser::TestAndNext(LuaTokenKind kind) {
    if (Current() == kind) {
        Next();
        return true;
    }
    return false;
}

void LuaParser::LuaExpectedError(std::string_view message, LuaTokenKind expectedToken) {
    auto me = MarkEvent(MarkEventType::Error);
    me.U.Error.ErrorKind = LuaParserErrorKind::Expect;
    me.U.Error.TokenKind = expectedToken;
    _events.push_back(me);
    if (_tokenIndex < _tokens.size()) {
        _errors.emplace_back(message, _tokens[_tokenIndex].Range, expectedToken);
    } else if (!_tokens.empty()) {
        auto tokenIndex = _tokens.size() - 1;
        _errors.emplace_back(message, _tokens[tokenIndex].Range, expectedToken);
    } else {
        _errors.emplace_back(message, TextRange(0, 0), expectedToken);
    }
}

void LuaParser::LuaError(std::string_view message) {
    _errors.emplace_back(message, _tokens[_tokenIndex].Range, 0);
}

CompleteMarker LuaParser::NameDefList() {
    auto m = Mark();

    CheckAndNext(TK_NAME);
    while (TestAndNext(',')) {
        CheckAndNext(TK_NAME);
    }

    return m.Complete(*this, LuaSyntaxNodeKind::NameDefList);
}

std::vector<LuaToken> &LuaParser::GetTokens() {
    return _tokens;
}
