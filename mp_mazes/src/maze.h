/* Your code here! */
#include <vector>
#include "cs225/PNG.h"

using cs225::PNG;

class SquareMaze {
public:
    SquareMaze();
    void makeMaze (int width, int height);
    bool canTravel (int x, int y, int dir) const;
    void setWall (int x, int y, int dir, bool exists);
    std::vector<int> solveMaze ();
    PNG * drawMaze () const;
    PNG * drawMazeWithSolution ();
};