/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    // Your code here
    if (find(root, key) == nullptr) {
        return V();
    }
    return find(root, key)->value;
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    // Your code here
    if (subtree == nullptr) {
        return subtree;
    }
    if (subtree->key == key) {
        return subtree;
    }
    if (subtree->key > key) {
        return find(subtree->left, key);
    }
    else {
        return find(subtree->right, key);
    }
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    // your code here
    insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
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
    }
    else {
        insert(subtree->right, key, value);
    }
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    if (first == nullptr || second == nullptr) {
        std::cout << "BST<K, V>::swap(Node*& first, Node*& second) : first or second is nullptr" << std::endl;
        return;
    }
    std::swap(first->key, second->key);
    std::swap(first->value, second->value);
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    // your code here
    remove(root, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    auto& delNode = find(subtree, key);
    if (delNode == nullptr) {
        return;
    }

    if (delNode->left == nullptr && delNode->right == nullptr) {
        delete delNode;
        delNode = nullptr;
        return;
    }

    if (delNode->right == nullptr) { //Left one child
        auto temp = delNode;
        delNode = delNode->left;
        delete temp;
        return;
    }

    if (delNode->left == nullptr) { //Right one child
        auto temp = delNode;
        delNode = delNode->right;
        delete temp;
        return;
    }

    // Two children
    auto current = delNode->left;
    auto parent = delNode;
    while (current->right != nullptr) {
        parent = current;
        current = current->right;
    }
    swap(delNode, current);
    if (current->left != nullptr) {
        if (parent == delNode) {
            parent->left = current->left;
        } else {
            parent->right = current->left;
        }
    } else {
        parent->right = nullptr;
    }
    delete current;

}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K, V> ret;
    for (auto node : inList) {
        ret.insert(node.first, node.second);
    }
    return ret;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    std::vector<int> heights;
    std::sort(inList.begin(), inList.end());
    do {
        unsigned height = listBuild(inList).height();
        if (height >= heights.size()) {
            heights.resize(height + 1, 0);
        }
        heights[height]++;
    } while (std::next_permutation(inList.begin(), inList.end()));
    return heights;
}