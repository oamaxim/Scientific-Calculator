#pragma once
#include <memory>

enum class NodeType
{
    Number,
    BINARY_OP
};

struct ASTNode
{
    virtual ~ASTNode() = default;
};

struct NumberNode : ASTNode
{
    double value;

    NumberNode(double v) : value(v) {}
};

struct BinaryNode : ASTNode
{
    char op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryNode(char o,
               std::unique_ptr<ASTNode> l,
               std::unique_ptr<ASTNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};
