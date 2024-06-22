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
    std::vector<std::pair<Lin::Vector<2>, Lin::Vector<2>>> lines;
    std::vector<int> pins;

    std::vector<int> Range(int k0, int k1) {
        std::vector<int> K;
        for (int i = k0; i < k1; ++i) {
            K.push_back(i);
        }
        return K;
    }

public:
    using Vector = Lin::Vector<2>;

    StringArt(std::vector<std::vector<int>> image, std::vector<Lin::Vector<2>> nails, 
        int windings=1000, int mode=0, double width=0.2, double radius=1.0) :
        image(image), nails(nails), windings(windings), mode(mode), stringWidth(width), radius(radius) {
        Compute();
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

    int
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

    void
    Compute() {
        
        int k=0;
        int numNails = nails.size();
        double ll=0;

        for (int i=0; i<windings; ++i) {
            int maxP = 0;
            int maxJ = 0;
            std::vector<int> K = Range(k-5,k+5);
            std::vector<int> J;
            for (int n=0; n<K.size(); ++n)
                J.push_back((((K[n]+numNails)%numNails)+numNails)%numNails);
            std::vector<int> R = Range(0,numNails);
            for (int j : J)
                R.erase(std::remove(R.begin(), R.end(), j), R.end());
            for (int j : R) {
                Vector n0 = nails[k];
                Vector n1 = nails[j];
                int s = Strength(n0,n1);
                if (s > maxP) {
                    maxP = s;
                    maxJ = j;
                }
            }
            if (maxP == 0)
                break;
            int jj = maxJ;
            Vector n0 = nails[k];
            Vector n1 = nails[jj];
            double rr = (n0-n1).magnitude();
            ll += rr;
            lines.push_back(std::make_pair(n0,n1));
            ClearPixels(n0,n1);
            printf("%d %d   %d -> %d\n",i,windings,k,jj);
            k = jj;
            pins.push_back(jj);
        }
    }

    std::vector<int>
    PinList() { return pins; }

};