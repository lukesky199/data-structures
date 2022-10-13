/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::updateHeight(Node*& t) {
    if (t == nullptr) {
        return;
    }
    int leftHeight = heightOrNeg1(t->left);
    int rightHeight = heightOrNeg1(t->right);
    t->height = std::max(leftHeight, rightHeight) + 1;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node* newT = t->right;
    t->right = newT->left;
    newT->left = t;

    updateHeight(t);
    t = newT;
    updateHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node* newT = t->left;
    t->left = newT->right;
    newT->right = t;

    updateHeight(t);
    t = newT;
    updateHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
int AVLTree<K, V>::getBalance(Node*& t) {
    if (t == nullptr) {
        return 0;
    }
    return heightOrNeg1(t->right) - heightOrNeg1(t->left);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    if (subtree == nullptr) {
        return;
    }
    int balance = getBalance(subtree);
    if (balance == -2) {
        if (getBalance(subtree->left) == -1) {
            rotateRight(subtree);
        } else if (getBalance(subtree->left) == 1) {
            rotateLeftRight(subtree);
        }
    } else if (balance == 2) {
        if (getBalance(subtree->right) == -1) {
            rotateRightLeft(subtree);
        } else if (getBalance(subtree->right) == 1) {
            rotateLeft(subtree);
        }
    }
    updateHeight(subtree->left);
    updateHeight(subtree->right);
    updateHeight(subtree);
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if (subtree == nullptr) {
        subtree = new Node(key, value);
        return;
    }
    if (subtree->key == key) {
        return;
    }
    if (subtree->key > key) {
        insert(subtree->left, key, value);
    } else if (subtree->key < key) {
        insert(subtree->right, key, value);
    }
    rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::removeTwoChild(Node*& subtree, Node*& parent, Node* current) {
    if (current->right != nullptr) {
        removeTwoChild(subtree, current, current->right);
    } else {
        swap(subtree, current);
        if (current->left != nullptr) {
            if (parent == subtree) {
                parent->left = current->left;
            } else {
                parent->right = current->left;
            }
        } else {
            if (parent == subtree) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
        }
        std::cout << current << std::endl;
        delete current;
        current = nullptr;
    }
    rebalance(current);
    rebalance(parent);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        // your code here
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        // your code here
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            std::cout << "0 child remove: " << subtree->key << std::endl;
            delete subtree;
            subtree = nullptr;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
            std::cout << "2 child remove: " << subtree->key << std::endl;
            auto current = subtree->left;
            auto parent = subtree;
            removeTwoChild(subtree, parent, current);
        } else {
            /* one-child remove */
            // your code here
            std::cout << "1 child remove: " << subtree->key << std::endl;
            if (subtree->left != nullptr) {
                auto temp = subtree;
                subtree = subtree->left;
                delete temp;
            } else if (subtree->right != nullptr) {
                auto temp = subtree;
                subtree = subtree->right;
                delete temp;
            }
        }
        // your code here
    }
    rebalance(subtree);
}
