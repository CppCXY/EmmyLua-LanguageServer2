target_sources(Core
        # src
        PRIVATE

        # parse
        Parser/LuaParser.cpp
        Util/Mark.cpp

        # lexer
        Lexer/LuaLexer.cpp
        Define/LuaIdentify.cpp
        Util/TextReader.cpp

        # ast
        Ast/LuaSyntaxNode.cpp
        Ast/LuaSyntaxTree.cpp
        Ast/LuaSyntaxMultiKind.cpp
        Ast/LuaSyntaxNodeKind.cpp

        # types
        Define/TextRange.cpp

        # file
        File/LuaFile.cpp

        # exception
        exception/LuaParseException.cpp

        # Psi
        Psi/PsiElement.cpp
        )
