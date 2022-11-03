/* Your code here! */
#include <vector>

class DisjointSets {
public:
    DisjointSets() = default;
    void addelements (int num);
    int find (int elem);
    void setunion (int a, int b);
    int size (int elem);

private:
    std::vector<int> elems_;
};