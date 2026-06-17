#pragma once
#include "../domain/MarketData.hpp"
#include "../threading/ThreadSafeQueue.hpp"
#include <random>
#include <thread>
#include <chrono>

// Simule le marché (random walk)
class FeedSimulator {
public:

    void run(ThreadSafeQueue<MarketData>& out) {

        double mid = 1.1000;

        std::default_random_engine eng;
        std::normal_distribution<double> noise(0, 0.0001);

        for (int i = 0; i < 50; i++) {

            mid += noise(eng);

            out.push(MarketData{mid});

            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));
        }

        out.stop();
    }
};