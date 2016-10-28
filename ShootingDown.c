#include <stdio.h>
#include <math.h>

#define N 4000
#define M 5
#define T 4
#define pi M_PI

double dist(double a, double b){
    double x = a * a + b * b;
    
    return sqrt(x);
}

void judgement(double distance, double radius){
    printf("Distance of closest approach = %lf\n", distance);
    
    if(distance > radius){
        printf("Fail!\n");
    }else{
        printf("Success!\n");
    }
}

int main(void){
    int i, j;
    int k = 0;
    double dt = (double)T / N;
    double xN, xN_new;
    double yN, yN_new;
    double uN, uN_new;
    double vN, vN_new;
    
    double a, b;
    printf("Enter the coordinate of the target!\n");
    printf("x-coordinate = ");
    scanf("%lf", &a);
    printf("y-coordinate = ");
    scanf("%lf", &b);
    
    double m = 1.0;
    double gamma = 0.1;
    double g = 9.8;
    double radius = 1.0;
    double distance;
    double distance_c = dist(a, b);
    double theta, flag;
    
    double v0;
    printf("v0 = ");
    scanf("%lf", &v0);
    
    for(i = 0; i <= 90; ++i){
        theta = i * pi / (2.0 * 90.0);
        flag = dist(a, b);
        
        xN = 0.0;
        yN = 0.0;
        uN = v0 * cos(theta);
        vN = v0 * sin(theta);
    
        for(j = 0; j < N; ++j){
            distance = dist(xN - a, yN - b);
        
            if(flag > distance){
                flag = distance;
            }
        
            xN_new = xN + dt * uN;
            yN_new = yN + dt * vN;
            uN_new = uN - dt * (gamma / m) * uN;
            vN_new = vN - dt * (g + (gamma / m) * vN);
            xN = xN_new;
            yN = yN_new;
            uN = uN_new;
            vN = vN_new;
        }
        
        if(distance_c > flag){
            distance_c = flag;
            k = i;
        }
    }
    
    printf("v0 = %lf,theta = %d * (pi / 90)\n", v0, k);
    judgement(distance_c, radius);
    
    return 0;
}