#include "game.hpp"
#include <iostream>
#include <complex.h>

using namespace std::complex_literals;

#define WIDTH 128           // Usually 320
#define HEIGHT 128          // Usually 240
#define NUM_COLORS 18

const double zoom = 80;     // Usually 130
const double x_range = WIDTH / zoom;
const double y_range = HEIGHT / zoom;
const double new_draw_angle_threshold = 0.1;
const double radius = 0.5;

double angle = 0.0;
double old_angle = -100.0;
std::complex<double> c = std::complex<double>(radius * cos(angle), radius * sin(angle));
uint32_t last_time = 0;

int grid[WIDTH][HEIGHT];
blit::Pen colors[NUM_COLORS];

int in_set(std::complex<double> num, std::complex<double> c) {
    for (int i = 0; i < NUM_COLORS - 1; i++) {
        num = num * num + c;
        if (std::abs(num) > 2) {
            return i;
        }
    }
    return NUM_COLORS - 1;
}

double scale_x(int x) {
    return - x_range + (double)x / WIDTH * x_range * 2;
}

double scale_y(int y) {
    return - y_range + (double)y / HEIGHT * y_range * 2;
}

void update_grid(std::complex<double> c) {
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            grid[i][j] = in_set(scale_x(i) + scale_y(j) * 1i, c);
        }
    }
}


void init() {
    blit::set_screen_mode(blit::ScreenMode::hires);

    for (int i = 0; i < NUM_COLORS; i++)
    {
        colors[i] = blit::Pen(30, 200 * i / NUM_COLORS, 130 - 130 * i / NUM_COLORS);
    }
    
}

void render(uint32_t time) {
    blit::screen.pen = blit::Pen(0, 0, 0);
    blit::screen.clear();

    for(int x=0; x<WIDTH; x++) {
        for(int y=0; y<HEIGHT; y++) {
            blit::screen.pen = colors[grid[x][y]];                
            blit::screen.pixel(blit::Point(x,y));
        }
    }
}

void update(uint32_t time) {
    if (last_time == 0) last_time = time; // first frame init
    double dt = (double) (time - last_time);
    last_time = time;

    angle += (double) (((float) dt) / (float) 1000.0);

    if (abs(angle - old_angle) >= new_draw_angle_threshold) {
        old_angle = angle;
        c = std::complex<double>(radius * cos(angle), radius * sin(angle));
        update_grid(c);
    }
}
