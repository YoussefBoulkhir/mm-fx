#pragma once
#include "../cache/MarketDataCache.hpp"
#include "../strategy/IQuotingStrategy.hpp"
#include "../threading/ThreadSafeQueue.hpp"
#include <string>

// Orchestrateur principal
class MarketMaker {
private:
    MarketDataCache cache;
    IQuotingStrategy& strategy;

    double inventory = 0.0;

public:

    MarketMaker(IQuotingStrategy& strat)
        : strategy(strat) {}

    void onMarketData(double mid,
                      ThreadSafeQueue<std::string>& loggerQ) {

        cache.update(mid);

        // calcul quote
        Quote q = strategy.compute(mid, inventory);

        // simulation trade simple
        inventory += 1.0; // simplifié

        // event log
        loggerQ.push(
            "mid=" + std::to_string(mid) +
            " bid=" + std::to_string(q.bid) +
            " ask=" + std::to_string(q.ask) +
            " inv=" + std::to_string(inventory)
        );
    }
};