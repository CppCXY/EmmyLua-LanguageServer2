#pragma once

#include "LuaParser/SyntaxTree/LuaSyntaxTree.h"

class LuaWorkspace {
public:
    LuaWorkspace() = default;

    void AddDocument();

    void RemoveDocument();

    void UpdateDocument();

private:
};