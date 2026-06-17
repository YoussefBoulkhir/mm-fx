#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

struct Node
{
    std::string name;
    bool isFile;
    std::size_t size;

    std::vector<std::shared_ptr<Node>> children;
};

// ============================================================
// DFS récursif
// ============================================================

void dfs_recursive(const std::shared_ptr<Node>& node)
{
    if (!node)
    {
        return;
    }

    std::cout << node->name << '\n';

    for (const auto& child : node->children)
    {
        dfs_recursive(child);
    }
}

// ============================================================
// DFS itératif
// ============================================================

void dfs_iterative(const std::shared_ptr<Node>& root)
{
    if (!root)
    {
        return;
    }

    std::stack<std::shared_ptr<Node>> st;
    st.push(root);

    while (!st.empty())
    {
        auto current = st.top();
        st.pop();

        std::cout << current->name << '\n';

        // On parcourt à l'envers pour conserver
        // le même ordre que la version récursive.
        for (auto it = current->children.rbegin();
             it != current->children.rend();
             ++it)
        {
            st.push(*it);
        }
    }
}

// ============================================================
// BFS
// ============================================================

void bfs(const std::shared_ptr<Node>& root)
{
    if (!root)
    {
        return;
    }

    std::queue<std::shared_ptr<Node>> q;
    q.push(root);

    while (!q.empty())
    {
        auto current = q.front();
        q.pop();

        std::cout << current->name << '\n';

        for (const auto& child : current->children)
        {
            q.push(child);
        }
    }
}

// ============================================================
// Calcul taille totale
// ============================================================

std::size_t compute_size(const std::shared_ptr<Node>& node)
{
    if (!node)
    {
        return 0;
    }

    if (node->isFile)
    {
        return node->size;
    }

    std::size_t total = 0;

    for (const auto& child : node->children)
    {
        total += compute_size(child);
    }

    return total;
}

// ============================================================
// Recherche par extension
// ============================================================

bool has_extension(
    const std::string& filename,
    const std::string& extension)
{
    if (filename.size() < extension.size())
    {
        return false;
    }

    return filename.compare(
               filename.size() - extension.size(),
               extension.size(),
               extension) == 0;
}

void find_extension(
    const std::shared_ptr<Node>& node,
    const std::string& extension)
{
    if (!node)
    {
        return;
    }

    if (node->isFile &&
        has_extension(node->name, extension))
    {
        std::cout << node->name << '\n';
    }

    for (const auto& child : node->children)
    {
        find_extension(child, extension);
    }
}

// ============================================================
// Main
// ============================================================

int main()
{
    //
    // root
    // ├── src
    // │   ├── main.cpp
    // │   ├── engine.cpp
    // │   └── orderbook.cpp
    // │
    // ├── include
    // │   ├── types.hpp
    // │   └── orderbook.hpp
    // │
    // └── docs
    //     └── readme.md
    //

    auto root =
        std::make_shared<Node>(
            Node{"root", false, 0});

    auto src =
        std::make_shared<Node>(
            Node{"src", false, 0});

    auto include =
        std::make_shared<Node>(
            Node{"include", false, 0});

    auto docs =
        std::make_shared<Node>(
            Node{"docs", false, 0});

    auto main_cpp =
        std::make_shared<Node>(
            Node{"main.cpp", true, 120});

    auto engine_cpp =
        std::make_shared<Node>(
            Node{"engine.cpp", true, 250});

    auto orderbook_cpp =
        std::make_shared<Node>(
            Node{"orderbook.cpp", true, 180});

    auto types_hpp =
        std::make_shared<Node>(
            Node{"types.hpp", true, 80});

    auto orderbook_hpp =
        std::make_shared<Node>(
            Node{"orderbook.hpp", true, 95});

    auto readme =
        std::make_shared<Node>(
            Node{"readme.md", true, 50});

    src->children.push_back(main_cpp);
    src->children.push_back(engine_cpp);
    src->children.push_back(orderbook_cpp);

    include->children.push_back(types_hpp);
    include->children.push_back(orderbook_hpp);

    docs->children.push_back(readme);

    root->children.push_back(src);
    root->children.push_back(include);
    root->children.push_back(docs);

    std::cout
        << "============================\n"
        << "DFS Recursive\n"
        << "============================\n";

    dfs_recursive(root);

    std::cout
        << "\n============================\n"
        << "DFS Iterative\n"
        << "============================\n";

    dfs_iterative(root);

    std::cout
        << "\n============================\n"
        << "BFS\n"
        << "============================\n";

    bfs(root);

    std::cout
        << "\n============================\n"
        << "Find .cpp files\n"
        << "============================\n";

    find_extension(root, ".cpp");

    std::cout
        << "\n============================\n"
        << "Total size\n"
        << "============================\n";

    std::cout
        << compute_size(root)
        << " bytes\n";

    return 0;
}