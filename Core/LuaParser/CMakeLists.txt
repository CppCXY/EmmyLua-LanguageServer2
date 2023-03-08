target_sources(Core
        # src
        PRIVATE

        # lexer
        Lexer/LuaLexer.cpp

        # parse
        Parser/LuaParser.cpp

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

        # node
        SyntaxNode/LuaSyntaxNode.cpp
        SyntaxNode/BaseSyntax.cpp

        # node/doc
        SyntaxNode/Doc/CommentSyntax.cpp

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
        SyntaxNode/Lua/ForStmtSyntax.cpp
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

        # tree
        SyntaxTree/LuaSyntaxTree.cpp
        SyntaxTree/LuaSyntaxMultiKind.cpp
        SyntaxTree/LuaTreeBuilder.cpp
        SyntaxTree/LuaDocTreeBuilder.cpp

        # file
        Source/LuaSource.cpp

        # exception
        exception/LuaParseException.cpp

        # Psi
        Psi/PsiElement.cpp
        )
