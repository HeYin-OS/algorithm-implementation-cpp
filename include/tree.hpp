#pragma once
#include <algorithm>
#include <vector>

template<typename T>
class AVL_Tree {
public:
    // Insert val into AVL_Tree
    void insert(T val) {
        if (dummyHead.left == nullptr) {
            dummyHead.left = new Node(val);
        } else {
            _insert_recursive(dummyHead.left, val);
        }
    }

    // Traverse AVL_Tree to get sorted data
    std::vector<T> get_sorted_data() {
        std::vector<T> data;
        _middle_traverse(dummyHead.left, data);
        return std::move(data);
    }

private:
    struct Node {
        T val;
        Node *left, *right;
        // Height of leaf node is 1, instead null node height is 0
        long long height = 1;
        Node(T value, Node *l, Node* r): val(value), left(l), right(r) {}
        Node(T value): Node(value, nullptr, nullptr) {}
        Node(): Node(-1, nullptr, nullptr) {}
    };

    // Insert new value into tree while keeping the balance and auto-updating height 
    Node* _insert_recursive(Node* root, T val) {
        // Get into left tree
        if (val < root->val) {
            // No child on left
            if (root->left == nullptr) root->left = new Node(val);
            // Insert in left child
            else {
                root->left = _insert_recursive(root->left, val);
            }
        // Get into right tree
        } else {
            // No child on right
            if (root->right == nullptr) root->right = new Node(val);
            // Insert in right child
            else {
                root->right = _insert_recursive(root->right, val);
            }
        }
        // Perform balance adjustment on root
        return _check_balance_and_rotate(root);
    }

    // (AVL Tree core) Perform rotation on unbalanced tree, and update height value respectively.
    Node* _check_balance_and_rotate(Node* root) {
        // Get balance factor of root
        auto bf = _bf_of(root);
        Node* new_root = root;
        // Left tree weighs more
        if (bf > 1) {
            // Still heavy on left tree of root left child tree
            if(_bf_of(root->left) >= 0) {
                // Do only right rolation on root
                new_root = _rotate_r(root);
            // Behave as heavy on right tree of root left child tree
            } else {
                // Do left rolation first on root left chile tree
                root->left = _rotate_l(root->left);
                // Then do right rotation on root
                new_root = _rotate_r(root);
            }
        // Right tree weighs more
        } else if (bf < -1) {
            // Still heavy on right tree of root right child tree
            if(_bf_of(root->right) <= 0) {
                // Do only left rolation on root
                new_root = _rotate_l(root);
            // Behave as heavy on left tree of root right child tree
            } else {
                // Do right rolation first on root right chile tree
                root->right = _rotate_r(root->right);
                // Then do left rotation on root
                new_root = _rotate_l(root);
            }
        }
        return new_root;
    }

    // Calculate balance factor of root, bf = lh - rh
    long long _bf_of(Node* root) {
        if (root == nullptr) return 0;
        auto lh = root->left == nullptr ? 0 : root->left->height;
        auto rh = root->right == nullptr ? 0 : root->right->height;
        return lh - rh;
    }

    // Do left rotation on this node
    Node* _rotate_l(Node* root) {
        if (root == nullptr) return nullptr;
        Node* new_l_child = root;
        Node* new_root = root->right;
        new_l_child->right = new_root->left;
        new_root->left = new_l_child;
        _update_height(new_root);
        return new_root;
    }

    // Do right rotation on this node
    Node* _rotate_r(Node* root) {
        if (root == nullptr) return nullptr;
        Node* new_r_child = root;
        Node* new_root = root->left;
        new_r_child->left = new_root->right;
        new_root->right = new_r_child;
        _update_height(new_root);
        return new_root;
    }

    // Re-calculate height of this node
    void _update_height(Node* root) {
        if (root == nullptr) return;
        auto lh = root->left == nullptr ? 0 : root->left->height;
        auto rh = root->right == nullptr ? 0 : root->right->height;
        root->height = std::max(lh, rh) + 1;
    }

    // Do middle traversal to get ordered data
    void _middle_traverse(Node* root, std::vector<T>& data) {
        if (root == nullptr) return;
        _middle_traverse(root->left, data);
        data.push_back(root->val);
        _middle_traverse(root->right, data);
    }

    // Use left to store root of AVL tree
    Node dummyHead;
};