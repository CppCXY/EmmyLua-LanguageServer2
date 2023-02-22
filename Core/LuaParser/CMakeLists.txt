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
        Ast/LuaSyntaxNode.cpp
        Ast/LuaSyntaxTree.cpp
        Ast/LuaSyntaxMultiKind.cpp
        Ast/LuaTreeBuilder.cpp
        Ast/LuaDocTreeBuilder.cpp

        # file
        File/LuaFile.cpp

        # exception
        exception/LuaParseException.cpp

        # Psi
        Psi/PsiElement.cpp
        )
