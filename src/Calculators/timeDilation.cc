/*
calculates the velocity required and earth time passed to travel to a distant
place at relativistic speeds with some moving reference frame travel time.
e.g. if it takes you 3 years travel time to travel 50lyr, what is your velocity
and how much earth time has passed before you get there.
*/

#include "../DataBase/field.hh"
#include <complex>

class TimeDilation {
private:
    double travelTime, distance;
    double tol = 1e-7;
    int maxIter = 10000000; // assume v=c if you don't converge after this many iterations
    double velocity, earthTime;

    double 
    func(double v) {
        return (distance/v) - (travelTime/sqrt(1.0-v*v)); 
    }

    double
    deriv(double v) {
        return (-distance/(v*v))+(0.5*travelTime / pow(1.0-v*v,1.5))*(-2.0*v);
    }
public:
    
    
    TimeDilation(double travelTime, double distance) : 
        travelTime(travelTime), distance(distance) {
        velocity = 0.9; // initial guess
        compute();
    }

    ~TimeDilation() {}

    void
    compute() {
        int i = maxIter;
        do 
        {
            velocity -= func(velocity) / deriv(velocity);
            if (velocity >= 1.0)
                velocity = 1.0-tol;
            i -= 1;
        } while ((fabs(func(velocity)) >= tol) && (i>0));

        if (i==0)
            earthTime = distance;
        else if (velocity < 0) {
            velocity = 0;
            earthTime = travelTime;
        }
        else
            earthTime = (travelTime / sqrt(1.0-(velocity*velocity)));
    }
    
    double
    getVelocity() const { return velocity; }

    double
    getEarthTime() const { return earthTime; }
};