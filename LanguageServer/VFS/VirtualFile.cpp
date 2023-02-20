#include "VirtualFile.h"
#include "Lib/LineIndex/LineIndex.h"
#include "LuaParser/Lexer/LuaLexer.h"
#include "LuaParser/Parser/LuaParser.h"
#include "LuaParser/Ast/LuaTreeBuilder.h"
#include "Util/File/Url.h"
#include "VirtualFileSystem.h"

VirtualFile::VirtualFile(std::size_t fileId)
    : _fileId(fileId) {
}

std::optional<LuaSyntaxTree> VirtualFile::GetSyntaxTree(VirtualFileSystem &vfs) const {
    auto &db = vfs.GetFileDB();
    auto opFile = db.Query(_fileId);
    if (opFile.has_value()) {
        auto file = opFile.value();
        LuaLexer luaLexer(file);
        auto &tokens = luaLexer.Tokenize();

        LuaParser p(file, std::move(tokens));
        p.Parse();

        LuaSyntaxTree t(file);

        LuaTreeBuilder treeBuilder;
        treeBuilder.BuildTree(t, p);

        return t;
    }
    return std::nullopt;
}

bool VirtualFile::IsNull() const {
    return _fileId == 0;
}

std::shared_ptr<LuaFile> VirtualFile::GetLuaFile(VirtualFileSystem &vfs) const {
    auto opFile = vfs.GetFileDB().Query(_fileId);
    if (opFile) {
        return opFile.value();
    }
    return nullptr;
}

std::string FilePath2FileName(std::string_view filePath) {
    if (filePath.empty()) {
        return "";
    }

    for (std::size_t index = filePath.size(); index > 0; index--) {
        char c = filePath[index - 1];
        if (c == '/' || c == '\\') {
            return std::string(filePath.substr(index));
        }
    }

    return std::string(filePath);
}
