#include "IQuotingStrategy.hpp"

// Stratégie qui ajuste les prix selon l'inventaire
class InventoryAwareStrategy : public IQuotingStrategy {
public:
    Quote compute(double mid, double inventory) override {

        double spread = 0.0002;

        // Si inventory positif → on veut vendre → on baisse les prix
        double skew = inventory * 0.00001;

        return Quote{
            mid - spread / 2 - skew,
            mid + spread / 2 - skew
        };
    }
};