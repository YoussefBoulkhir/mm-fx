// ============================================================
//  mm-fx — Exercice 1.1 : Test des types de base
//  Compilation : g++ -std=c++17 -Wall -Wextra -o ex1.1 main.cpp
// ============================================================

#include <mmfx/types.hpp>
#include <iostream>
#include <type_traits>  // is_nothrow_move_constructible, etc.

using namespace mmfx;

// ------------------------------------------------------------
//  Vérifications statiques (évaluées à la compilation)
//

static_assert(std::is_nothrow_move_constructible_v<Order>,
    "Order doit etre move-constructible sans exception "
    "(necessaire pour std::vector::push_back optimal)");

static_assert(std::is_nothrow_move_assignable_v<Order>,
    "Order doit être move-assignable sans exception");

static_assert(std::is_nothrow_copy_constructible_v<Quote>,
    "Quote doit etre copy-constructible sans exception");

// Vérification que Side est bien un enum class uint8_t (pas de conversion implicite)
static_assert(sizeof(Side) == 1, "Side doit tenir sur 1 octet");

// ------------------------------------------------------------
//  Démonstration des constructions générées par le compilateur
// ------------------------------------------------------------
void demo_rule_of_zero() {
    std::cout << "=== Rule of 0 : constructions implicites ===\n\n";

    // 1. Construction par défaut (membres initialisés à 0)
    Order o1;
    std::cout << "Order() par defaut     : id=" << o1.id
              << " price=" << o1.price << " qty=" << o1.qty << "\n";

    // 2. Construction avec le constructeur de commodité
    Order o2{42, 1.0850, 100'000, Side::Buy};
    std::cout << "Order(42, 1.085, 100k, Buy) : id=" << o2.id
              << " side=" << to_string(o2.side) << "\n";

    // 3. Copie — générée par le compilateur (Rule of 0)
    Order o3 = o2;           // copy constructor
    o3.id    = 43;
    std::cout << "Copie de o2 vers o3 : o2.id=" << o2.id
              << " o3.id=" << o3.id << " (independants)\n";

    // 4. Move — généré par le compilateur (Rule of 0)
    Order o4 = std::move(o2); // move constructor
    // o2 est maintenant dans un état "valid but unspecified"
    std::cout << "Move de o2 vers o4 : o4.id=" << o4.id << "\n";

    // 5. Spread d'une Quote
    Quote q{1.0850, 1.0852, 1'000'000};
    std::cout << "\nQuote bid=" << q.bid << " ask=" << q.ask
              << " spread=" << q.spread() * 10000 << " pips\n";

    std::cout << "\n Tous les constructeurs speciaux generes par le compilateur.\n";
    std::cout << " Tous sont noexcept (types triviaux).\n";
}

// ------------------------------------------------------------
//  Démonstration de l'enum class vs enum classique
// ------------------------------------------------------------
void demo_enum_class() {
    std::cout << "\n=== enum class Side ===\n\n";

    Side s = Side::Buy;

    // Pas de conversion implicite vers int :
    // int x = s;  // ERREUR de compilation

    // Cast explicite possible :
    int x = static_cast<int>(s);
    std::cout << "Side::Buy en int = " << x << " (cast explicite requis)\n";

    switch (s) {
        case Side::Buy:  std::cout << "Ordre d'achat\n";  break;
        case Side::Sell: std::cout << "Ordre de vente\n"; break;
    }
}

int main() {
    demo_rule_of_zero();
    demo_enum_class();
    return 0;
}
