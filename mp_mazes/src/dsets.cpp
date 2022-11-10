/* Your code here! */
#include "dsets.h"
#include <iostream>

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++) {
        elems_.push_back(-1);
    }
}

int DisjointSets::find(int elem) {
    if (elems_[elem] < 0) {
        return elem;
    }
    elems_[elem] = find(elems_[elem]);
    return elems_[elem];
}

void DisjointSets::setunion (int a, int b) {
    int aRoot = find(a);
    int bRoot = find(b);
    if (aRoot == bRoot) {
        //bad union
        std::cout << "bad union" << std::endl;
        return;
    }
    if (elems_[aRoot] <= elems_[bRoot]) {
        elems_[aRoot] += elems_[bRoot];
        elems_[bRoot] = aRoot;
    } else {
        elems_[bRoot] += elems_[aRoot];
        elems_[aRoot] = bRoot;
    }

}

int DisjointSets::size(int elem) {
    return elems_[find(elem)] * -1;
}

