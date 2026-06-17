#include <iostream>
#include <memory>
#include <string>

// ============================================================
// Visitor forward
// ============================================================

class Number;
class Addition;
class Multiplication;

// ============================================================
// Visitor interface
// ============================================================

class IVisitor
{
public:
    virtual double visit(Number&) = 0;
    virtual double visit(Addition&) = 0;
    virtual double visit(Multiplication&) = 0;
    virtual ~IVisitor() = default;
};

// ============================================================
// Expression base
// ============================================================

class Expression
{
public:
    virtual ~Expression() = default;
    virtual double accept(IVisitor&) = 0;
};

// ============================================================
// Nodes
// ============================================================

class Number : public Expression
{
public:
    double value;

    Number(double v) : value(v) {}

    double accept(IVisitor& v) override
    {
        return v.visit(*this);
    }
};

class Addition : public Expression
{
public:
    std::unique_ptr<Expression> left, right;

    Addition(std::unique_ptr<Expression> l,
             std::unique_ptr<Expression> r)
        : left(std::move(l)), right(std::move(r)) {}

    double accept(IVisitor& v) override
    {
        return v.visit(*this);
    }
};

class Multiplication : public Expression
{
public:
    std::unique_ptr<Expression> left, right;

    Multiplication(std::unique_ptr<Expression> l,
                   std::unique_ptr<Expression> r)
        : left(std::move(l)), right(std::move(r)) {}

    double accept(IVisitor& v) override
    {
        return v.visit(*this);
    }
};

// ============================================================
// Evaluator
// ============================================================

class EvalVisitor : public IVisitor
{
public:
    double visit(Number& n) override
    {
        return n.value;
    }

    double visit(Addition& a) override
    {
        return a.left->accept(*this)
             + a.right->accept(*this);
    }

    double visit(Multiplication& m) override
    {
        return m.left->accept(*this)
             * m.right->accept(*this);
    }
};

// ============================================================
// Main
// ============================================================

int main()
{
    auto expr =
        std::make_unique<Addition>(
            std::make_unique<Number>(2),
            std::make_unique<Multiplication>(
                std::make_unique<Number>(3),
                std::make_unique<Number>(4)));

    EvalVisitor eval;

    std::cout << expr->accept(eval) << "\n"; // 2 + 3*4 = 14
}