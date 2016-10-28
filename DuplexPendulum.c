#include <stdio.h>
#include <math.h>

#define T 100
#define N 10000
#define P M_PI


double f(double x){
    return x;
}

double g(double theta0, double theta1, double u, double v){
    double m0 = 2.0;
    double m1 = 0.4;
    double l0 = 1.0;
    double l1 = 1.3;
    double g = 9.8;
    
    double s = g * (2.0 * m0 + m1) * sin(theta0) + g * m1 * sin(theta0 - 2.0 * theta1) + 2.0 * m1 * sin(theta0 - theta1) * (l0 * u * u * cos(theta0 - theta1) + l1 * v * v);
    double t = 2.0 * l0 * (m0 - m1 * cos(theta0 - theta1) * cos(theta0 - theta1) + m1);
    
    return s / t;
}

double h(double theta0, double theta1, double u, double v){
    double m0 = 2.0;
    double m1 = 0.4;
    double l0 = 1.0;
    double l1 = 1.3;
    double g = 9.8;
    
    double s = sin(theta0 - theta1) * ((m0 + m1) * (g * cos(theta0) + l0 * u * u) + l1 * m1 * v * v * cos(theta0 - theta1));
    double t = l1 * (m0 - m1 * cos(theta0 - theta1) * cos(theta0 - theta1) + m1);
    
    return s / t;
}

int main(void){
    int i;
    double dt = (double)T / N;
    double xN, xN_new;
    double yN, yN_new;
    double uN, uN_new;
    double vN, vN_new;
    double kx1, kx2, kx3, kx4;
    double ky1, ky2, ky3, ky4;
    double ku1, ku2, ku3, ku4;
    double kv1, kv2, kv3, kv4;
    
    double x1, x2, y1, y2;
    double l0 = 1.0;
    double l1 = 1.3;
    
    
    FILE *gp;
    gp = popen("gnuplot -persist", "w");
    
    fprintf(gp, "set term x11\n");
    fprintf(gp, "set xrange [-3.0:3.0]\n");
    fprintf(gp, "set yrange [-3.0:3.0]\n");
    //fprintf(gp, "plot '-' w l\n");
    
    xN = P / 12.0;
    yN = P / 4.0;
    uN = 0.0;
    vN = 0.0;
    
    
    for(i = 0; i < N; ++i){
        x1 = l0 * sin(xN);
        y1 = l0 * cos(xN);
        x2 = x1 + l1 * sin(yN);
        y2 = y1 + l1 * cos(yN);
        
        fprintf(gp, "plot '-' w l\n");
        fprintf(gp, "0 0\n");
        fprintf(gp, "%lf %lf \n", x1, y1);
        fprintf(gp, "%lf %lf \n", x2, y2);
        
        kx1 = f(uN);
        ky1 = f(vN);
        ku1 = g(xN, yN, uN, vN);
        kv1 = h(xN, yN, uN, vN);
        
        kx2 = f(uN + ku1 * dt / 2.0);
        ky2 = f(vN + kv1 * dt / 2.0);
        ku2 = g(xN + kx1 * dt / 2.0, yN + ky1 * dt / 2.0, uN + ku1 * dt / 2.0, vN + kv1 * dt / 2.0);
        kv2 = h(xN + kx1 * dt / 2.0, yN + ky1 * dt / 2.0, uN + ku1 * dt / 2.0, vN + kv1 * dt / 2.0);
        
        kx3 = f(uN + ku2 * dt / 2.0);
        ky3 = f(vN + kv2 * dt / 2.0);
        ku3 = g(xN + kx2 * dt / 2.0, yN + ky2 * dt / 2.0, uN + ku2 * dt / 2.0, vN + kv2 * dt / 2.0);
        kv3 = h(xN + kx2 * dt / 2.0, yN + ky2 * dt / 2.0, uN + ku2 * dt / 2.0, vN + kv2 * dt / 2.0);
        
        kx4 = f(uN + ku3 * dt);
        ky4 = f(vN + kv3 * dt);
        ku4 = g(xN + kx3 * dt, yN + ky3 * dt, uN + ku3 * dt, vN + kv3 * dt);
        kv4 = h(xN + kx3 * dt, yN + ky3 * dt, uN + ku3 * dt, vN + kv3 * dt);
        
        xN_new = xN + dt * (kx1 + 2.0 * kx2 + 2.0 * kx3 + kx4) / 6.0;
        yN_new = yN + dt * (ky1 + 2.0 * ky2 + 2.0 * ky3 + ky4) / 6.0;
        uN_new = uN + dt * (ku1 + 2.0 * ku2 + 2.0 * ku3 + ku4) / 6.0;
        vN_new = vN + dt * (kv1 + 2.0 * kv2 + 2.0 * kv3 + kv4) / 6.0;
        
        xN = xN_new;
        yN = yN_new;
        uN = uN_new;
        vN = vN_new;
        fprintf(gp, "e\n");
    }
    
    //fprintf(gp, "e\n");
    
    fflush(gp);
    pclose(gp);
    
    return 0;
}