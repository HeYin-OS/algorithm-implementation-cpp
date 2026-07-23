#pragma once
#include <algorithm>
#include <vector>

template<typename T>
class AVL_Tree {
public:
    void insert(T val) {
        if (dummyHead.left == nullptr) {
            dummyHead.left = new Node(val);
        } else {
            _insert_recursive(dummyHead.left, val);
        }
    }

    std::vector<T> get_sorted_data() {
        std::vector<T> data;
        _middle_traverse(dummyHead.left, data);
        return std::move(data);
    }

private:
    struct Node {
        T val;
        Node *left, *right;
        long long height = 1;
        Node(T value, Node *l, Node* r): val(value), left(l), right(r) {}
        Node(T value): Node(value, nullptr, nullptr) {}
        Node(): Node(-1, nullptr, nullptr) {}
    };

    Node* _insert_recursive(Node* root, T val) {
        if (val < root->val) {
            if (root->left == nullptr) root->left = new Node(val);
            else {
                root->left = _insert_recursive(root->left, val);
            }
            
        } else {
            if (root->right == nullptr) root->right = new Node(val);
            else {
                root->right = _insert_recursive(root->right, val);
            }
        }
        return _check_ballance_and_rotate(root);
    }

    Node* _check_ballance_and_rotate(Node* root) {
        auto bf = _bf_of(root);
        Node* new_root = root;
        if (bf > 1) {
            if(_bf_of(root->left) >= 0) {
                new_root = _rotate_r(root);
            } else {
                root->left = _rotate_l(root->left);
                new_root = _rotate_r(root);
            }
        } else if (bf < -1) {
            if(_bf_of(root->right) <= 0) {
                new_root = _rotate_l(root);
            } else {
                root->right = _rotate_r(root->right);
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

    Node* _rotate_l(Node* root) {
        if (root == nullptr) return nullptr;
        Node* new_l_child = root;
        Node* new_root = root->right;
        new_l_child->right = new_root->left;
        new_root->left = new_l_child;
        _update_height(new_root);
        return new_root;
    }

    Node* _rotate_r(Node* root) {
        if (root == nullptr) return nullptr;
        Node* new_r_child = root;
        Node* new_root = root->left;
        new_r_child->left = new_root->right;
        new_root->right = new_r_child;
        _update_height(new_root);
        return new_root;
    }

    void _update_height(Node* root) {
        if (root == nullptr) return;
        auto lh = root->left == nullptr ? 0 : root->left->height;
        auto rh = root->right == nullptr ? 0 : root->right->height;
        root->height = std::max(lh, rh) + 1;
    }

    void _middle_traverse(Node* root, std::vector<T>& data) {
        if (root == nullptr) return;
        _middle_traverse(root->left, data);
        data.push_back(root->val);
        _middle_traverse(root->right, data);
    }

    // Use left to store root of AVL tree
    Node dummyHead;
};