#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

struct Quote
{
    double bid{};
    double ask{};
};

/////////////////////////////////////////////////////////////////
// MUTEX VERSION
/////////////////////////////////////////////////////////////////

class MarketDataCacheMutex
{
public:
    void updateQuote(const std::string& symbol, const Quote& quote)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        quotes_[symbol] = quote;
    }

    Quote getQuote(const std::string& symbol) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = quotes_.find(symbol);
        if (it != quotes_.end())
            return it->second;

        return {};
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, Quote> quotes_;
};

/////////////////////////////////////////////////////////////////
// SHARED_MUTEX VERSION
/////////////////////////////////////////////////////////////////

class MarketDataCacheRW
{
public:
    void updateQuote(const std::string& symbol, const Quote& quote)
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        quotes_[symbol] = quote;
    }

    Quote getQuote(const std::string& symbol) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);

        auto it = quotes_.find(symbol);
        if (it != quotes_.end())
            return it->second;

        return {};
    }

private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, Quote> quotes_;
};

/////////////////////////////////////////////////////////////////
// BENCHMARK
/////////////////////////////////////////////////////////////////

template <typename Cache>
std::uint64_t benchmark(Cache& cache, int nbReaders, int durationSeconds)
{
    std::atomic<bool> running{true};
    std::atomic<std::uint64_t> readCount{0};

    cache.updateQuote("EUR/USD", {1.1000, 1.1002});

    // WRITER
    std::thread writer([&]()
    {
        int counter = 0;

        while (running.load(std::memory_order_relaxed))
        {
            cache.updateQuote(
                "EUR/USD",
                {
                    1.1000 + counter * 1e-7,
                    1.1002 + counter * 1e-7
                });

            ++counter;
        }
    });

    // READERS
    std::vector<std::thread> readers;
    readers.reserve(nbReaders);

    for (int i = 0; i < nbReaders; ++i)
    {
        readers.emplace_back([&]()
        {
            while (running.load(std::memory_order_relaxed))
            {
                auto q = cache.getQuote("EUR/USD");

                volatile double spread = q.ask - q.bid;
                (void)spread;

                readCount.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // RUN
    std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));
    running.store(false);

    // JOIN
    writer.join();

    for (auto& t : readers)
        t.join();

    return readCount.load();
}

/////////////////////////////////////////////////////////////////
// DRIVER
/////////////////////////////////////////////////////////////////

template <typename Cache>
void runBench(const std::string& name)
{
    std::cout << "\n=============================\n";
    std::cout << name << "\n";
    std::cout << "=============================\n";

    constexpr int duration = 5;

    for (int r = 1; r <= 16; r *= 2)
    {
        Cache cache;

        auto reads = benchmark(cache, r, duration);

        std::cout << r
                  << " readers -> "
                  << (reads / duration)
                  << " reads/sec\n";
    }
}

/////////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////////

int main()
{
    std::cout << "Hardware threads: "
              << std::thread::hardware_concurrency()
              << "\n";

    runBench<MarketDataCacheMutex>("std::mutex");
    runBench<MarketDataCacheRW>("std::shared_mutex");

    return 0;
}

