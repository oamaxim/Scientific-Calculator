#pragma once
#include <memory>
#include <vector>
#include <string>

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

    NumberNode(double v)
        : value(v) {}
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

struct VariableNode : ASTNode
{
    std::string name;

    VariableNode(const std::string &n)
        : name(n) {}
};

struct AssignNode : ASTNode
{
    std::string name;
    std::unique_ptr<ASTNode> value;

    AssignNode(const std::string &n, std::unique_ptr<ASTNode> v)
        : name(n), value(std::move(v)) {}
};

struct FunctionNode : ASTNode
{
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> args;

    FunctionNode(const std::string &n,
                 std::vector<std::unique_ptr<ASTNode>> a)
        : name(std::move(n)), args(std::move(a)) {}
};