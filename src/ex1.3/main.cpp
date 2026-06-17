// ============================================================
//  mm-fx — Exercice 1.3 : main — transfert de propriete
//  Compilation : g++ -std=c++17 -Wall -Wextra -o ex1.3 main.cpp
// ============================================================

#include "order_manager.hpp"
#include <iostream>

using namespace mmfx;

void sep(const char* title) {
    std::printf("\n-------------------------------\n");
    std::printf("  %s\n", title);
    std::printf("---------------------------------\n");
}

int main() {

    // --------------------------------------------------------
    //  1. Creation d'ordres avec make_unique
    //
    //  Toujours preferer make_unique e new :
    //    - Exception-safe (si le constructeur leve, pas de fuite)
    //    - Plus lisible
    //    - Interdit de passer un pointeur brut par erreur
    // --------------------------------------------------------
    sep("1. Creation d'ordres avec make_unique");

    // make_unique alloue Order sur le heap et en prend possession
    auto o1 = std::make_unique<Order>(1, 1.0850, 100'000, Side::Buy);
    auto o2 = std::make_unique<Order>(2, 1.0855, 50'000,  Side::Sell);
    auto o3 = std::make_unique<Order>(3, 1.0848, 200'000, Side::Buy);

    std::printf("o1.get() = %p (adresse de l'objet Order dans le heap)\n",
                static_cast<void*>(o1.get()));

    // --------------------------------------------------------
    //  2. Transfert vers un OrderManager (sink function)
    //
    //  std::move(o1) -> o1 devient nullptr apres le transfert.
    //  Le OrderManager est maintenant le seul proprietaire.
    // --------------------------------------------------------
    sep("2. Transfert de propriete vers manager_a");

    OrderManager manager_a{"Manager-A"};
    OrderManager manager_b{"Manager-B"};

    manager_a.add(std::move(o1));  // o1 -> manager_a
    manager_a.add(std::move(o2));  // o2 -> manager_a
    manager_a.add(std::move(o3));  // o3 -> manager_a

    // o1, o2, o3 sont maintenant nullptr !
    std::printf("o1 apres move : %s\n", o1 ? "valide" : "nullptr ✓");
    std::printf("o2 apres move : %s\n", o2 ? "valide" : "nullptr ✓");

    manager_a.print_orders();

    // --------------------------------------------------------
    //  3. Tentative de copie -> ERREUR DE COMPILATION
    //
    //  Decommentez pour voir l'erreur :
    //    error: use of deleted function 'unique_ptr(const unique_ptr&)'
    // --------------------------------------------------------
    sep("3. Copie de unique_ptr -> impossible (commente)");

    /*
    auto o4 = std::make_unique<Order>(4, 1.09, 10000, Side::Buy);
    auto o5 = o4;                  // ERREUR : unique_ptr non copiable
    manager_b.add(o4);             // ERREUR : doit utiliser std::move
    */

    std::puts("  La copie d'un unique_ptr est interdite e la compilation.");
    std::puts("  -> Garantit qu'il n'y a jamais de double liberation.");

    // --------------------------------------------------------
    //  4. Transfert d'un ordre entre deux managers
    //
    //  manager_a cede l'ordre #2 e manager_b.
    //  Apres transfer(), manager_a n'a plus cet ordre.
    // --------------------------------------------------------
    sep("4. Transfert d'un ordre de manager_a vers manager_b");

    std::printf("Avant : manager_a=%zu ordres, manager_b=%zu ordres\n",
                manager_a.size(), manager_b.size());

    // transfer() retourne un unique_ptr — propriete transferee
    auto transferred = manager_a.transfer(1);  // index 1 = ordre #2
    manager_b.add(std::move(transferred));

    std::printf("Apres : manager_a=%zu ordres, manager_b=%zu ordres\n",
                manager_a.size(), manager_b.size());

    manager_a.print_orders();
    manager_b.print_orders();

    // --------------------------------------------------------
    //  5. Acces non-proprietaire via raw pointer (observateur)
    //
    //  find() retourne const Order* (pas unique_ptr).
    //  On observe l'objet sans en prendre la propriete.
    //  La duree de vie est geree par le manager.
    // --------------------------------------------------------
    sep("5. Observation sans propriete — raw pointer");

    const Order* obs = manager_a.find(1);
    if (obs) {
        std::printf("Ordre trouve via raw pointer : id=%llu price=%.4f\n",
                    static_cast<unsigned long long>(obs->id), obs->price);
        std::puts("  -> Le manager_a reste proprietaire. obs ne detruit rien.");
    }

    // --------------------------------------------------------
    //  6. Duree de vie automatique (RAII)
    //
    //  En sortant du scope main(), les managers sont detruits.
    //  Leurs unique_ptr liberent automatiquement chaque Order.
    //  Aucun delete manuel necessaire.
    // --------------------------------------------------------
    sep("6. Fin de main() -> destruction automatique (RAII)");
    std::puts("  Les managers vont sortir de portee...");

    // Destruction dans l'ordre inverse de declaration :
    // manager_b puis manager_a
    return 0;
}
