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

        # ast
        SyntaxNode/LuaSyntaxNode.cpp
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
