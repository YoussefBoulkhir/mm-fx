// ============================================================
//  mm-fx — Exercice 1.4 : Localite de cache
//  Comparaison vector<Order> vs vector<unique_ptr<Order>>
//
//  Compilation (optimisations activees — comme en production) :
//    g++ -std=c++17 -O2 -o ex1.4 main.cpp
//
//  Compilation (sans optimisations — ecart plus visible) :
//    g++ -std=c++17 -O0 -o ex1.4 main.cpp
// ============================================================

#include <mmfx/types.hpp>
#include <vector>
#include <memory>
#include <chrono>
#include <iostream>
#include <numeric>    // std::accumulate
#include <iomanip>    // std::setw
#include <random>

using namespace mmfx;
using Clock = std::chrono::high_resolution_clock;
using Ms    = std::chrono::duration<double, std::milli>;

// ============================================================
//  Pourquoi y a-t-il une difference ?
//
//  Modele memoire simplifie d'un CPU moderne :
//
//    CPU  <-->  L1 cache (32 KB, ~1 ns)
//         <-->  L2 cache (256 KB, ~4 ns)
//         <-->  L3 cache (8 MB, ~20 ns)
//         <-->  RAM (8 GB, ~80 ns)
//
//  Une "ligne de cache" = 64 octets charges d'un coup depuis la RAM.
//  sizeof(Order) = 24 octets (id:8 + price:8 + qty:4 + side:1 + padding:3)
//  -> 2-3 Order tiennent dans une ligne de cache.
//
//  vector<Order> : [ O1 O2 O3 O4 O5 O6 ... ] <- contigu en memoire
//    Acceder a O1 charge aussi O2, O3 dans le cache -> L1 hit pour O2, O3
//    Le CPU peut predire les acces (prefetching)
//
//  vector<unique_ptr<Order>> : [ ptr1 ptr2 ptr3 ... ] -> heap disperse
//    ptr1 -> [adresse_aleatoire_1] -> cache MISS
//    ptr2 -> [adresse_aleatoire_2] -> cache MISS
//    ptr3 -> [adresse_aleatoire_3] -> cache MISS
//    Chaque acces declenche un aller-retour vers la RAM (80 ns)
// ============================================================

constexpr int N = 1'000'000;

// Empêche le compilateur d'eliminer les calculs (optimisation agressive)
// volatile sink force la lecture effective
volatile double g_sink = 0.0;

// ============================================================
//  Benchmark 1 : vector<Order> — acces contigus
// ============================================================
double bench_vector_direct() {
    // Construction
    std::vector<Order> orders;
    orders.reserve(N);
    for (int i = 0; i < N; ++i) {
        orders.emplace_back(
            static_cast<uint64_t>(i),
            1.0850 + i * 0.00001,
            100 + i % 1000,
            i % 2 == 0 ? Side::Buy : Side::Sell
        );
    }

    // Benchmark : iteration et somme des prix
    auto t1 = Clock::now();

    double sum = 0.0;
    for (const auto& o : orders) {
        sum += o.price;  // acces sequentiel — cache friendly
    }
    g_sink = sum;  // empêche d'eliminer la boucle

    auto t2 = Clock::now();
    return Ms{t2 - t1}.count();
}

// ============================================================
//  Benchmark 2 : vector<unique_ptr<Order>> — pointeurs disperses
// ============================================================
double bench_vector_ptr() {
    // Construction : N allocations heap individuelles
    std::vector<std::unique_ptr<Order>> orders;
    orders.reserve(N);
    for (int i = 0; i < N; ++i) {
        orders.push_back(std::make_unique<Order>(
            static_cast<uint64_t>(i),
            1.0850 + i * 0.00001,
            100 + i % 1000,
            i % 2 == 0 ? Side::Buy : Side::Sell
        ));
    }

    // Benchmark : iteration et somme des prix
    auto t1 = Clock::now();

    double sum = 0.0;
    for (const auto& ptr : orders) {
        sum += ptr->price;  // dereferencement -> potentiel cache miss
    }
    g_sink = sum;

    auto t2 = Clock::now();
    return Ms{t2 - t1}.count();
}

