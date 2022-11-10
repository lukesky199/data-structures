/* Your code here! */
#include <vector>
#include "cs225/PNG.h"

using cs225::PNG;

struct MazeCell {
    int id;
    // Default True/Has a wall
    bool right = true;
    // Default True/Has a wall
    bool down = true;
};

class SquareMaze {
public:
    SquareMaze();
    void makeMaze (int width, int height);
    bool canTravel (int x, int y, int dir) const;
    void setWall (int x, int y, int dir, bool exists);
    std::vector<int> solveMaze ();
    PNG * drawMaze () const;
    PNG * drawMazeWithSolution ();

private:
    int getY(int val);
    int getX(int val);
    int getId(int x, int y);
    std::vector<MazeCell> randCells(int x, int y);

    std::vector<std::vector<MazeCell>> maze;
    int width_;
    int height_;
};