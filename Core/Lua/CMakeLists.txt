target_sources(Core
        # src
        PRIVATE

        # lexer
        LuaLexer/LuaLexer.cpp

        # parse
        LuaParser/LuaParser.cpp

        # docLexer
        DocLexer/LuaDocLexer.cpp
        # docParser
        DocParser/LuaDocParser.cpp

        # util
        Util/TextReader.cpp

        # parseState
        ParseState/Mark.cpp
        ParseState/ParseState.cpp

        # define
        Define/LuaIdentify.cpp
        Define/LuaOperatorType.cpp

        # node
        SyntaxNode/LuaSyntaxNode.cpp
        SyntaxNode/LuaSyntaxToken.cpp

        # node/doc
        SyntaxNode/Doc/CommentSyntax.cpp
        SyntaxNode/Doc/DocTagSyntax.cpp
        SyntaxNode/Doc/DocBaseSyntax.cpp
        SyntaxNode/Doc/DocClassSyntax.cpp
        SyntaxNode/Doc/GenericDefListSyntax.cpp
        SyntaxNode/Doc/TypeSyntax.cpp
        SyntaxNode/Doc/TypeSyntaxList.cpp
        SyntaxNode/Doc/IdTypeSyntax.cpp
        SyntaxNode/Doc/StringTypeSyntax.cpp
        SyntaxNode/Doc/FuncTypeSyntax.cpp
        SyntaxNode/Doc/FuncParamSyntaxList.cpp
        SyntaxNode/Doc/FuncParamSyntax.cpp
        SyntaxNode/Doc/ParTypeSyntax.cpp
        SyntaxNode/Doc/GenericTypeSyntax.cpp
        SyntaxNode/Doc/UnionTypeSyntax.cpp
        SyntaxNode/Doc/ArrayTypeSyntax.cpp
        SyntaxNode/Doc/TableTypeSyntax.cpp
        SyntaxNode/Doc/TableFieldTypeSyntax.cpp
        SyntaxNode/Doc/DocEnumSyntax.cpp
        SyntaxNode/Doc/DocInterfaceSyntax.cpp
        SyntaxNode/Doc/DocAliasSyntax.cpp
        SyntaxNode/Doc/DocFieldSyntax.cpp
        SyntaxNode/Doc/DocTypeSyntax.cpp
        SyntaxNode/Doc/DocParamSyntax.cpp
        SyntaxNode/Doc/DocOverloadSyntax.cpp
        SyntaxNode/Doc/DocOverrideSyntax.cpp
        SyntaxNode/Doc/DocGenericSyntax.cpp
        SyntaxNode/Doc/DocSeeSyntax.cpp
        SyntaxNode/Doc/DocSinceSyntax.cpp
        SyntaxNode/Doc/DocDeprecatedSyntax.cpp
        SyntaxNode/Doc/DocAccessSyntax.cpp
        SyntaxNode/Doc/EnumFieldSyntax.cpp
        SyntaxNode/Doc/FieldIndexSyntax.cpp
        SyntaxNode/Doc/GenericDeclareSyntax.cpp

        # node/lua
        SyntaxNode/Lua/BodySyntax.cpp
        SyntaxNode/Lua/LuaBaseSyntax.cpp
        SyntaxNode/Lua/StmtSyntax.cpp
        SyntaxNode/Lua/LocalStmtSyntax.cpp
        SyntaxNode/Lua/NameDefSyntaxList.cpp
        SyntaxNode/Lua/NameDefSyntax.cpp
        SyntaxNode/Lua/ExprSyntaxList.cpp
        SyntaxNode/Lua/ExprSyntax.cpp
        SyntaxNode/Lua/LocalFuncStmtSyntax.cpp
        SyntaxNode/Lua/ParamSyntaxList.cpp
        SyntaxNode/Lua/IfStmtSyntax.cpp
        SyntaxNode/Lua/WhileStmtSyntax.cpp
        SyntaxNode/Lua/DoStmtSyntax.cpp
        SyntaxNode/Lua/ForNumberStmtSyntax.cpp
        SyntaxNode/Lua/ForRangeStmtSyntax.cpp
        SyntaxNode/Lua/RepeatStmtSyntax.cpp
        SyntaxNode/Lua/FuncStmtSyntax.cpp
        SyntaxNode/Lua/FuncNameExprSyntax.cpp
        SyntaxNode/Lua/LabelStmtSyntax.cpp
        SyntaxNode/Lua/BreakStmtSyntax.cpp
        SyntaxNode/Lua/ReturnStmtSyntax.cpp
        SyntaxNode/Lua/GotoStmtSyntax.cpp
        SyntaxNode/Lua/CallStmtSyntax.cpp
        SyntaxNode/Lua/AssignStmtSyntax.cpp
        SyntaxNode/Lua/VarSyntaxList.cpp
        SyntaxNode/Lua/SuffixedExprSyntax.cpp
        SyntaxNode/Lua/NameExprSyntax.cpp
        SyntaxNode/Lua/ParExprSyntax.cpp
        SyntaxNode/Lua/LiteralExprSyntax.cpp
        SyntaxNode/Lua/StringLiteralExprSyntax.cpp
        SyntaxNode/Lua/ClosureExprSyntax.cpp
        SyntaxNode/Lua/UnaryExprSyntax.cpp
        SyntaxNode/Lua/BinaryExprSyntax.cpp
        SyntaxNode/Lua/TableExprSyntax.cpp
        SyntaxNode/Lua/TableFieldSyntax.cpp
        SyntaxNode/Lua/IndexExprSyntax.cpp
        SyntaxNode/Lua/CallExprSyntax.cpp

        # iterator
        SyntaxNode/Iterator/SyntaxNodeIterator.cpp

        # kind
        Kind/LuaSyntaxMultiKind.cpp
        # tree
        SyntaxTree/LuaNodeOrToken.cpp
        SyntaxTree/LuaSyntaxTree.cpp
        SyntaxTree/LuaTreeBuilder.cpp
        SyntaxTree/LuaDocTreeBuilder.cpp
        SyntaxTree/TreeUpdateEvent.cpp

        # source
        Source/LuaSource.cpp
        Source/SourceUpdateEvent.cpp

        # exception
        exception/LuaParseException.cpp

        )
