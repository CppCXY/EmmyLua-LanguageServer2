target_sources(Core
        # src
        PRIVATE

        # parse
        Parser/LuaParser.cpp
        Parser/Mark.cpp

        # lexer
        Lexer/LuaLexer.cpp
        Lexer/LuaIdentify.cpp
        Lexer/TextReader.cpp

        # ast
        Ast/LuaSyntaxNode.cpp
        Ast/LuaSyntaxTree.cpp
        Ast/LuaSyntaxMultiKind.cpp
        Ast/LuaSyntaxNodeKind.cpp

        # types
        Types/TextRange.cpp

        # file
        File/LuaFile.cpp

        # exception
        exception/LuaParseException.cpp
        )
