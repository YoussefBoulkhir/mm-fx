#pragma once
#include "../threading/ThreadSafeQueue.hpp"
#include <iostream>

// Consomme les events et affiche
class MetricsLogger {
public:

    void run(ThreadSafeQueue<std::string>& q) {

        std::string msg;

        while (q.pop(msg)) {
            std::cout << "[LOG] " << msg << std::endl;
        }
    }
};