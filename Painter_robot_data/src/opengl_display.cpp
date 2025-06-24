#include "opengl_display.h"
#include <iostream>
#include <GL/glu.h>
#include <unistd.h>

#include "config.h"

using namespace std;

OpenGLDisplay::OpenGLDisplay() {}

void OpenGLDisplay::main(std::string title, int width, int height, std::vector<Point> path)
{
    follow_path = path;
    m_width = width;
    m_height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr <<  "Video initialization failed: " << SDL_GetError() << endl;
        return;
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (m_window == NULL) {
        cerr << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        return;
    }

    //Create context
    m_context = SDL_GL_CreateContext(m_window);
    if (m_context == NULL) {
        cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
        return;
    }

    setup_opengl(width, height);

    print_help();

    init_robot();

    m_last_tick = SDL_GetTicks();

    while (!m_quit) {
        m_dt = (SDL_GetTicks() - m_last_tick) / 1000.0;

        process_events();
        update_robot();
        display();

        m_last_tick = SDL_GetTicks();

        usleep(10000);
    }
}

void OpenGLDisplay::setup_opengl(int width, int height)
{
    // for 2d drawing
    glDepthMask(GL_FALSE);  // disable writes to Z-Buffer
    glDisable(GL_DEPTH_TEST);

    // for font
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // for transparent texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLDisplay::process_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    m_quit = true;
                }
                break;
            }
        }
    }
}

void OpenGLDisplay::display()
{
    glViewport(0, 0, m_width, m_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, m_width, 0, m_height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_boundary();
    render_window();

    render_robot();

    SDL_GL_SwapWindow(m_window);
}

void OpenGLDisplay::init_robot()
{
    m_robot.x(100);
    m_robot.y(100);
    m_robot.yaw(M_PI_2);

}

void OpenGLDisplay::render_robot(float x, float y, float yaw, float r, float g, float b)
{
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(x - 50, y - 50);
    glVertex2f(x - 50, y + 50);
    glVertex2f(x + 50, y + 50);
    glVertex2f(x + 50, y - 50);
    glEnd();

    glColor3f(r, g, b);

    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_LINE_LOOP);
    for (float a=0; a < 360; a += 10) {
        float _x = x + ROBOT_RADIUS*cos(a*M_PI/180);
        float _y = y + ROBOT_RADIUS*sin(a*M_PI/180);

        glVertex2f(_x, _y);
    }
    glEnd();

    // heading arrow
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + ROBOT_HEADING*cos(yaw), y + ROBOT_HEADING*sin(yaw));
    glEnd();

    glPopMatrix();
}

void OpenGLDisplay::render_robot()
{
    render_robot(m_robot.x(), m_robot.y(), m_robot.yaw(), 0, 1, 0);

    glColor3f(0, 1, 0);

    glPopMatrix();
}

void OpenGLDisplay::render_boundary()
{
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(BOUNDARY_X1, BOUNDARY_Y1);
    glVertex2f(BOUNDARY_X2, BOUNDARY_Y1);
    glVertex2f(BOUNDARY_X2, BOUNDARY_Y2);
    glVertex2f(BOUNDARY_X1, BOUNDARY_Y2);
    glEnd();

    glPopMatrix();
}

void OpenGLDisplay::render_window()
{
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(WINDOW_X1, WINDOW_Y1);
    glVertex2f(WINDOW_X2, WINDOW_Y1);
    glVertex2f(WINDOW_X2, WINDOW_Y2);
    glVertex2f(WINDOW_X1, WINDOW_Y2);
    glEnd();

    glPopMatrix();
}

void OpenGLDisplay::update_robot()
{
    double prev_x = m_robot.x();
    double prev_y = m_robot.y();

    if (follow_path.size() > 0) {
        double target_x = static_cast<double>(follow_path[0].x * 100);
        double target_y = static_cast<double>(follow_path[0].y * 100);
        // std::cout << "target x: " << target_x << "\n";
        // std::cout << "target y: " << target_y << "\n";

        if (std::sqrt(std::pow(target_x - m_robot.x(), 2) + std::pow(target_y - m_robot.y(), 2)) < 10) {
            follow_path.erase(follow_path.begin());
            return;
        }
        double delta_x = (target_x - m_robot.x());
        double delta_y = (target_y - m_robot.y());

        if (delta_x > 0.0)
            delta_x = 3.0;
        else if (delta_x < 0.0)
            delta_x = -3.0;
        if (delta_y > 0.0)
            delta_y = 3.0;
        else if (delta_y < 0.0)
            delta_y = -3.0;

        m_robot.x(m_robot.x() + delta_x);
        m_robot.y(m_robot.y() + delta_y);
    }
        
    // enforce boundary
    if (m_robot.x() < BOUNDARY_X1 ||
        m_robot.y() < BOUNDARY_Y1 ||
        m_robot.x() > BOUNDARY_X2 ||
        m_robot.y() > BOUNDARY_Y2) {

        m_robot.x(prev_x);
        m_robot.y(prev_y);
        m_robot.vel(0);
    }

    // enforce window
    if (m_robot.x() > WINDOW_X1 &&
        m_robot.y() > WINDOW_Y1 &&
        m_robot.x() < WINDOW_X2 &&
        m_robot.y() < WINDOW_Y2) {

        m_robot.x(prev_x);
        m_robot.y(prev_y);
        m_robot.vel(0);
    }

}

void OpenGLDisplay::print_help()
{
    cout << "Robot will do the painting autonomously" << endl;
    cout << "Press escape to quit" << endl;
}
