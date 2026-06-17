#include <chrono>
#include <iostream>
#include <limits>
#include <vector>

constexpr int INF =
    std::numeric_limits<int>::max() / 2;

// ============================================================
// Version récursive naïve
// ============================================================

int coin_change_recursive(
    const std::vector<int>& coins,
    int amount)
{
    if (amount == 0)
    {
        return 0;
    }

    if (amount < 0)
    {
        return INF;
    }

    int best = INF;

    for (int coin : coins)
    {
        best =
            std::min(
                best,
                1 +
                coin_change_recursive(
                    coins,
                    amount - coin));
    }

    return best;
}

// ============================================================
// Mémoïsation
// ============================================================

int coin_change_memo_impl(
    const std::vector<int>& coins,
    int amount,
    std::vector<int>& memo)
{
    if (amount == 0)
    {
        return 0;
    }

    if (amount < 0)
    {
        return INF;
    }

    if (memo[amount] != -1)
    {
        return memo[amount];
    }

    int best = INF;

    for (int coin : coins)
    {
        best =
            std::min(
                best,
                1 +
                coin_change_memo_impl(
                    coins,
                    amount - coin,
                    memo));
    }

    memo[amount] = best;

    return best;
}

int coin_change_memo(
    const std::vector<int>& coins,
    int amount)
{
    std::vector<int> memo(
        amount + 1,
        -1);

    return coin_change_memo_impl(
        coins,
        amount,
        memo);
}

// ============================================================
// Bottom-Up
// ============================================================

int coin_change_bottom_up(
    const std::vector<int>& coins,
    int amount)
{
    std::vector<int> dp(
        amount + 1,
        INF);

    dp[0] = 0;

    for (int current = 1;
         current <= amount;
         ++current)
    {
        for (int coin : coins)
        {
            if (current >= coin)
            {
                dp[current] =
                    std::min(
                        dp[current],
                        dp[current - coin] + 1);
            }
        }
    }

    return dp[amount];
}

// ============================================================
// Benchmark
// ============================================================

template<typename F>
void benchmark(
    const std::string& name,
    F func)
{
    auto start =
        std::chrono::high_resolution_clock::now();

    auto result = func();

    auto end =
        std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start);

    std::cout
        << name
        << '\n'
        << "result = "
        << result
        << '\n'
        << "time   = "
        << duration.count()
        << " us\n\n";
}

// ============================================================
// Main
// ============================================================

int main()
{
    std::vector<int> coins{
        1,
        2,
        5,
        10,
        20,
        50
    };

    constexpr int amount = 200;

    std::cout
        << "Coins : ";

    for (auto c : coins)
    {
        std::cout << c << ' ';
    }

    std::cout
        << "\nAmount : "
        << amount
        << "\n\n";

    // --------------------------------------------------------
    // ATTENTION
    // La version récursive devient vite exponentielle.
    // On benchmark sur une valeur plus petite.
    // --------------------------------------------------------

    benchmark(
        "Recursive",
        [&]
        {
            return coin_change_recursive(
                coins,
                30);
        });

    benchmark(
        "Memoization",
        [&]
        {
            return coin_change_memo(
                coins,
                amount);
        });

    benchmark(
        "Bottom-Up",
        [&]
        {
            return coin_change_bottom_up(
                coins,
                amount);
        });

    return 0;
}