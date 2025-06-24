#include "path_finder.h"


bool PathFinder::IsFullyDiscovered(std::vector<std::vector<int>> input) {
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 0)
                return false;
        }
    }
    return true;
}

void PathFinder::PrintPath(std::vector<Point> path) {
    for (auto &i: path) {
        std::cout << i.x << " " << i.y << "\n";
    }
}

std::vector<Point> PathFinder::FindPath(std::vector<std::vector<int>> state, Point current, int score) {
    std::vector<Point> path;
    auto result = FindPath(state, current, 0, path);
    if (result == false) 
        std::cout << "No path found";
    return best_path;
}

int PathFinder::GetStepScore(std::vector<Point> path) {
    // Straight motion gets low score
    if (path.size() < 3) 
        return 3;
    // Motion along horizontal axis
    else if (path[path.size() - 1].x == path[path.size() - 2].x && 
             path[path.size() - 2].x == path[path.size() - 3].x)
        return 3;
    // Motion along vertical axis
    else if (path[path.size() - 1].y == path[path.size() - 2].y && 
             path[path.size() - 2].y == path[path.size() - 3].y)
        return 3;
    // L-shaped motion is penalized
    return 4; 
}

bool PathFinder::FindPath(std::vector<std::vector<int>> state, Point current, int score, std::vector<Point> path) {
    state[current.x][current.y] = 1;
    path.push_back(current);
    score += GetStepScore(path);
    if (IsFullyDiscovered(state) == true) {
        std::cout << "Found a valid path\n";
        if (score < best_path_score) {
            best_path_score = score;
            best_path = path;
        }
        return true;
    }
    if (score > best_path_score)
        return false;

    bool path_found = false;
    if (state[current.x + 1][current.y] == 0) {
        Point next(current.x + 1, current.y);
        path_found = path_found || FindPath(state, next, score, path);
    }
    if (state[current.x - 1][current.y] == 0) {
        Point next(current.x - 1, current.y);
        path_found = path_found || FindPath(state, next, score, path);
    }
    if (state[current.x][current.y + 1] == 0) {
        Point next(current.x, current.y + 1);
        path_found = path_found || FindPath(state, next, score, path);
    }
    if (state[current.x][current.y - 1] == 0) {
        Point next(current.x, current.y - 1);
        path_found = path_found || FindPath(state, next, score, path);
    }
    return path_found;
}