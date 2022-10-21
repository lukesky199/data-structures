/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] == second[curDim]) {
      return first < second;
    }
    return first[curDim] < second[curDim];
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    double currDist = 0, potDist = 0;

    for (unsigned i = 0; i < Dim; i++) {
      currDist += (currentBest[i] - target[i]) * (currentBest[i] - target[i]);
      potDist += (potential[i] - target[i]) * (potential[i] - target[i]);
    }
    // std::cout << potDist << ", " << currDist << std::endl;
    if (potDist == currDist) {
      return potential < currentBest;
    }
    return potDist < currDist;
}

template <int Dim>
size_t KDTree<Dim>::partition(vector<Point<Dim>>& newPoints, size_t dim, int left, int right, int pivotIndex) {
  Point<Dim> pivotValue = newPoints[pivotIndex];
  std::swap(newPoints[pivotIndex], newPoints[right]);

  size_t storeIndex = left;
  for (int i = left; i < right; i++) {
    if (newPoints[i][dim] < pivotValue[dim] || (newPoints[i][dim] == pivotValue[dim] && newPoints[i] < pivotValue)) {
      std::swap(newPoints[storeIndex], newPoints[i]);
      storeIndex++;
    }
  }
  std::swap(newPoints[storeIndex], newPoints[right]);

  return storeIndex;
}

template <int Dim>
Point<Dim> KDTree<Dim>::quickselect(vector<Point<Dim>>& newPoints, size_t dim, int left, int right, int k) {
  while (true) {
    if (left >= right) {
      return newPoints[left];
    }
    int pivotIndex = (left + right) / 2;
    pivotIndex = partition(newPoints, dim, left, right, pivotIndex);
    if (k == pivotIndex) {
      return newPoints[k];
    } else if (k < pivotIndex) {
      right = pivotIndex - 1;
    } else {
      left = pivotIndex + 1;
    }
  }
}

template <int Dim>
void KDTree<Dim>::buildTree(vector<Point<Dim>>& newPoints, size_t dim, int left, int right, KDTreeNode*& curRoot) {
  if (left <= right) {
    size_t middle = (left + right) / 2;

    // TESTING
    /*for (auto point : newPoints) {
      std::cout << point << ", ";
    }
    std::cout << std::endl;*/

    Point<Dim> midVal = quickselect(newPoints, dim, left, right, middle);

    // TESTING
    /*std::cout << "DIMENSION: " << dim << std::endl;
    std::cout << "LEFT: " << left << ", RIGHT: " << right << std::endl;
    for (auto point : newPoints) {
      std::cout << point[dim] << ", ";
    }
    std::cout << std::endl;
    std::cout << midVal << std::endl;*/

    curRoot = new KDTreeNode(midVal);
    buildTree(newPoints, (dim + 1) % Dim, left, middle - 1, curRoot->left);
    buildTree(newPoints, (dim + 1) % Dim, middle + 1, right, curRoot->right);
  }
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    vector<Point<Dim>>tempNewPoints(newPoints);
    buildTree(tempNewPoints, 0, 0, newPoints.size() - 1, root);
    size = newPoints.size();
    // printTree(cout);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy(KDTreeNode* otherSubroot) {
  if (otherSubroot == nullptr) {
    return nullptr;
  }
  KDTree* ret = new KDTreeNode(otherSubroot->point);
  ret->right = copy(otherSubroot->right);
  ret->left = copy(otherSubroot->left);
  return ret;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  copy(other.root);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  clear(root);

  copy(rhs.root);

  return *this;
}

template <int Dim>
void KDTree<Dim>::clear(KDTreeNode* subroot) {
  if (subroot == nullptr) {
    return;
  }
  clear(subroot->left);
  clear(subroot->right);
  delete subroot;
  subroot = nullptr;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  clear(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query, size_t dim, KDTreeNode* curRoot) const {
  if (curRoot->left == nullptr && curRoot->right == nullptr) {
    return curRoot->point;
  }
  Point<Dim> nearest;
  bool recursedLeft = false;
  if (query == curRoot->point) {
    return curRoot->point;
  }
  if (query[dim] < curRoot->point[dim]) {
    if (curRoot->left != nullptr) {
      nearest = findNearestNeighbor(query, (dim + 1) % Dim, curRoot->left);
      recursedLeft = true;
    }
  } else {
    if (curRoot->right != nullptr) {
      nearest = findNearestNeighbor(query, (dim + 1) % Dim, curRoot->right);
    }
  }
  if (shouldReplace(query, nearest, curRoot->point)) {
    nearest = curRoot->point;
  }

  double radius = 0;
  for (unsigned i = 0; i < Dim; i++) {
    radius += (query[i] - nearest[i]) * (query[i] - nearest[i]);
  }
  size_t splitDist = (curRoot->point[dim] - query[dim]) * (curRoot->point[dim] - query[dim]);

  if (radius >= splitDist) {
    Point<Dim> tempNearest;
    if (recursedLeft || curRoot->left == nullptr) {
      tempNearest = findNearestNeighbor(query, (dim + 1) % Dim, curRoot->right);
    } else {
      tempNearest = findNearestNeighbor(query, (dim + 1) % Dim, curRoot->left);
    }
    if (shouldReplace(query, nearest, tempNearest)) {
      nearest = tempNearest;
    }
  }
  return nearest;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */

    return findNearestNeighbor(query, 0, root);
}

