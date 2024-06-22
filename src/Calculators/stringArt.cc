#include "../Math/vectorMath.hh"
#include <iostream>

class StringArt {
private:
    std::vector<std::vector<int>> image;
    std::vector<Lin::Vector<2>> nails;
    int windings;
    int mode;
    double stringWidth;
    double radius;
public:
    using Vector = Lin::Vector<2>;

    StringArt(std::vector<std::vector<int>> image, std::vector<Lin::Vector<2>> nails, 
        int windings=1000, int mode=0, double width=0.2, double radius=1.0) :
        image(image), nails(nails), windings(windings), mode(mode), stringWidth(width), radius(radius) {
    }

    ~StringArt() {}

    std::vector<Vector> BresenhamLine(Vector start, Vector end) {
        std::vector<Vector> points;
        int x1 = static_cast<int>(start[0]);
        int y1 = static_cast<int>(start[1]);
        int x2 = static_cast<int>(end[0]);
        int y2 = static_cast<int>(end[1]);
            
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);

        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;

        int err = dx - dy;

        while (true) {
            points.push_back(Vector{static_cast<double>(x1), static_cast<double>(y1)});

            if (x1 == x2 && y1 == y2) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }

        return points;
    }

    double
    Strength(Vector n0, Vector n1) {
        Vector v0 = n0 + Vector{radius,radius};
        Vector v1 = n1 + Vector{radius,radius};
        std::vector<Vector> points = BresenhamLine(v0,v1);
        int s = 0;
        int c = 0;
        for (int i=0;i<points.size();i++) {
            c++;
            int px = points[i][0];
            int py = points[i][1];
            s += 255*(1-mode) + pow(-1,mode+1)*image[px][py];
        }
        return ( c > 0 ? s/c : 0);
    }

    void
    ClearPixels(Vector n0, Vector n1) {
        Vector v0 = n0 + Vector{radius,radius};
        Vector v1 = n1 + Vector{radius,radius};
        std::vector<Vector> points = BresenhamLine(v0,v1);
        for (int i=0;i<points.size();i++) {
            int px = points[i][0];
            int py = points[i][1];
            image[px][py] = 255*(1-mode);
        }
    }

};