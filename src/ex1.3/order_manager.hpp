#pragma once
// ============================================================
//  mm-fx — Exercice 1.3 : Transfert de propriete avec unique_ptr
// ============================================================
//
//  Objectif :
//    - Comprendre la semantique de propriete exclusive
//    - Observer que unique_ptr ne peut pas être copie
//    - Maîtriser le transfert de propriete avec std::move
//    - Voir le pattern de "sink function" (fonction qui prend
//      possession d'un unique_ptr)
// ============================================================

#include <mmfx/types.hpp>
#include <vector>
#include <memory>     // std::unique_ptr, std::make_unique
#include <stdexcept>  // std::out_of_range
#include <cstdio>
#include <optional>

namespace mmfx {

// ============================================================
//  OrderManager
//
//  Detient des Order via std::unique_ptr.
//  Chaque Order a exactement UN proprietaire a tout instant.
//
//  Analogie : unique_ptr est comme un ticket nominatif.
//    - On ne peut pas le photocopier (pas de copie).
//    - On peut le transferer a quelqu'un d'autre (move).
//    - Quand le ticket est detruit, l'objet est libere.
// ============================================================
class OrderManager {
public:
    explicit OrderManager(const char* name) : name_{name} {
        std::printf("[%s] OrderManager cree\n", name_);
    }

    ~OrderManager() {
        std::printf("[%s] OrderManager detruit (%zu ordres liberes)\n",
                    name_, orders_.size());
    }

    // OrderManager est non-copiable (unique_ptr l'impose implicitement)
    // On le rend explicite pour la clarte :
    OrderManager(const OrderManager&)            = delete;
    OrderManager& operator=(const OrderManager&) = delete;

    // Movable : on peut transferer un OrderManager entier
    OrderManager(OrderManager&&)            = default;
    OrderManager& operator=(OrderManager&&) = default;

    // --------------------------------------------------------
    //  add() — "sink function" : prend possession de l'ordre
    //
    //  Signature : unique_ptr<Order> passe par valeur.
    //  Cela force l'appelant a explicitement std::move son ptr
    //  -> intention claire dans le code appelant.
    //
    //  INTERDIT : add(const unique_ptr<Order>&) n'apporte rien
    //  car on ne peut pas transferer la propriete par const ref.
    // --------------------------------------------------------
    void add(std::unique_ptr<Order> order) {
        if (!order) {
            std::puts("  [WARN] tentative d'ajout d'un nullptr ignoree");
            return;
        }
        std::printf("[%s] Prise en charge de l'ordre #%llu\n",
                    name_,
                    static_cast<unsigned long long>(order->id));
        orders_.push_back(std::move(order));
    }

    // --------------------------------------------------------
    //  transfer() — transfere la propriete d'un ordre vers
    //  un autre OrderManager.
    //
    //  Apres transfer(), orders_[idx] est nullptr dans this.
    //  On retire l'entree pour ne pas garder de nullptr.
    // --------------------------------------------------------
    std::unique_ptr<Order> transfer(std::size_t idx) {
        if (idx >= orders_.size()) {
            throw std::out_of_range("transfer: index hors limites");
        }
        // Extraire le unique_ptr du vecteur
        auto ptr = std::move(orders_[idx]);
        // Supprimer l'emplacement devenu nullptr
        orders_.erase(orders_.begin() + static_cast<std::ptrdiff_t>(idx));

        std::printf("[%s] Transfert de l'ordre #%llu\n",
                    name_,
                    static_cast<unsigned long long>(ptr->id));
        return ptr;  // NRVO : 0 move supplementaire ici
    }

    // --------------------------------------------------------
    //  find() — acces non-proprietaire via un raw pointer
    //
    //  On retourne Order* (observateur) et non unique_ptr.
    //  Le proprietaire reste l'OrderManager.
    //  Retourner nullptr si non trouve.
    // --------------------------------------------------------
    const Order* find(uint64_t id) const noexcept {
        for (const auto& ptr : orders_) {
            if (ptr && ptr->id == id) return ptr.get();
        }
        return nullptr;
    }

    // --------------------------------------------------------
    //  Accesseurs
    // --------------------------------------------------------
    std::size_t size()       const noexcept { return orders_.size(); }
    const char* name()       const noexcept { return name_; }

    void print_orders() const {
        std::printf("[%s] %zu ordre(s) :\n", name_, orders_.size());
        for (const auto& ptr : orders_) {
            if (ptr) {
                std::printf("  #%llu price=%.4f qty=%d side=%s\n",
                            static_cast<unsigned long long>(ptr->id),
                            ptr->price, ptr->qty,
                            to_string(ptr->side));
            }
        }
    }

private:
    const char*                          name_;
    std::vector<std::unique_ptr<Order>>  orders_;
};

} // namespace mmfx
