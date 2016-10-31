/* 物体の拡散をN-box Systemで解くプログラム*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define T 100

double k;
double dt = 0.01;

/* 境界（＝端）の箱に関する計算 */
double f(double a, double b){
    return k * (a - b);
}

/* その他の箱での計算 */
double g(double a, double b, double c){
    return k * (a - 2.0 * b + c);
}
/* Euler法  */
void Euler(double *u, int N){
    int i;
    double u_new[N];
    
    /* calculation */
    u_new[0] = u[0] + dt * f(u[1], u[0]);
    u_new[N - 1] = u[N - 1] + dt * f(u[N - 2], u[N - 1]);
    
    for(i = 1; i < N - 1; ++i)
        u_new[i] = u[i] + dt * g(u[i + 1], u[i], u[i - 1]);

    /* renewal */
    for(i = 0; i < N; ++i)
        u[i] = u_new[i];
}
/* Runge-Kutta法 */
void RungeKutta(double *u, int N){
    int i;
    
    double u_new[N];
    double k[N][4];
    
    /* 1st step */
    k[0][0] = f(u[1], u[0]);
    k[N - 1][0] = f(u[N - 2], u[N - 1]);
    for(i = 1; i < N - 1; ++i)
        k[i][0] = g(u[i + 1], u[i], u[i - 1]);
    
    /* 2nd step */
    k[0][1] = f(u[1] + k[1][0] * dt / 2.0, u[0] + k[1][0] * dt / 2.0);
    k[N - 1][1] = f(u[N - 2] + k[N - 2][0] * dt / 2.0, u[N - 1] + k[N - 1][0] * dt / 2.0);
    for(i = 1; i < N - 1; ++i)
        k[i][1] = g(u[i + 1] + k[i + 1][0] * dt / 2.0, u[i] + k[i][0] * dt / 2.0, u[i - 1] + k[i - 1][0] * dt / 2.0);
    
    /* 3rd step */
    k[0][2] = f(u[1] + k[1][1] * dt / 2.0, u[0] + k[1][1] * dt / 2.0);
    k[N - 1][2] = f(u[N - 2] + k[N - 2][1] * dt / 2.0, u[N - 1] + k[N - 1][1] * dt / 2.0);
    for(i = 1; i < N - 1; ++i)
        k[i][2] = g(u[i + 1] + k[i + 1][1] * dt / 2.0, u[i] + k[i][1] * dt / 2.0, u[i - 1] + k[i - 1][1] * dt / 2.0);
    
    /* 4th step */
    k[0][3] = f(u[1] + k[1][2] * dt, u[0] + k[1][2] * dt);
    k[N - 1][3] = f(u[N - 2] + k[N - 2][2] * dt, u[N - 1] + k[N - 1][2] * dt);
    for(i = 1; i < N - 1; ++i)
        k[i][3] = g(u[i + 1] + k[i + 1][2] * dt, u[i] + k[i][2] * dt, u[i - 1] + k[i - 1][2] * dt);
    
    /* final step */
    for(i = 0; i < N; ++i)
        u_new[i] = u[i] + dt * (k[i][0] + 2.0 * k[i][1] + 2.0 * k[i][2] + k[i][3]) / 6.0;
    
    /* renewal */
    for(i = 0; i < N; ++i)
        u[i] = u_new[i];
}

int main(void){
    int i, i_time;
    
    /* 解法の選択 */
    int method;
    printf("Euler Method : 0 or Runge-Kutta Method : 1\n");
    printf("Enter 0 or 1.\n");
    scanf("%d", &method);
    
    /* 微分方程式に含まれる係数（隣の箱への流れやすさ）の決定 */
    printf("k = ");
    scanf("%lf", &k);
    
    /* 箱の数を決定 */
    int N;
    printf("N = ");
    scanf("%d", &N);
    double u[N], u_new[N];
    
    /* 初期値は乱数で0〜1 */
    srand((unsigned)time(NULL));
    for(i = 0; i < N; ++i)
        u[i] = rand() / (double)RAND_MAX;
    
    /*
    for(i = 0; i < N; ++i)
        printf("%lf\n", u[i]);
    */
    
    double dt = 0.01;
    
    /* 以下タイムループ */
    for(i_time = 0; i_time < T; ++i_time){
        /* 表示 */
        for(i = 0; i < N; ++i)
            printf("%lf ", u[i]);
    
        printf("\n");
        
        /* Calculation */
        if(method == 0){
            Euler(u, N);
        }else if(method == 1){
            RungeKutta(u, N);
        }else{
            printf("Erorr!\n");
            return 0;
        }
    }
    
    return 0;
}