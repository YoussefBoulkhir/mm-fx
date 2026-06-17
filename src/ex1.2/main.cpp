// ============================================================
//  mm-fx — Exercice 1.2 : main — observer copie vs mouvement
//  Compilation : g++ -std=c++17 -Wall -Wextra -o ex1.2 main.cpp
// ============================================================

#include "order_book.hpp"
#include <iostream>

using namespace mmfx;

// Separateur visuel
void sep(const char* title) {
    std::printf("\n-------------------\n");
    std::printf("  %s\n", title);
    std::printf("---------------------\n");
}

int main() {
    // --------------------------------------------------------
    //  SCeNARIO A : add_by_const_ref avec une lvalue
    //  Attendu : 1 COPY ctor (push_back copie dans le vecteur)
    // --------------------------------------------------------
    sep("A. add_by_const_ref(lvalue) -> 1 COPY");
    {
        OrderBook book;
        book.reserve(4);   // pas de reallocation pendant le test

        InstrumentedOrder o{1, 1.0850, 100'000, Side::Buy};
        std::puts("  -> appel add_by_const_ref(o)");
        book.add_by_const_ref(o);
        // o existe encore ici (non modifie)
        std::printf("  o.id apres appel = %llu (non modifie)\n",
                    static_cast<unsigned long long>(o.id));
    } // destructeurs a la fin du scope

    // --------------------------------------------------------
    //  SCeNARIO B : add_by_value avec une lvalue
    //  Attendu : 1 COPY ctor (passage du parametre) + 1 MOVE ctor
    //            (push_back avec std::move dans la fonction)
    // --------------------------------------------------------
    sep("B. add_by_value(lvalue) -> 1 COPY + 1 MOVE");
    {
        OrderBook book;
        book.reserve(4);

        InstrumentedOrder o{2, 1.0851, 50'000, Side::Sell};
        std::puts("  -> appel add_by_value(o)");
        book.add_by_value(o);
        std::printf("  o.id apres appel = %llu (non modifie car lvalue copiee)\n",
                    static_cast<unsigned long long>(o.id));
    }

    // --------------------------------------------------------
    //  SCeNARIO C : add_by_value avec une rvalue (std::move)
    //  Attendu : 1 MOVE ctor (passage) + 1 MOVE ctor (push_back)
    //  Aucune copie !
    // --------------------------------------------------------
    sep("C. add_by_value(std::move(o)) -> 2 MOVE");
    {
        OrderBook book;
        book.reserve(4);

        InstrumentedOrder o{3, 1.0852, 200'000, Side::Buy};
        std::puts("  -> appel add_by_value(std::move(o))");
        book.add_by_value(std::move(o));
        // ATTENTION : o est dans un etat "valid but unspecified"
        // On ne doit plus lire o.id de façon significative
        std::printf("  o.id apres move = %llu (etat non specifie)\n",
                    static_cast<unsigned long long>(o.id));
    }

    // --------------------------------------------------------
    //  SCeNARIO D : add_by_value avec une rvalue temporaire
    //  Attendu : 1 MOVE ctor (la construction du temporaire
    //            + le passage fusionnent grâce au RVO/NRVO)
    //            + 1 MOVE ctor (push_back)
    // --------------------------------------------------------
    sep("D. add_by_value(rvalue temporaire) -> 1-2 MOVE (RVO)");
    {
        OrderBook book;
        book.reserve(4);

        std::puts("  -> appel add_by_value({4, 1.085, 75000, Sell})");
        book.add_by_value(InstrumentedOrder{4, 1.0853, 75'000, Side::Sell});
        //  Le compilateur peut eliminer le move du temporaire (RVO)
    }

    // --------------------------------------------------------
    //  SCeNARIO E : emplace_back — le plus efficace
    //  Attendu : 1 ctor (construction directe dans le vecteur)
    //            Aucune copie, aucun move
    // --------------------------------------------------------
    sep("E. emplace_back -> 1 ctor direct (0 copie, 0 move)");
    {
        OrderBook book;
        book.reserve(4);

        std::puts("  -> appel add_emplace(5, 1.085, 50000, Buy)");
        book.add_emplace(5, 1.0854, 50'000, Side::Buy);
        std::puts("  Construit directement dans le vecteur !");
    }

    // --------------------------------------------------------
    //  SCeNARIO F : impact du reserve() sur les reallocations
    //  Sans reserve, push_back peut declencher une reallocation
    //  -> move de TOUS les elements existants vers le nouveau buffer
    // --------------------------------------------------------
    sep("F. Sans reserve() -> reallocations visibles");
    {
        OrderBook book;
        // Pas de reserve : reallocation apres chaque depassement de capacite

        std::puts("  Insertion sans reserve (capacite initiale = 0) :");
        book.add_emplace(10, 1.0850, 1000, Side::Buy);
        std::puts("  --- 2e insertion (realloc probable) ---");
        book.add_emplace(11, 1.0851, 2000, Side::Sell);
        std::puts("  --- 3e insertion ---");
        book.add_emplace(12, 1.0852, 3000, Side::Buy);
        std::puts("  -> Remarquez les MOVE ctor declenches par les reallocations.");
        std::puts("  -> C'est pourquoi noexcept sur le move ctor est critique.");
    }

    std::puts("\n=== Resume ===");
    std::puts("  add_by_const_ref(lvalue)  : 1 COPY");
    std::puts("  add_by_value(lvalue)       : 1 COPY + 1 MOVE");
    std::puts("  add_by_value(rvalue)       : 2 MOVE (0 copie)");
    std::puts("  emplace_back               : 1 ctor direct (optimal)");

    return 0;
}
