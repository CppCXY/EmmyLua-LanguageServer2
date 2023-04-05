#pragma once

class Compilation;

class SemanticModel {
public:
    explicit SemanticModel(Compilation* owner);



private:
    Compilation * _owner;
};

