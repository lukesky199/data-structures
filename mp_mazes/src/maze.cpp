/* Your code here! */
#include "maze.h"
#include <cassert>

#include <stack>
#include <queue>
#include "dsets.h"

#include <random>
#include <algorithm>
#include <time.h>

#include <iostream>

SquareMaze::SquareMaze() {

}

int SquareMaze::getY(int val) {
    return val / width_;
}

int SquareMaze::getX(int val) {
    return val % width_ ;
}

int SquareMaze::getId(int x, int y) {
    return y * width_ + x;
}

void SquareMaze::makeMaze (int width, int height) {
    // Initialize Maze
    width_ = width;
    height_ = height;

    maze = std::vector<std::vector<MazeCell>>(width_, std::vector<MazeCell>(height_));
    int counter = 0;
    for (int row = 0; row < height_; row++) {
        for (int col = 0; col < width_; col++) {
            maze[col][row].id = row * width_ + col;
            // std::cout << maze[col][row].id << " ";
        }
        // std::cout << std::endl;
    }

    std::stack<int> stack;
    // srand(time(NULL));
    stack.push(rand() % (width_ * height_));
    DisjointSets set;
    set.addelements(width_ * height_);
    auto rng = std::default_random_engine {};
    // std::cout << "\n\n\n\n----------------------------------------------------------NEW MAZE" << std::endl;
    while (!stack.empty()) {
        int x = getX(stack.top());
        int y = getY(stack.top());
        stack.pop();
        // std::cout << "Current Coord: (" << x << ", " << y << ")" << std::endl;

        // Randomly Remove Walls
        std::vector<std::pair<MazeCell, int>> cells;
        // Right
        if (x < width_ - 1 && set.find(maze[x][y].id) != set.find(maze[x + 1][y].id)) {
            cells.push_back(std::pair<MazeCell, int>(maze[x + 1][y], 0));
        }
        // Down
        if (y < height_ - 1 && set.find(maze[x][y].id) != set.find(maze[x][y + 1].id)) {
            cells.push_back(std::pair<MazeCell, int>(maze[x][y + 1], 1));
        }
        // Left
        if (x > 0 && set.find(maze[x][y].id) != set.find(maze[x - 1][y].id)) {
            cells.push_back(std::pair<MazeCell, int>(maze[x - 1][y], 2));
        }
        // Up
        if (y > 0 && set.find(maze[x][y].id) != set.find(maze[x][y - 1].id)) {
            cells.push_back(std::pair<MazeCell, int>(maze[x][y - 1], 3));
        }
        std::shuffle(cells.begin(), cells.end(), rng);

        // Set Wall
        for (auto cell : cells) {
            if (set.find(maze[x][y].id) != set.find(cell.first.id)) {
                set.setunion(maze[x][y].id, cell.first.id);
                stack.push(cell.first.id);
                setWall(x, y, cell.second, false);
            }
        }
    }

    // TEST
    // setWall(0, 0, 1, false);
    /* for (int row = 0; row < height_; row++) {
        for (int col = 0; col < width_; col++) {
            std::cout << maze[col][row].id << ", " << maze[col][row].right << ", " << maze[col][row].down << " | ";
        }
        std::cout << std::endl;
    } */
}

bool SquareMaze::canTravel (int x, int y, int dir) const {
    // Right
    if (dir == 0) {
        if (x >= width_ - 1) {
            return false;
            assert(false); // Out of bounds
        }
        return !maze[x][y].right;
    }
    // Down
    if (dir == 1) {
        if (y >= height_ - 1) {
            return false;
            assert(false); // Out of bounds
        }
        return !maze[x][y].down;
    }
    // Left
    if (dir == 2) {
        if (x <= 0) {
            return false;
            assert(false); // Out of bounds
        }
        return !maze[x - 1][y].right;
    }
    // Up
    if (dir == 3) {
        if (y <= 0) {
            return false;
            assert(false); // Out of bounds
        }
        return !maze[x][y - 1].down;
    }
    assert(false); // Incorrect dir
}
void SquareMaze::setWall (int x, int y, int dir, bool exists) {
    // Right
    if (dir == 0) {
        if (x >= width_ - 1) {
            assert(false); // Out of bounds
        }
        maze[x][y].right = exists;
    }
    // Down
    if (dir == 1) {
        if (y >= height_ - 1) {
            assert(false); // Out of bounds
        }
        maze[x][y].down = exists;
    }
    // Left
    if (dir == 2) {
        if (x <= 0) {
            assert(false); // Out of bounds
        }
        maze[x - 1][y].right = exists;
    }
    // Up
    if (dir == 3) {
        if (y <= 0) {
            assert(false); // Out of bounds
        }
        maze[x][y - 1].down = exists;
    }
}

