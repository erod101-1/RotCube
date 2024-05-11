#include <iostream>
#include <vector>
#include <SDL.h>
#include <numeric>
#include <cstdlib>
#include <ctime>
#define pos1 100+50
#define pos2 200+50
using namespace std;
class Screen {
private:
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;
    Screen()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(
           400 * 2, 300 * 2, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 2, 2);
    }

public:

    static Screen& getInstance() {
        static Screen instance;
        return instance;
    }

    void pixel(float x, float y);
    void show();
    void input();
    void clear();
};

void Screen::pixel(float x, float y)
{
    SDL_FPoint point = { x, y };
    points.push_back(point);
}

void Screen::show()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto& point : points)
    {
        SDL_RenderDrawPoint(renderer, point.x, point.y);
    }
    SDL_RenderPresent(renderer);
}

void Screen::input()
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(0);
        }
    }
}

void Screen::clear()
{
    points.clear();
}

class Cube {
private:
    Screen& cScreen;
    struct vec3d { float x, y, z; };
    struct connections
    {
        int a, b;
    };
    std::vector<vec3d> points = {
        {pos1, pos1, pos1},
        {pos2, pos1, pos1},
        {pos2, pos2, pos1},
        {pos1, pos2, pos1},

        {pos1, pos1, pos2},
        {pos2, pos1, pos2},
        {pos2, pos2, pos2},
        {pos1, pos2, pos2},
    };
    std::vector<connections> connectMatrix
    {
        {0,4},
        {1,5},
        {2,6},
        {3,7},
        {0,1},
        {1,2},
        {2,3},
        {3,0},
        {4,5},
        {5,6},
        {6,7},
        {7,4}
    };
    vec3d centr = { 0,0,0 };
    void drawLine(float x1, float y1, float x2, float y2);
public:

    Cube(Screen& screen) : cScreen(screen)
    {
      
    }
    void rotCube(int speed_factor);
  
};

void Cube::drawLine(float x1, float y1, float x2, float y2)
{
    auto line = [&](float x1, float y1, float x2, float y2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        float length = std::sqrt(dx * dx + dy * dy);
        float angle = std::atan2(dy, dx);
        for (float i = 0; i < length; i++) {
            cScreen.pixel(x1 + std::cos(angle) * i, y1 + std::sin(angle) * i);
        }
    };
    line(x1,y1,x2,y2);
}
void Cube::rotCube(int speed_factor)
{
    static int count = 0;
    auto getCentroid = [&]() {
        for (auto& p : points) {
            centr.x += p.x;
            centr.y += p.y;
            centr.z += p.z;
        }
        centr.x /= points.size();
        centr.y /= points.size();
        centr.z /= points.size();
    };

    auto rotate = [](vec3d& point, float x, float y, float z) {
        float rad = 0;
        rad = x;
        point.y = std::cos(rad) * point.y - std::sin(rad) * point.z;
        point.z = std::sin(rad) * point.y + std::cos(rad) * point.z;

        rad = y;
        point.x = std::cos(rad) * point.x + std::sin(rad) * point.z;
        point.z = -std::sin(rad) * point.x + std::cos(rad) * point.z;

        rad = z;
        point.x = std::cos(rad) * point.x - std::sin(rad) * point.y;
        point.y = std::sin(rad) * point.x + std::cos(rad) * point.y;
    };

    if(count == 0)
        getCentroid();


    for (auto& p : points)
    {
        p.x -= centr.x;
        p.y -= centr.y;
        p.z -= centr.z;
        rotate(p, 0.002*speed_factor, 0.001 * speed_factor, 0.004 * speed_factor);
        p.x += centr.x;
        p.y += centr.y;
        p.z += centr.z;
        cScreen.pixel(p.x, p.y);
    }
    for (auto& conn : connectMatrix)
    {
        drawLine(points[conn.a].x, points[conn.a].y,
            points[conn.b].x, points[conn.b].y);
    }
    count++;
}

int main(int argc, char* argv[])
{
    Screen& screen = Screen::getInstance();
    Cube cube(screen);

    while (true)
    {
        cube.rotCube(6); // 1 -> base speed
        screen.show();
        screen.clear();
        screen.input();
        SDL_Delay(30);
    }

    return 0;
}
