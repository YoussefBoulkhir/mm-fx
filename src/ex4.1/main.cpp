#include <iostream>
#include <memory>
#include <string>

// ============================================================
// Interface
// ============================================================

class IBeverage
{
public:
    virtual ~IBeverage() = default;

    virtual std::string description() const = 0;
    virtual double cost() const = 0;
};

// ============================================================
// Base drinks
// ============================================================

class Coffee : public IBeverage
{
public:
    std::string description() const override
    {
        return "Coffee";
    }

    double cost() const override
    {
        return 2.0;
    }
};

class Tea : public IBeverage
{
public:
    std::string description() const override
    {
        return "Tea";
    }

    double cost() const override
    {
        return 1.5;
    }
};

// ============================================================
// Decorator base
// ============================================================

class BeverageDecorator : public IBeverage
{
protected:
    std::shared_ptr<IBeverage> bev;

public:
    BeverageDecorator(std::shared_ptr<IBeverage> b)
        : bev(std::move(b)) {}
};

// ============================================================
// Concrete decorators
// ============================================================

class Milk : public BeverageDecorator
{
public:
    using BeverageDecorator::BeverageDecorator;

    std::string description() const override
    {
        return bev->description() + " + Milk";
    }

    double cost() const override
    {
        return bev->cost() + 0.3;
    }
};

class Sugar : public BeverageDecorator
{
public:
    using BeverageDecorator::BeverageDecorator;

    std::string description() const override
    {
        return bev->description() + " + Sugar";
    }

    double cost() const override
    {
        return bev->cost() + 0.2;
    }
};

class Cream : public BeverageDecorator
{
public:
    using BeverageDecorator::BeverageDecorator;

    std::string description() const override
    {
        return bev->description() + " + Cream";
    }

    double cost() const override
    {
        return bev->cost() + 0.5;
    }
};

class Caramel : public BeverageDecorator
{
public:
    using BeverageDecorator::BeverageDecorator;

    std::string description() const override
    {
        return bev->description() + " + Caramel";
    }

    double cost() const override
    {
        return bev->cost() + 0.7;
    }
};

// ============================================================
// Main
// ============================================================


// class IArticle{
//     int id_ ;
//     virtual double price() const = 0;
// };


// class Artilce: IArticle{
//     double price() override{

//     }
// }

// class ArticleDeco :public  IArticle{
//     protected : 
//         std::shared_ptr<IArticle> articl_ ; 

//     public : 
//         ArticleDeco(std::unique_ptr<IArticle> articl) : articl_(std::move(articl)){};

// };

// class promotion : public ArticleDeco{


//     public : 
//         using ArticleDeco::ArticleDeco;
//         double price() const override{
//             return 3;
//         }
// };

// class stock : public ArticleDeco{


//     public : 
//         using ArticleDeco::ArticleDeco;
//         double price() const override{
//             return 2;
//         }
// }

// class disponible : public ArticleDeco{


//     public : 
//         using ArticleDeco::ArticleDeco;
//         double price() const override{
//             return 5;
//         }
// };





// class CommandeService{
//     double buyArticle(int id ) {

//         return 0; 
//     }
//     // QUAND l'article est en promotion , on renvoie 3 
//     // quand l'article est en stock , on renvoie 2 
//     // quand l'article est disponible , on envoie 5 
//     // on a une classe qui permet de savoir dans quelle catégorie se trouve un article
    
//     // prendre en compte la possibilité d'avoir plusieurs fournisseurs de la classe qui permet de savoir dans quelle catégorie se trouve un article
    
// }
int main()
{

    
    std::shared_ptr<IBeverage> drink =
        std::make_shared<Coffee>();

    drink = std::make_shared<Milk>(drink);
    drink = std::make_shared<Sugar>(drink);
    drink = std::make_shared<Caramel>(drink);

    std::cout << drink->description() << "\n";
    std::cout << drink->cost() << "\n";


}