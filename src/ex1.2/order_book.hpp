#pragma once
// ============================================================
//  mm-fx — Exercice 1.2 : OrderBook — copie ou mouvement ?
// ============================================================
//
//  Objectif : comprendre concretement quand une copie ou un
//  move est declenche lors d'une insertion dans un conteneur.
//
//  On utilise un type "instrumente" qui affiche ce qui se passe.
// ============================================================

#include <mmfx/types.hpp>
#include <vector>
#include <cstdio>    // puts — plus rapide que cout pour du debug

namespace mmfx {

// ============================================================
//  InstrumentedOrder
//  Même structure qu'Order, mais avec des constructeurs qui
//  affichent leur activite. Utile uniquement pour l'analyse.
// ============================================================
struct InstrumentedOrder {
    uint64_t id    {0};
    Price    price {0.0};
    Quantity qty   {0};
    Side     side  {Side::Buy};

    // ----- Constructeur utilisateur -----
    InstrumentedOrder(uint64_t id_, Price p, Quantity q, Side s) noexcept
        : id{id_}, price{p}, qty{q}, side{s}
    {
        std::printf("  [%llu] ctor         (construction directe)\n",
                    static_cast<unsigned long long>(id));
    }

    // ----- Destructeur -----
    ~InstrumentedOrder() {
        std::printf("  [%llu] ~dtor\n",
                    static_cast<unsigned long long>(id));
    }

    // ----- Copy constructor -----
    InstrumentedOrder(const InstrumentedOrder& o)
        : id{o.id}, price{o.price}, qty{o.qty}, side{o.side}
    {
        std::printf("  [%llu] COPY ctor     <-copie\n",
                    static_cast<unsigned long long>(id));
    }

    // ----- Copy assignment -----
    InstrumentedOrder& operator=(const InstrumentedOrder& o) {
        id = o.id; price = o.price; qty = o.qty; side = o.side;
        std::printf("  [%llu] COPY assign  <- copie\n",
                    static_cast<unsigned long long>(id));
        return *this;
    }

    // ----- Move constructor -----
    // noexcept : INDISPENSABLE pour que vector utilise le move
    InstrumentedOrder(InstrumentedOrder&& o) noexcept
        : id{o.id}, price{o.price}, qty{o.qty}, side{o.side}
    {
        o.id = 0;  // laisser l'objet source dans un etat valide
        std::printf("  [%llu] MOVE ctor   <- mouvement\n",
                    static_cast<unsigned long long>(id));
    }

    // ----- Move assignment -----
    InstrumentedOrder& operator=(InstrumentedOrder&& o) noexcept {
        id = o.id; price = o.price; qty = o.qty; side = o.side;
        o.id = 0;
        std::printf("  [%llu] MOVE assign   <- mouvement\n",
                    static_cast<unsigned long long>(id));
        return *this;
    }
};

// ============================================================
//  OrderBook
//
//  Deux methodes d'ajout pour comparer le comportement :
//
//  1. add_by_const_ref(const Order&)
//     → toujours une COPIE dans le vecteur
//        Avantage : simple, sûr
//        Inconvenient : copie même si l'appelant ne reutilise pas l'objet
//
//  2. add_by_value(Order)
//     → si lvalue passee : 1 COPIE (a l'appel) + 1 MOVE (dans le vecteur)
//        si rvalue passee : 1 MOVE (a l'appel) + 1 MOVE (dans le vecteur)
//        Idiome recommande pour "sink parameters" (parametres que la
//        fonction va posseder).
//
//  Note sur emplace_back :
//     La methode la plus efficace est emplace_back(args...) qui
//     construit l'objet directement dans le vecteur (0 copie, 0 move).
// ============================================================
class OrderBook {
public:
    // --------------------------------------------------------
    //  Version 1 : parametre par const ref
    //  Declenchement : push_back appelle le COPY constructor
    // --------------------------------------------------------
    void add_by_const_ref(const InstrumentedOrder& o) {
        orders_.push_back(o);   // copy
    }

    // --------------------------------------------------------
    //  Version 2 : parametre par valeur + std::move
    //
    //  o est deja une copie ou un move de l'argument original.
    //  On le move ensuite dans le vecteur — pas de seconde copie.
    // --------------------------------------------------------
    void add_by_value(InstrumentedOrder o) {
        orders_.push_back(std::move(o));  // move dans le vecteur
    }

    // --------------------------------------------------------
    //  Version 3 (bonus) : emplace_back — construction directe
    //  Le constructeur est appele avec les arguments directement
    //  dans la memoire du vecteur → 0 copie, 0 move.
    // --------------------------------------------------------
    void add_emplace(uint64_t id, Price p, Quantity q, Side s) {
        orders_.emplace_back(id, p, q, s);  // construction sur place
    }

    std::size_t size() const noexcept { return orders_.size(); }

    // Reserve evite les reallocations (et donc les moves de reallocation)
    void reserve(std::size_t n) { orders_.reserve(n); }

private:
    std::vector<InstrumentedOrder> orders_;
};

} // namespace mmfx
