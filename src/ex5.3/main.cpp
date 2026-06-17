#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

/////////////////////////////////////////////////////////////////
// GENERATION DU VECTEUR
/////////////////////////////////////////////////////////////////

std::vector<int> generateVector(size_t n)
{
    std::vector<int> v;
    v.reserve(n);

    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 100);

    for (size_t i = 0; i < n; ++i)
    {
        v.push_back(dist(gen));
    }

    return v;
}

/////////////////////////////////////////////////////////////////
// VERSION SEQUENTIELLE
/////////////////////////////////////////////////////////////////

long long sequentialSum(const std::vector<int>& v)
{
    return std::accumulate(v.begin(), v.end(), 0LL);
}

/////////////////////////////////////////////////////////////////
// VERSION ASYNC (PARTIEL)
/////////////////////////////////////////////////////////////////

long long partialSum(
    const std::vector<int>& v,
    size_t start,
    size_t end)
{
    long long sum = 0;

    for (size_t i = start; i < end; ++i)
    {
        sum += v[i];
    }

    return sum;
}

/////////////////////////////////////////////////////////////////
// VERSION PARALLELE ASYNC
/////////////////////////////////////////////////////////////////

long long parallelSum(
    const std::vector<int>& v,
    int numTasks)
{
    size_t n = v.size();
    size_t chunkSize = n / numTasks;

    std::vector<std::future<long long>> futures;
    futures.reserve(numTasks);

    for (int t = 0; t < numTasks; ++t)
    {
        size_t start = t * chunkSize;
        size_t end = (t == numTasks - 1)
                         ? n
                         : (t + 1) * chunkSize;

        futures.push_back(
            std::async(std::launch::async,
                       partialSum,
                       std::cref(v),
                       start,
                       end));
    }

    long long total = 0;

    for (auto& f : futures)
    {
        total += f.get();
    }

    return total;
}

/////////////////////////////////////////////////////////////////
// BENCHMARK
/////////////////////////////////////////////////////////////////

template <typename Func>
double benchmark(Func f, const std::string& name)
{
    auto start = std::chrono::high_resolution_clock::now();

    long long result = f();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;

    std::cout << name
              << " | result = "
              << result
              << " | time = "
              << diff.count()
              << " sec\n";

    return diff.count();
}

/////////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////////

int main()
{
    constexpr size_t N = 10'000'000;

    std::cout << "Generating vector...\n";
    std::vector<int> v = generateVector(N);

    std::cout << "Vector size = " << v.size() << "\n\n";

    /////////////////////////////////////////////////////////////
    // SEQUENTIAL
    /////////////////////////////////////////////////////////////

    auto seqTime = benchmark(
        [&]()
        {
            return sequentialSum(v);
        },
        "Sequential std::accumulate");

    /////////////////////////////////////////////////////////////
    // PARALLEL TESTS
    /////////////////////////////////////////////////////////////

    for (int tasks : {1, 2, 4, 8, 16})
    {
        auto parTime = benchmark(
            [&]()
            {
                return parallelSum(v, tasks);
            },
            "Parallel async tasks = " + std::to_string(tasks));

        std::cout << "Speedup = "
                  << seqTime / parTime
                  << "x\n\n";
    }

    return 0;
}