#pragma once
#include <shared_mutex>
#include <mutex>

// Cache partagé entre threads (dernier prix connu)
class MarketDataCache {
private:
    double mid = 0.0;

    mutable std::shared_mutex mtx;

public:

    // écrit par Feed
    void update(double newMid) {
        std::unique_lock lock(mtx);
        mid = newMid;
    }

    // lu par Strategy / Logger
    double get() const {
        std::shared_lock lock(mtx);
        return mid;
    }
};