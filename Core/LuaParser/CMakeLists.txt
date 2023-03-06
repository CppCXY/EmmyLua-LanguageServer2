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
        SyntaxNode/Lua/StatementSyntax.cpp
        SyntaxNode/Lua/LocalStatementSyntax.cpp
        SyntaxNode/Lua/NameDefSyntaxList.cpp
        SyntaxNode/Lua/NameDefSyntax.cpp
        SyntaxNode/Lua/ExprSyntaxList.cpp
        SyntaxNode/Lua/ExprSyntax.cpp
        SyntaxNode/Lua/LocalFunctionStatementSyntax.cpp
        SyntaxNode/Lua/ParamSyntaxList.cpp
        SyntaxNode/Lua/IfStatementSyntax.cpp
        SyntaxNode/Lua/WhileStatementSyntax.cpp
        SyntaxNode/Lua/DoStatementSyntax.cpp
        SyntaxNode/Lua/ForStatementSyntax.cpp
        SyntaxNode/Lua/RepeatStatementSyntax.cpp
        SyntaxNode/Lua/FunctionStatementSyntax.cpp
        SyntaxNode/Lua/FunctionNameExprSyntax.cpp
        SyntaxNode/Lua/LabelStatementSyntax.cpp
        SyntaxNode/Lua/BreakStatementSyntax.cpp
        SyntaxNode/Lua/ReturnStatementSyntax.cpp

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
