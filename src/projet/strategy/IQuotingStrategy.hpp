#pragma once
#include "../domain/Quote.hpp"

// Interface Strategy (OCP : on peut ajouter des stratégies sans modifier le reste)
class IQuotingStrategy {
public:
    virtual Quote compute(double mid, double inventory) = 0;

    virtual ~IQuotingStrategy() = default;
};