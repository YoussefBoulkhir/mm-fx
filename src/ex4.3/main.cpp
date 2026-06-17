#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

// ============================================================
// Strategy interface
// ============================================================

class ISortStrategy
{
public:
    virtual void sort(std::vector<int>& v) = 0;
    virtual ~ISortStrategy() = default;
};

// ============================================================
// BubbleSort
// ============================================================

class BubbleSort : public ISortStrategy
{
public:
    void sort(std::vector<int>& v) override
    {
        for (size_t i = 0; i < v.size(); ++i)
            for (size_t j = 0; j + 1 < v.size(); ++j)
                if (v[j] > v[j + 1])
                    std::swap(v[j], v[j + 1]);
    }
};

// ============================================================
// QuickSort (wrapper std::sort style)
// ============================================================

class QuickSort : public ISortStrategy
{
public:
    void sort(std::vector<int>& v) override
    {
        std::sort(v.begin(), v.end());
    }
};

// ============================================================
// StdSort strategy
// ============================================================

class StdSort : public ISortStrategy
{
public:
    void sort(std::vector<int>& v) override
    {
        std::sort(v.begin(), v.end());
    }
};

// ============================================================
// Factory
// ============================================================

class SortFactory
{
public:
    static std::unique_ptr<ISortStrategy> create(const std::string& type)
    {
        if (type == "bubble")
            return std::make_unique<BubbleSort>();

        if (type == "quick")
            return std::make_unique<QuickSort>();

        if (type == "std")
            return std::make_unique<StdSort>();

        return nullptr;
    }
};

// ============================================================
// Benchmark
// ============================================================

template<typename F>
void bench(const std::string& name, F f)
{
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << name << " : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end - start)
                     .count()
              << " ms\n";
}

// ============================================================
// Main
// ============================================================

int main()
{
    std::vector<int> data(100000);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1000000);

    for (auto& x : data)
        x = dist(rng);

    for (auto type : {"bubble", "quick", "std"})
    {
        auto strat = SortFactory::create(type);

        auto copy = data;

        bench(type, [&]
        {
            strat->sort(copy);
        });
    }
}