#pragma once
#include "IQuotingStrategy.hpp"

// Stratégie simple : toujours centrée autour du mid
class SymmetricStrategy : public IQuotingStrategy {
public:
    Quote compute(double mid, double /*inventory*/) override {

        double spread = 0.0002;

        return Quote{
            mid - spread / 2,
            mid + spread / 2
        };
    }
};