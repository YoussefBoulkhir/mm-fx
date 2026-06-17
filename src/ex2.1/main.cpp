#include <iostream>

#include "order_book_map.hpp"

using namespace mmfx;

int main()
{
    OrderBookMap book;

    book.addOrder(
        Order{
            1,
            100.25,
            100,
            Side::Buy});

    book.addOrder(
        Order{
            2,
            100.50,
            200,
            Side::Buy});

    book.addOrder(
        Order{
            3,
            101.00,
            150,
            Side::Sell});

    book.addOrder(
        Order{
            4,
            100.90,
            50,
            Side::Sell});

    book.print();

    std::cout
        << "Best Bid = "
        << book.best_bid()
        << '\n';

    std::cout
        << "Best Ask = "
        << book.best_ask()
        << '\n';

    std::cout
        << "\nCancel order 2\n";

    book.cancelOrder(2);

    book.print();

    std::cout
        << "Best Bid = "
        << book.best_bid()
        << '\n';

    std::cout
        << "Best Ask = "
        << book.best_ask()
        << '\n';

    return 0;
}