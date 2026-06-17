#pragma once
// ============================================================
//  mm-fx — Exercice 1.1 : Types de base du projet
// ============================================================
//
//  Objectif : définir Price, Quantity, Side, Quote, Order
//  en respectant la Rule of 0, avec les bons constructeurs
//  et le marquage noexcept approprié.
//
//  Rule of 0 : si tous les membres gèrent eux-mêmes leurs
//  ressources (int, double, enum, string…), on ne définit
//  AUCUN constructeur spécial — le compilateur génère les
//  cinq opérations correctement et efficacement.
// ============================================================

#include <cstdint>   // uint64_t
#include <string>    // std::string
#include <compare>
#include <map>

namespace mmfx {

// ------------------------------------------------------------
//  Alias de types — "Strong typedefs" légers
//
//  On utilise des alias plutôt que des types primitifs bruts
//  pour documenter l'intention dans les signatures de fonctions.
//  Exemple : void fill(Price p, Quantity q) est plus lisible
//  que void fill(double p, int q).
// ------------------------------------------------------------
using Price    = double;
using Quantity = int;



// struct key {

//     auto operator<(key& ky) = default;

//     std::vector<int> top_N(std::vector<int> vec , int N){
//         // N elements les plus grands
//         // dacler vertuellement
//         // Queue ,indice !!
//         // faire avec decalage vertuelle 


//     }



// }


// std::map<key , int> mp ; 

// struct Price{
//     public : 

//     Price(double val){
//         val_ = val;
//     };

//     Price& operator=(Price& price) = delete;
//     Price& operator=(Price&& price) = delete;

//     Price(Price& price) = delete;
//     Price(Price&& price) = delete;

//     auto operator<=>(Price& price) const = default;

//     private :
//         double val_;

// }

// ------------------------------------------------------------
//  Side — énumération du côté de l'ordre
//
//  enum class (C++11) :
//    - pas de conversion implicite vers int (sûr)
//    - pas de pollution du namespace englobant
//    - comparaison et switch explicites
// ------------------------------------------------------------
enum class Side : uint8_t {
    Buy,
    Sell
};

// Utilitaire d'affichage
constexpr const char* to_string(Side s) noexcept {
    return s == Side::Buy ? "Buy" : "Sell";
}

// ------------------------------------------------------------
//  Quote — cotation bid/ask fournie par un fournisseur de prix
//
//  Rule of 0 appliquée :
//    - tous les membres sont triviaux (double, int)
//    - le compilateur génère : default ctor, copy ctor,
//      move ctor, copy assign, move assign, destructor
//    - tous sont implicitement noexcept (types triviaux)
//
//  Constructeur utilisateur (non-spécial) fourni pour
//  faciliter l'initialisation.
// ------------------------------------------------------------
struct Quote {
    Price    bid  {0.0};  // meilleur prix acheteur
    Price    ask  {0.0};  // meilleur prix vendeur
    Quantity size {0};    // quantité disponible

    
    // noexcept car aucune allocation, aucune exception possible
    Quote(Price bid_, Price ask_, Quantity size_) noexcept
        : bid{bid_}, ask{ask_}, size{size_}
    {}

    // Constructeur par défaut explicite (les membres ont des
    // initialiseurs, donc le compilateur en génère un,
    // mais l'écrire = default documente l'intention)
    Quote() = default;

    // Spread en points de base
    double spread() const noexcept { return ask - bid; }
};

// ------------------------------------------------------------
//  Order — ordre passé sur le marché
//
//  Rule of 0 : identique à Quote.
//  Tous les membres sont primitifs ou enum.
// noexcept pour que vector utilise le move
struct Order {
    uint64_t id    {0};
    Price    price {0.0};
    Quantity qty   {0};
    Side     side  {Side::Buy};

    // Constructeur de commodité
    Order(uint64_t id_, Price price_, Quantity qty_, Side side_) noexcept
        : id{id_}, price{price_}, qty{qty_}, side{side_}
    {}

    Order() = default;

    bool is_buy()  const noexcept { return side == Side::Buy;  }
    bool is_sell() const noexcept { return side == Side::Sell; }
};

} // namespace mmfx
