#include "feed/FeedSimulator.hpp"
#include "logger/MetricsLogger.hpp"
#include "strategy/InventoryAwareStrategy.hpp"
#include "strategy/SymmetricStrategy.hpp"
#include "core/MarketMaker.hpp"
#include "threading/ThreadSafeQueue.hpp"

#include <thread>

int main() {

    ThreadSafeQueue<MarketData> marketQueue;
    ThreadSafeQueue<std::string> logQueue;

    SymmetricStrategy strategy;

    MarketMaker mm(strategy);

    FeedSimulator feed;
    MetricsLogger logger;

    // THREAD 1 : Feed
    std::thread t1([&] {
        feed.run(marketQueue);
    });

    // THREAD 2 : Strategy loop
    std::thread t2([&] {

        MarketData md;

        while (marketQueue.pop(md)) {
            mm.onMarketData(md.mid, logQueue);
        }

        logQueue.stop();
    });

    // THREAD 3 : Logger
    std::thread t3([&] {
        logger.run(logQueue);
    });

    t1.join();
    t2.join();
    t3.join();
}