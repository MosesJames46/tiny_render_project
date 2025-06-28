#include "tgaimage.h"
#include "Model.h"
#include <cmath>
#include "tgaimage.h"
#include <iostream>
#include <chrono>
#include "Vector3.h"
#include "Line.h"
#include "Triangle.h"
#include "basic_math.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

constexpr TGAColor white = { 255, 255, 255, 255 }; // attention, BGRA order
constexpr TGAColor green = { 0, 255,   0, 255 };
constexpr TGAColor red = { 0,   0, 255, 255 };
constexpr TGAColor blue = { 255, 128,  64, 255 };
constexpr TGAColor yellow = { 0, 200, 255, 255 };

constexpr int width = 800;
constexpr int height = 800;

std::vector<float> z_buffer(width * height, 0.f);

int main(int argc, char** argv) {
    
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax = 17, ay = 4, az = 13;
    int bx = 55, by = 39, bz = 128;
    int cx = 23, cy = 59, cz = 255;

    
    auto start = std::chrono::high_resolution_clock::now();
    /*framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);*/
    Vec3 a(ax, ay, az);
    Vec3 b(bx, by, bz);
    Vec3 c(cx, cy, cz);
    
    /*Line::draw_line(a, b, framebuffer, red);
    Line::draw_line(b, c, framebuffer, blue);
    Line::draw_line(c, a, framebuffer, green);*/
    //Triangle T(a, b, c, true);

    Model m("Exc.obj");
    //m.wire_frame(framebuffer, blue);
    //m.rasterize_scan(framebuffer, white);
    
    m.rotate_matrix(framebuffer, white, z_buffer);
    //m.rasterize_bary(framebuffer, white, z_buffer);

    auto end = std::chrono::high_resolution_clock::now();
    auto time = end - start;
    std::cout << time.count() / 1000000000.f << std::endl;
    framebuffer.write_tga_file("framebuffer.tga");

    return 0;
}