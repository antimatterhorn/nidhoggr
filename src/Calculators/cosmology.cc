/*
calculates a cosmology for a given redshift and cosmological parameters
*/

#include "../DataBase/field.hh"
#include <complex>

class Cosmology {
private:
    double ageNow, ageThen, radiusNow, radiusThen, omegaMatter, omegaLambda, omegaRadiation;
    double properDistanceNow, lightTravelDistanceNow, angularDistanceNow, luminosityDistanceNow;
    double recessionSpeedNow, properDistanceThen, hubbleParameterThen, recessionSpeedThen, curvatureThen;

    double z,m,l,h;
    double at,r,k,a,ai,qa,dx,t,dc,dp,da,dl,dtc,dtl,dtp,dta,mh,hpt;
    int n,eset;

    double myr = 978000.;
public:
    
    
    Cosmology(double z, double omega_m, double lam, double h0) : 
        z(z), m(omega_m), l(lam), h(h0) {
        at  = 1/(1+z);
        r   = 0.0000824;
        k   = 1.0-m-l-r;
        n   = 20000;
        a   = 0;
        ai  = 0;
        qa  = 0;
        dx  = 1.0/(double)n;
        t   = 0;
        eset= 0;
        dc  = 0;
        dp  = 0;
        da  = 0;
        dl  = 0;
        dtc = 0;
        dtl = 0;
        dtp = 0;
        dta = 0;

        compute();
        output();
    }

    ~Cosmology() {}

    void
    compute() {
        while (ai < 1.0) {
            a = ai + (dx/2.0);
            qa = sqrt((m/a) + k + r*pow(a,-2.0) + l*a*a);
            t  += (dx/qa);
            dc += ((1/(a*qa))*dx);
            dp += ((1/qa)*dx);
            ai += dx;
            
            if ((a>at) && (eset==0))
            {
                ageThen = t*myr/h;
                dtc  = dc;
                dtp  = dp;
                dtl  = dl;
                dta  = da;
                eset = 1;
            }
        }
        
        if (fabs(k) < 0.05)
        {
            da = at * (dc-dtc);
        } else if (k>0) {
            da = at * pow(k,-0.5)*0.5*(exp(sqrt(k)*(dc-dtc))-exp(-sqrt(k)*(dc-dtc)));
        } else {
            da = at * pow(-k,-0.5)*sin(sqrt(-k)*(dc-dtc));
        }
        
        if (da<0) da = -da;
        dl = da * pow(at,-2.0);
        
        mh= myr/h;

        t   = t*mh;
        dc  = dc*mh;
        dtc = dtc*mh;
        dp  = dp*mh;
        dtp = dtp*mh;
        da  = da*mh;
        dl  = dl*mh;
        
        ageNow                  = t;                                    //Myr
        radiusNow               = dc;                                   //Mly
        angularDistanceNow      = da;                                   //Mly
        luminosityDistanceNow   = dl;                                   //Mly
        lightTravelDistanceNow  = dp - dtp;                             //Mly
        properDistanceNow       = dc - dtc;                             //Mly
        recessionSpeedNow       = (((dc-dtc)/3.26)*h)/300000.0;         //c
        
        hpt = (1/at)*h*sqrt((m*(1/at))+(r*(1/pow(at,2.0)))+k+l*pow(at,2.0));
        
        hubbleParameterThen     = hpt;                                  //km/s/Mpc
        omegaMatter             = m*pow(h,2.0)/(pow(hpt,2.0)*pow(at,3.0));
        omegaLambda             = l*pow(h,2.0)/pow(hpt,2.0);
        omegaRadiation          = r*pow(h,2.0)/(pow(hpt,2.0)*pow(at,4.0));
        curvatureThen           = 1.0 - omegaMatter - omegaLambda - omegaRadiation;
        
        radiusThen              = dtc;                                  //Mly
        recessionSpeedThen      = (((dc-dtc)*at/3.26)*hpt)/300000.0;    //c
        properDistanceThen      = (dc-dtc)*at;                          //Mly
    }
    
    void
    output() {
        printf("\nNow:\n");
        printf("t0 = %3.4f Myr\n",t);
        printf("H0 = %3.4f\n",h);
        printf("Ωm = %3.4f\n",m);
        printf("Ωr = %3.4f\n",r);
        printf("Λ  = %3.4f\n",l);
        printf("K0 = %3.4f\n",k);
        printf("Radius = %3.4f Mly\n",radiusNow);
        printf("Angular Distance = %3.4f Mly\n",angularDistanceNow);
        printf("Luminosity Distance = %3.4f Mly\n",luminosityDistanceNow);
        printf("Light Travel Distance = %3.4f Mly\n",lightTravelDistanceNow);
        printf("Proper Distance = %3.4f Mly\n",properDistanceNow);
        printf("Recession Speed = %3.4f c\n",recessionSpeedNow);
        
        printf("\nThen:\n");
        printf("t  = %3.4f Myr\n",ageThen);
        printf("H  = %3.4f\n",hpt);
        printf("Ωm = %3.4f\n",omegaMatter);
        printf("Ωr = %3.4f\n",omegaRadiation);
        printf("Λ  = %3.4f\n",omegaLambda); 
        printf("K  = %3.4f\n",curvatureThen);
        printf("Radius = %3.4f Mly\n",radiusThen);
        printf("Proper Distance = %3.4f Mly\n",properDistanceThen);
        printf("Recession Speed = %3.4f c\n",recessionSpeedThen);
    }

};