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
