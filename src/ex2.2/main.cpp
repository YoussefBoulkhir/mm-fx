#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include "mmfx/types.hpp"

namespace mmfx
{

struct Trade
{
    Price price;
    Quantity volume;
};

}

using namespace mmfx;

int main()
{
    // --------------------------------------------------
    // Génération aléatoire de trades
    // --------------------------------------------------

    std::mt19937 rng{42};

    std::uniform_real_distribution<double> price_dist(99.0, 101.0);
    std::uniform_int_distribution<int> volume_dist(1, 1000);

    std::vector<Trade> trades(20);

    std::generate(
        trades.begin(),
        trades.end(),
        [&]()
        {
            return Trade{
                price_dist(rng),
                volume_dist(rng)};
        });

    // --------------------------------------------------
    // Affichage
    // --------------------------------------------------

    std::cout << "Generated trades\n\n";

    for (const auto& t : trades)
    {
        std::cout
            << "price="
            << std::fixed
            << std::setprecision(4)
            << t.price
            << " volume="
            << t.volume
            << '\n';
    }

    // --------------------------------------------------
    // Volume total
    // --------------------------------------------------

    const int total_volume =
        std::accumulate(
            trades.begin(),
            trades.end(),
            0,
            [](int acc, const Trade& t)
            {
                return acc + t.volume;
            });

    // --------------------------------------------------
    // VWAP
    // --------------------------------------------------

    const double total_notional =
        std::accumulate(
            trades.begin(),
            trades.end(),
            0.0,
            [](double acc, const Trade& t)
            {
                return acc + t.price * t.volume;
            });

    const double vwap =
        total_volume == 0
            ? 0.0
            : total_notional / total_volume;

    // --------------------------------------------------
    // Nombre de trades > seuil
    // --------------------------------------------------

    constexpr double threshold = 100.50;

    const auto count_above_threshold =
        std::count_if(
            trades.begin(),
            trades.end(),
            [&](const Trade& t)
            {
                return t.price > threshold;
            });

    // --------------------------------------------------
    // Top 5 volumes
    // --------------------------------------------------

    auto top_trades = trades;

    constexpr std::size_t top_n = 5;

    std::partial_sort(
        top_trades.begin(),
        top_trades.begin() + top_n,
        top_trades.end(),
        [](const Trade& lhs, const Trade& rhs)
        {
            return lhs.volume > rhs.volume;
        });

    top_trades.resize(top_n);

    // --------------------------------------------------
    // Résultats
    // --------------------------------------------------

    std::cout << "\n=========================\n";
    std::cout << "STATISTICS\n";
    std::cout << "=========================\n";

    std::cout
        << "Total volume : "
        << total_volume
        << '\n';

    std::cout
        << "VWAP         : "
        << vwap
        << '\n';

    std::cout
        << "Trades > "
        << threshold
        << " : "
        << count_above_threshold
        << '\n';

    std::cout << "\nTop 5 volumes\n";

    for (const auto& t : top_trades)
    {
        std::cout
            << "price="
            << t.price
            << " volume="
            << t.volume
            << '\n';
    }

    return 0;
}