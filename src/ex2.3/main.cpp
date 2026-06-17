#include <algorithm>
#include <iostream>
#include <vector>

#include "mmfx/types.hpp"

using namespace mmfx;

void printOrders(const std::vector<Order>& orders)
{
    std::cout << "\nOrders\n";

    for (const auto& o : orders)
    {
        std::cout
            << "id="
            << o.id
            << " price="
            << o.price
            << " qty="
            << o.qty
            << " side="
            << to_string(o.side)
            << '\n';
    }
}

int main()
{
    std::vector<Order> orders{
        {1, 100.10, 100, Side::Buy},
        {2, 100.20,   0, Side::Buy},
        {3, 100.30, 250, Side::Sell},
        {4, 100.40,   0, Side::Sell},
        {5, 100.50, 500, Side::Buy}
    };

    std::cout << "Before cleanup\n";

    printOrders(orders);

    orders.erase(
        std::remove_if(
            orders.begin(),
            orders.end(),
            [](const Order& o)
            {
                return o.qty == 0;
            }),
        orders.end());

    std::cout << "\nAfter cleanup\n";

    printOrders(orders);

    return 0;
}