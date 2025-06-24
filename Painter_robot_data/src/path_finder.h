#pragma once

#include <vector>
#include <iostream>
#include <climits>

struct Point {
    int x, y;
    Point(int x_in, int y_in) : x(x_in), y(y_in) {};
};

class PathFinder {
  public:
    int best_path_score = INT_MAX;
    std::vector<Point> best_path;
    void PrintPath(std::vector<Point> path);
    bool IsFullyDiscovered(std::vector<std::vector<int>>);
    std::vector<Point> FindPath(std::vector<std::vector<int>> state, Point current, int score);
    int GetStepScore(std::vector<Point> path);
    bool FindPath(std::vector<std::vector<int>> state, Point current, int score, std::vector<Point> path);
};