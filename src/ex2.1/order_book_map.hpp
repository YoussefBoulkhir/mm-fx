#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include <mmfx/types.hpp>

namespace mmfx {

class OrderBookMap
{
public:
    using OrderId = uint64_t;

    void addOrder(const Order& order)
    {
        if (order.is_buy())
        {
            bids_[order.price].push_back(order);
        }
        else
        {
            asks_[order.price].push_back(order);
        }

        index_[order.id] = {order.side, order.price};
    }

    bool cancelOrder(OrderId id)
    {
        auto idx = index_.find(id);

        if (idx == index_.end())
        {
            return false;
        }

        const auto [side, price] = idx->second;

        auto remove_from_book =
            [&](auto& book)
            {
                auto level_it = book.find(price);

                if (level_it == book.end())
                {
                    return false;
                }

                auto& orders = level_it->second;

                auto order_it =
                    std::find_if(
                        orders.begin(),
                        orders.end(),
                        [id](const Order& o)
                        {
                            return o.id == id;
                        });

                if (order_it == orders.end())
                {
                    return false;
                }

                orders.erase(order_it);

                if (orders.empty())
                {
                    book.erase(level_it);
                }

                return true;
            };

        bool removed =
            (side == Side::Buy)
                ? remove_from_book(bids_)
                : remove_from_book(asks_);

        if (removed)
        {
            index_.erase(idx);
        }

        return removed;
    }

    Price best_bid() const
    {
        if (bids_.empty())
        {
            return 0.0;
        }

        return bids_.rbegin()->first;
    }

    Price best_ask() const
    {
        if (asks_.empty())
        {
            return 0.0;
        }

        return asks_.begin()->first;
    }

    void print() const
    {
        std::cout << "\n===== ORDER BOOK =====\n";

        std::cout << "\nASKS\n";

        for (const auto& [price, orders] : asks_)
        {
            std::cout
                << price
                << " -> "
                << orders.size()
                << " order(s)\n";
        }

        std::cout << "\nBIDS\n";

        for (auto it = bids_.rbegin(); it != bids_.rend(); ++it)
        {
            std::cout
                << it->first
                << " -> "
                << it->second.size()
                << " order(s)\n";
        }

        std::cout << '\n';
    }

private:
    std::map<Price, std::vector<Order>> bids_;
    std::map<Price, std::vector<Order>> asks_;

    // optimisation de recherche par id
    std::unordered_map<OrderId, std::pair<Side, Price>> index_;
};

} // namespace mmfx