/*
 Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root. Выведите элементы в порядке in-order (слева направо).
 Рекурсия запрещена.
 */

#include <iostream>
#include <stack>
#include <functional>

struct Node {
    explicit Node(int value) : value(value), left(nullptr), right(nullptr) {}
    
    int value;
    Node* left;
    Node* right;
};

class Tree {
public:
    void add(int value) {
        if (!root) {
            root = new Node(value);
            return;
        }
        Node* current = root;
        while (true) {
            if (value < current->value) {
                if (current->left) {
                    current = current->left;
                }
                else {
                    current->left = new Node(value);
                    break;
                }
            } else {
                if (current->right) {
                    current = current->right;
                }
                else {
                    current->right = new Node(value);
                    break;
                }
            }
        }
    }
    
    void inorder_traversal(std::function<void (Node*)>&& func) {
        if (!root) {
            return;
        }
        Node* current = root;

        std::stack<Node*> backpath;
        
        while (current) {
            backpath.push(current);
            current = current->left;
        }
        
        while (!backpath.empty()) {
            current = backpath.top();
            backpath.pop();
            Node* right = current->right;
            func(current);
            if (right) {
                current = right;
                while (current) {
                    backpath.push(current);
                    current = current->left;
                }
            }
        }
    }
    
    ~Tree() {
        inorder_traversal([](Node* node) {delete node;});
    }
private:
    Node* root = nullptr;
};

int main(int argc, const char * argv[]) {
    size_t N;
    std::cin >> N;
    
    Tree tree;
    for (size_t i = 0; i < N; ++i) {
        int value;
        std::cin >> value;
        tree.add(value);
    }
    
    bool first = true;
    tree.inorder_traversal([&first](Node* node) {
        if (!first) {
            std::cout << ' ';
        }
        first = false;
        std::cout << node->value;
        
    });
    return 0;
}