std::vector<int> SquareMaze::solveMaze () {
    int startX = -1;
    std::vector<std::vector<int>> prev(width_, std::vector<int>(height_, -1));
    std::vector<std::vector<int>> dist(width_, std::vector<int>(height_, 0));
    std::vector<std::vector<bool>> visited(width_, std::vector<bool>(height_, false));
    std::queue<int> queue;

    visited[0][0] = true;
    queue.push(0);
    while (!queue.empty()) {
        int x = getX(queue.front());
        int y = getY(queue.front());
        queue.pop();
        for (int dir = 0; dir < 4; dir++) {
            int nextX = x;
            int nextY = y;
            if (dir == 0) {
                nextX = x + 1;
            }
            if (dir == 1) {
                nextY = y + 1;
            }
            if (dir == 2) {
                nextX = x - 1;
            }
            if (dir == 3) {
                nextY = y - 1;
            }
            if (canTravel(x, y, dir) && !visited[nextX][nextY]) {
                if (dist[nextX][nextY] == 0 || dist[nextX][nextY] > dist[x][y] + 1) {
                    dist[nextX][nextY] = dist[x][y] + 1;
                    prev[nextX][nextY] = getId(x, y);
                }
                visited[nextX][nextY] = true;
                queue.push(getId(nextX, nextY));
            }
        }
    }
    /*for (auto vec : dist) {
        for (auto len : vec) {
            std::cout << len << " | ";
        }
        std::cout << std::endl;
    }*/
    int largestIdx = 0;
    for (int col = 0; col < width_; col++) {
        if (dist[col][height_ - 1] > dist[largestIdx][height_ - 1]) {
            largestIdx = col;
        }
    }
    std::vector<int> route;
    int x = largestIdx;
    int y = height_ - 1;
    int curr = dist[largestIdx][height_ - 1];
    while (getId(x, y) != 0) {
        int prevId = prev[x][y];
        // Right
        if (getX(prevId) == x - 1) {
            route.push_back(0);
            x--;
            continue;
        }
        // Down
        if (getY(prevId) == y - 1) {
            route.push_back(1);
            y--;
            continue;
        }
        // Left
        if (getX(prevId) == x + 1) {
            route.push_back(2);
            x++;
            continue;
        }
        // Up
        if (getY(prevId) == y + 1) {
            route.push_back(3);
            y++;
            continue;
        }
    }
    std::reverse(route.begin(), route.end());
    return route;
}

PNG * SquareMaze::drawMaze () const {
    PNG* img = new PNG(width_ * 10 + 1, height_ * 10 + 1);
    // Top Wall
    for (int x = 0; x < width_ * 10 + 1; x++) {
        if (x < 1 || x > 9) {
            img->getPixel(x, 0).l = 0;
        }
    }
    // Left Wall
    for (int y = 0; y < height_ * 10 + 1; y++) {
        img->getPixel(0, y).l = 0;
    }
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (maze[x][y].right) {
                for (int k = 0; k <= 10; k++) {
                    img->getPixel((x + 1) * 10, y * 10 + k).l = 0;
                }
            }
            if (maze[x][y].down) {
                for (int k = 0; k <= 10; k++) {
                    img->getPixel(x * 10 + k, (y + 1) * 10).l = 0;
                }
            }
        }
    }
    return img;
}
PNG * SquareMaze::drawMazeWithSolution () {
    PNG* img = drawMaze();
    std::vector<int> route = solveMaze();
    std::vector<std::pair<int, int>>  getDir{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    int x = 5;
    int y = 5;
    cs225::HSLAPixel red(0, 1, 0.5, 1);
    cs225::HSLAPixel white(0, 0, 1, 1);
    for (int dir : route) {
        for (int i = 0; i < 10; i++) {
            img->getPixel(x, y) = red;
            x += getDir[dir].first;
            y += getDir[dir].second;
        }
    }
    img->getPixel(x, y) = red;
    x = x/10;
    y = y/10;
    for (int k = 1; k < 10; k++) {
        img->getPixel(x * 10 + k, (y + 1) * 10) = white;
    }
    return img;
}