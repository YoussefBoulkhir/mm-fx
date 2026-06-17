#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <chrono>

/////////////////////////////////////////////////////////////
// 1. PROMISE / FUTURE SIMPLE
/////////////////////////////////////////////////////////////

void worker(std::promise<int> p)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));

    int result = 42;

    p.set_value(result);
}

void simpleExample()
{
    std::cout << "\n=== SIMPLE PROMISE/FUTURE ===\n";

    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t(worker, std::move(p));

    std::cout << "Main thread doing work...\n";

    int value = f.get(); // bloque ici

    std::cout << "Received value = " << value << "\n";

    t.join();
}

/////////////////////////////////////////////////////////////
// 2. SHARED FUTURE (MULTI CONSUMERS)
/////////////////////////////////////////////////////////////

void sharedWorker(std::promise<int> p)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));

    p.set_value(100);
}

void consumer(int id, std::shared_future<int> sf)
{
    int value = sf.get();

    std::cout << "Consumer " << id
              << " got value = "
              << value << "\n";
}

void sharedExample()
{
    std::cout << "\n=== SHARED FUTURE ===\n";

    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::shared_future<int> sf = f.share();

    std::thread producer(sharedWorker, std::move(p));

    std::vector<std::thread> consumers;

    for (int i = 0; i < 5; ++i)
    {
        consumers.emplace_back(consumer, i, sf);
    }

    producer.join();

    for (auto& t : consumers)
        t.join();
}

/////////////////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////////////////

int main()
{
    simpleExample();

    sharedExample();

    return 0;
}



// Avec promise t'as le controle de créer le thread que tu veux . Avec std::async C++ le crée pour toi 