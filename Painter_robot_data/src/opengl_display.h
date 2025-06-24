#pragma once

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <cmath>
#include "robot.h"

#include "path_finder.h"

class OpenGLDisplay
{
public:
    OpenGLDisplay();

    void main(std::string title, int width, int height, std::vector<Point> path);

private:
    // Display stuff
    void setup_opengl(int width, int height);
    void process_events();
    void display();

    void render_robot();
    void render_robot(float x, float y, float yaw, float r, float g, float b);
    void render_boundary();
    void render_window();

    void init_robot();
    void update_robot();

    void print_help();

private:
    static constexpr float ROBOT_RADIUS = 10;
    static constexpr float ROBOT_HEADING = 20;

    bool m_quit = false;
    int m_width;
    int m_height;

    SDL_Window *m_window = nullptr;
    SDL_GLContext m_context;
    uint32_t m_last_tick = 0;

    int m_yaw_vel_sp = 0;
    int m_vel_sp = 0;

    Robot m_robot;

    double m_dt = 0;
    std::vector<Point> follow_path;
};
