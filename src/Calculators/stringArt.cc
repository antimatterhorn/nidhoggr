#include "../Math/vectorMath.hh"

class StringArt {
private:
    std::string fileName;
    int nails,windings;
    bool mode;
    double stringWidth;
public:
    using Vector = Lin::Vector<2>;

    StringArt(std::string fileName, int nails=300, int windings=1000, bool mode=0, double width=0.2) :
        fileName(fileName), nails(nails), windings(windings), mode(mode), stringWidth(width) {

    }

    ~StringArt() {}

};