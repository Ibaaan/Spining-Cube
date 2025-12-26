#include <iostream>
#include <pico/stdlib.h>
#include "st7735.h"
#include "fonts.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <format> // C++20
#include <sstream>

#define WIDTH 160
#define HEIGHT 128

#define MAX_X 1.60
#define MAX_Y 1.28
#define MIN_X -1.60
#define MIN_Y -1.28

#define RK_UP 15
#define RK_DOWN 17
#define RK_RIGHT 10
#define RK_LEFT 11

class Vector3
{
public:
    double x;
    double y;
    double z;

    Vector3(double x1, double y1, double z1)
        : x(x1), y(y1), z(z1)
    {
    }

    Vector3()
        : x(0.), y(0.), z(0.)
    {
    }

    Vector3 translate(double zcv)
    {
        return Vector3(x, y, z + zcv);
    }

    Vector3 rotate_xy(double angle)
    {
        double s = sin(angle);
        double c = cos(angle);
        double new_x = x * c - y * s;
        double new_y = x * s + y * c;
        return Vector3(new_x, new_y, z);
    }

    Vector3 rotate_xz(double angle)
    {
        double s = sin(angle);
        double c = cos(angle);
        double new_x = x * c - z * s;
        double new_z = x * s + z * c;
        return Vector3(new_x, y, new_z);
    }

    Vector3 rotate_yz(double angle)
    {
        double s = sin(angle);
        double c = cos(angle);
        double new_y = y * c - z * s;
        double new_z = y * s + z * c;
        return Vector3(x, new_y, new_z);
    }
};

int calculate_pixel_y(double y_val)
{
    return (HEIGHT) * (MAX_Y - y_val) / (MAX_Y - MIN_Y);
}

int calculate_pixel_x(double x_val)
{
    return (WIDTH) * (x_val - MIN_X) / (MAX_X - MIN_X);
}

struct PlaneZIndex
{
    int index;
    double z_value;
};

bool compare_z_index(const PlaneZIndex &z1, const PlaneZIndex &z2)
{
    return z1.z_value < z2.z_value;
}

int main()
{
    Vector3 dot1(0.5, 0.5, 0.5);
    Vector3 dot2(-0.5, 0.5, 0.5);
    Vector3 dot3(-0.5, -0.5, 0.5);
    Vector3 dot4(0.5, -0.5, 0.5);

    Vector3 dot5(0.5, 0.5, -0.5);
    Vector3 dot6(-0.5, 0.5, -0.5);
    Vector3 dot7(-0.5, -0.5, -0.5);
    Vector3 dot8(0.5, -0.5, -0.5);

    std::vector<Vector3> dots;

    dots.push_back(dot1);
    dots.push_back(dot2);
    dots.push_back(dot3);
    dots.push_back(dot4);
    dots.push_back(dot5);
    dots.push_back(dot6);
    dots.push_back(dot7);
    dots.push_back(dot8);

    std::vector<std::vector<int>> fs = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
    };

    std::vector<std::vector<int>> trs = {
        {0, 1, 2, 3},
        {0, 4, 5, 1},
        {0, 3, 7, 4},
        {2, 3, 7, 6},
        {1, 2, 6, 5},
        {5, 4, 7, 6},
    };

    int colors[6]{
        ST7735_WHITE,
        ST7735_RED,
        ST7735_GREEN,
        ST7735_MAGENTA,
        ST7735_BLUE,
        ST7735_YELLOW};

    std::vector<PlaneZIndex> plane_fz_idcs;

    ST7735_Init();
    ST7735_FillScreen(ST7735_BLACK);
    int xx1, yy1, yy2, xx2, xx3, yy3, xx4, yy4;
    double angle_x = 0.;
    double angle_y = 0.;
    double angle_z = 0.;

    Vector3 new_dots[8];
    while (true)
    {
        

        

        ST7735_FillScreen(ST7735_BLACK);
        angle_x += 0.02;
        angle_y += 0.015;
        angle_z += 0.01;

        for (int i = 0; i < 8; i++)
        {
            new_dots[i] = dots[i]
                              .rotate_yz(angle_x)  // Around X
                              .rotate_xz(angle_y); // Around Y
        }

        for (int index = 0; index < trs.size(); index++)
        {
            std::vector<int> v = trs[index];
            double z_summ = 0;
            for (int i : v)
            {
                z_summ += new_dots[i].z;
            }
            plane_fz_idcs.push_back((PlaneZIndex){index, z_summ});
        }
        std::sort(plane_fz_idcs.begin(), plane_fz_idcs.end(), compare_z_index);
        for (int i = plane_fz_idcs.size() - 1; i >= 0; i--)
        {
            PlaneZIndex plz = plane_fz_idcs[i];
            std::vector<int> square = trs[plz.index];

            Vector3 dot1 = new_dots[square[0]];
            Vector3 dot2 = new_dots[square[1]];
            Vector3 dot3 = new_dots[square[2]];
            Vector3 dot4 = new_dots[square[3]];
            int color = colors[plz.index];

            xx1 = calculate_pixel_x(dot1.x / (dot1.z + 2.));
            yy1 = calculate_pixel_y(dot1.y / (dot1.z + 2.));
            xx2 = calculate_pixel_x(dot2.x / (dot2.z + 2.));
            yy2 = calculate_pixel_y(dot2.y / (dot2.z + 2.));
            xx3 = calculate_pixel_x(dot3.x / (dot3.z + 2.));
            yy3 = calculate_pixel_y(dot3.y / (dot3.z + 2.));
            xx4 = calculate_pixel_x(dot4.x / (dot4.z + 2.));
            yy4 = calculate_pixel_y(dot4.y / (dot4.z + 2.));

            ST7735_DrawTriangleFill(
                xx1, yy1,
                xx2, yy2,
                xx3, yy3,
                color);

            ST7735_DrawTriangleFill(
                
                xx4, yy4,
                xx3, yy3,
                xx1, yy1,
                color
            );
        }

        // for (std::vector<int> f : fs)
        // {
        //     for (int i = 0; i < f.size(); i++)
        //     {
        //         Vector3 dot1 = new_dots[f[i]];
        //         Vector3 dot2 = new_dots[f[(i + 1) % f.size()]];
        //         xx1 = calculate_pixel_x(dot1.x / (dot1.z + 2.));
        //         yy1 = calculate_pixel_y(dot1.y / (dot1.z + 2.));
        //         xx2 = calculate_pixel_x(dot2.x / (dot2.z + 2.));
        //         yy2 = calculate_pixel_y(dot2.y / (dot2.z + 2.));
        //         ST7735_DrawLine(xx1, yy1, xx2, yy2, ST7735_WHITE);
        //     }
        // }

        ST7735_Update();
        plane_fz_idcs.clear();
    }

    return 0;
}