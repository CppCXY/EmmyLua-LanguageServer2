#pragma once

#include "Core/Lib/LSP/LSP.h"
#include "Core/LuaParser/Ast/LuaSyntaxTree.h"
#include "DB/FileDB.h"
#include "Core/Lib/LineIndex/LineIndex.h"

class VirtualFileSystem;

class VirtualFile
{
public:
	explicit VirtualFile(std::size_t fieldId);

    bool IsNull() const;

    std::optional<LuaSyntaxTree> GetSyntaxTree(VirtualFileSystem& vfs) const;

    std::shared_ptr<LineIndex> GetLineIndex(VirtualFileSystem& vfs) const;
private:
	std::size_t _fileId;
};