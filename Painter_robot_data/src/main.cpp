#include "opengl_display.h"
#include "path_finder.h"
#include "config.h"

int main(int argc, char **argv)
{
    
    PathFinder path_finder;
    std::vector<std::vector<int>> map
    {
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };
    Point start(1, 1);
    auto path = path_finder.FindPath(map, start, 0);
    // path_finder.PrintPath(path);
    
    OpenGLDisplay display;

    display.main("Painter Robot", WORLD_WIDTH, WORLD_HEIGHT, path);

    return 0;
}