// ============================================================
//  Benchmark 3 : vector<unique_ptr<Order>> avec ordre aleatoire
//  Simule la fragmentation reelle du heap apres allocations/
//  desallocations multiples (cas reel d'un systeme de trading)
// ============================================================
double bench_vector_ptr_shuffled() {
    std::vector<std::unique_ptr<Order>> orders;
    orders.reserve(N);

    // Intercaler des allocations "polluantes" pour fragmenter le heap
    std::vector<std::unique_ptr<int>> noise;
    noise.reserve(N / 2);

    for (int i = 0; i < N; ++i) {
        orders.push_back(std::make_unique<Order>(
            static_cast<uint64_t>(i), 1.0850 + i * 0.00001,
            100, Side::Buy));
        // Toutes les 2 insertions, on alloue un int parasite
        if (i % 2 == 0) noise.push_back(std::make_unique<int>(i));
    }
    noise.clear();  // libere les int -> cree des trous dans le heap

    auto t1 = Clock::now();
    double sum = 0.0;
    for (const auto& ptr : orders) {
        sum += ptr->price;
    }
    g_sink = sum;
    auto t2 = Clock::now();
    return Ms{t2 - t1}.count();
}

// ============================================================
//  Affichage du rapport
// ============================================================
void print_result(const char* label, double ms, double ref_ms) {
    double ratio = ms / ref_ms;
    std::cout << "  " << std::left << std::setw(42) << label
              << std::right << std::setw(8) << std::fixed
              << std::setprecision(2) << ms << " ms"
              << "  (×" << std::setprecision(1) << ratio << " vs reference)\n";
}

int main() {
    std::cout << "=== Cache locality benchmark ===\n";
    std::cout << "  N = " << N << " ordres\n";
    std::cout << "  sizeof(Order)         = " << sizeof(Order) << " octets\n";
    std::cout << "  sizeof(unique_ptr<Order>) = " << sizeof(std::unique_ptr<Order>) << " octets\n";
    std::cout << "  Ligne de cache        = 64 octets\n";
    std::cout << "  Order / ligne cache   = " << 64 / sizeof(Order) << "\n\n";

    // Warm-up (JIT de l'OS, pages physiques, etc.)
    bench_vector_direct();
    bench_vector_ptr();

    // Mesures
    double t_direct   = bench_vector_direct();
    double t_ptr      = bench_vector_ptr();
    double t_shuffled = bench_vector_ptr_shuffled();

    std::cout << "-----------------------------------\n";
    std::cout << "  Resultats (iteration sur " << N / 1'000'000 << "M ordres)\n";
    std::cout << "-----------------------------------\n";
    print_result("vector<Order>           (contigu)", t_direct,   t_direct);
    print_result("vector<unique_ptr<Order>> (sequentiel)", t_ptr,      t_direct);
    print_result("vector<unique_ptr<Order>> (fragmente)", t_shuffled, t_direct);
    std::cout << "------------------------------------\n";

    std::cout << "\n=== Analyse ===\n";
    std::cout << "  vector<Order> est ~" << std::setprecision(0)
              << (t_ptr / t_direct) << "x plus rapide.\n";
    std::cout << "\n  Explication :\n";
    std::cout << "  - vector<Order> : objets contigus -> prechargement CPU efficace\n";
    std::cout << "    Chaque ligne de cache contient " << 64/sizeof(Order)
              << " Order -> ratio hit/miss eleve.\n\n";
    std::cout << "  - vector<unique_ptr> : N allocations heap separees\n";
    std::cout << "    -> adresses aleatoires -> cache miss presque systematique\n";
    std::cout << "    -> aller-retour RAM (~80 ns) pour chaque dereferencement.\n\n";

    std::cout << "  Regle pratique (systemes de trading) :\n";
    std::cout << "  -> Stocker les Order par valeur dans le hot path.\n";
    std::cout << "  -> Reserver unique_ptr aux cas ou la duree de vie\n";
    std::cout << "     dynamique est reellement necessaire.\n";

    return 0;
}
