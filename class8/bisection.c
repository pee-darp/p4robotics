#include <stdio.h>
double bisect(double a, double b, double tolerance, int itr_max);
double poly_eval(double x);
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: ./bisection <low> <high>\n");
        return 1;
    }
    double a, b, tol;
    tol = 1E-6;
    int args_read = 0;
    //printf("%11.4e\n", a);
    args_read = sscanf(argv[1], "%lf", &a);
    if (args_read <= 0) {
        return 1;
    }
    args_read = sscanf(argv[2], "%lf", &b);
    if (args_read <= 0) {
        return 1;
    }
    bisect(a, b, tol, 100000);
    //printf("%11.4e\n", root);
    return 0;
}

double bisect(double a, double b, double tolerance, int itr_max) {
    if (a > b) {
        fprintf(stderr, "Input lower limit first\n");
    }
    if (poly_eval(a) * poly_eval(b) > 0) {
        fprintf(stderr, "Both limits evaluate to same sign. Method failed.\n");
    }
    int itr = 0;
    double c = 0.0;
    while (itr <= itr_max) {
        c = (a + b) / 2;
        if (poly_eval(c) == 0 || (b - a) / 2 < tolerance) {
            printf("x_mid: %11.4e y_mid: %11.4e\n", c, poly_eval(c));
            return c;
        }
        itr++;
        if (poly_eval(a) * poly_eval(c) > 0) {
            printf("x_mid: %11.4e y_mid: %11.4e\n", c, poly_eval(c));
            a = c;
        } else {
            printf("x_mid: %11.4e y_mid: %11.4e\n", c, poly_eval(c));
            b = c;
        }
    }
    fprintf(stderr, "Method failed.\n");
    return 1;
}

double poly_eval(double x) {
    return 2 * x - 20 * x * x + 20 * x * x * x - 4 * x * x * x * x;
}
