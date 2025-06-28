#include "Line.h"
#include "tgaimage.h"
#include "Vector3.h"

void Line::draw_line(int ax, int ay, int bx, int by, TGAImage & image, TGAColor color) {
    bool steep = false;
    if (ax > bx) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    if (bx - ax < by - ay) {
        std::swap(ax, ay);
        std::swap(bx, by);
        steep = true;
    }

    int dy = by - ay;
    int dx = bx - ax;

    int error = 0;
    int y = ay;
    int y_slope = (by - ay) > 0 ? 1 : -1;
    for (int x = ax; x <= bx; ++x) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        
        if (error >= dx) {
            y += y_slope;
            error -= dx;
        }
        error += dy;
    }
}

void Line::draw_line(Vec3 a, Vec3 b, TGAImage& image, TGAColor color) {
    bool steep = std::abs(b.x - a.x) < std::abs(b.y - a.y);
    
    if (steep) {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }

    if (a.x > b.x) {
        std::swap(a, b);
    }

    int dy = std::abs(b.y - a.y);
    int dx = b.x - a.x;

    //if (dx == 0) return;
    
    int error = 0;
    int y = a.y;
    int y_slope = a.y < b.y ? 1 : -1;
   
    for (int x = a.x; x <= b.x; ++x) {
        if (steep) {
            image.set(y, x, color);
        }else {
            image.set(x, y, color);
        }
        error += dy;
        if (error > dx) {
            y += y_slope;
            error -= dx;
        }
        
    }

    /*if (steep) {
        image.set(a.y, a.x, TGAColor({ 255, 0, 0, 255 }));
        image.set(b.y, b.x, TGAColor({ 255, 0, 0, 255 }));
    }
    else {
        image.set(a.x, a.y, TGAColor({ 255, 0, 0, 255 }));
        image.set(b.x, b.y, TGAColor({ 255, 0, 0, 255 }));
    }*/
    
}