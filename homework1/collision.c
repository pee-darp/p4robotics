#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct point {
    double x;
    double y;
} point_t;

typedef struct line {
    point_t p1;
    point_t p2;
} line_t;

typedef struct polygon {
    int size;
    line_t side[16];
    double x_glob, y_glob;
    double rot;
} polygon_t;

bool is_intersect(line_t *l1, line_t *l2);
bool is_pt_eq(point_t *p1, point_t *p2);
void init_line(line_t *l, double x1, double y1, double x2, double y2);
bool is_collide(polygon_t *pol1, polygon_t *pol2);
void init_poly(polygon_t *pol, point_t *point);
bool is_contain(polygon_t *pol1, polygon_t *pol2);
double cross(line_t *l, point_t *p);
void rot(point_t *p, double theta);
void trans(point_t *p, double trans_x, double trans_y);

int main(void) {
    polygon_t polygons[50] = {0};
    FILE *fp, *f;
    int n_poly = 0;
    int n_pol = 0;
    //skip the keys//
    char cha;
    f = fopen("polygons.csv", "r");
    if (!f) {
        fprintf(stderr, "polygons.csv is missing\n");
        return 1;
    }
    while (1) {
        cha = fgetc(f);
        if (cha == '\n') {
            n_poly++;
        }
        if (cha == EOF) {
            break;
        }
    }
    n_poly -= 1; //To account for the key line
    if (n_poly > 2) {
        fprintf(stderr, "Too many polygons\n");
        return 1;
    }
    char ch;
    fp = fopen("polygons.csv", "r");
    while (1) {
        ch = fgetc(fp);
        if (ch == '\n') {
            break;
        }
    }
    //printf("\n");
    //initialize the polygons//
    int args_read = 0;
    while (n_pol < n_poly + 1) {
        args_read = fscanf(fp, "%lf %lf %lf %d", &polygons[n_pol].x_glob,
                           &polygons[n_pol].y_glob, &polygons[n_pol].rot, &polygons[n_pol].size);
        if (args_read == 0) {
            printf("%d\n", args_read);
            fprintf(stderr, "1.a polygons.csv number is invalid\n");
            return 1;
        }
        if (polygons[n_pol].size > 16) {
            fprintf(stderr, "Error:Please enter polygons of size less than 16\n");
            return 1;
        }
        point_t point[16] = {0.0};
        for (int i = 0; i < polygons[n_pol].size; i++) {
            args_read = fscanf(fp, "%lf", &point[i].x);
            if (args_read < 1) {
                fprintf(stderr, "2.a polygons.csv number is invalid\n");
                return 1;
            }
        }
        for (int i = 0; i < polygons[n_pol].size; i++) {
            args_read = fscanf(fp, "%lf", &point[i].y);
            if (args_read < 1) {
                fprintf(stderr, "3.a polygons.csv number is invalid\n");
                return 1;
            }
        }
        for (int i = 0; i < polygons[n_pol].size; i++) {
            rot(&point[i], polygons[n_pol].rot);
            trans(&point[i], polygons[n_pol].x_glob, polygons[n_pol].y_glob);
        }
        init_poly(&polygons[n_pol], point);
        n_pol++;
    }
    //check collision//
    for (int i = 0; i < n_poly - 1; i++) {
        for (int j = i + 1; j < n_poly; j++) {
            bool collision = is_collide(&polygons[i], &polygons[j]);
            bool contain = is_contain(&polygons[i], &polygons[j]);
            //printf("p%d and p%d are colliding:%d | contained:%d\n", i, j, collision, contain);
            if (collision || contain) {
                //if (contain) printf("%d,%d:contain\n", i, j);
                printf("collision!\n");
                return 0;
            }
        }
    }
    printf("no collision\n");
}

bool is_intersect(line_t *l1, line_t *l2) {
    double cp1 = (l1->p2.x - l1->p1.x) * ((l2->p1.y) - (l1->p1.y)) -
                 ((l1->p2.y - l1->p1.y) * ((l2->p1.x) - (l1->p1.x)));
    double cp2 = ((l1->p2.x - l1->p1.x) * (l2->p2.y - l1->p1.y)) -
                 ((l1->p2.y - l1->p1.y) * (l2->p2.x - l1->p1.x));
    double cp3 = (l2->p2.x - l2->p1.x) * ((l1->p1.y) - (l2->p1.y)) -
                 ((l2->p2.y - l2->p1.y) * ((l1->p1.x) - (l2->p1.x)));
    double cp4 = ((l2->p2.x - l2->p1.x) * (l1->p2.y - l2->p1.y)) -
                 ((l2->p2.y - l2->p1.y) * (l1->p2.x - l2->p1.x));
    //printf("cp1:%f, cp2:%f\n", cp1, cp2);
    if (cp1 * cp2 < 0 && cp3 * cp4 < 0) {
        return 1;
    }
    if (cp1 * cp2 > 0) {
        return 0;
    }
    if (cp1 * cp2 == 0) {
        if (is_pt_eq(&l1->p1, &l2->p1) || is_pt_eq(&l1->p1, &l2->p2) ||
            is_pt_eq(&l1->p2, &l2->p1) || is_pt_eq(&l1->p2, &l2->p2)) {
            return 1;
        }
        return 0;
    }
    return 0;
}

void init_line(line_t *l, double x1, double y1, double x2, double y2) {
    l->p1.x = x1;
    l->p1.y = y1;
    l->p2.x = x2;
    l->p2.y = y2;
}

bool is_pt_eq(point_t *p1, point_t *p2) {
    if (p1->x == p2->x && p1->y == p2->y) {
        return 1;
    }
    return 0;
}

bool is_collide(polygon_t *pol1, polygon_t *pol2) {
    for (int i = 0; i < pol1->size; i++) {
        for (int j = 0; j < pol2->size; j++) {
            //printf("side %d x side %d:\n", i, j);
            if (is_intersect(&pol1->side[i], &pol2->side[j])) {
                return 1;
            }
        }
    }
    return 0;
}

void init_poly(polygon_t *pol, point_t *point) {
    for (int i = 0; i < (pol->size) - 1; i++) {
        init_line(&pol->side[i], point->x, point->y, (point + 1)->x, (point + 1)->y);
        point++;
    }
    init_line(&pol->side[pol->size - 1], point->x, point->y,
              (point - (pol->size - 1))->x, (point - (pol->size - 1))->y);
}

double cross(line_t *l, point_t *p) {
    return ((l->p2.x - l->p1.x) * (l->p1.y - p->y) - (l->p2.y - l->p1.y) * (l->p1.x - p->x));
}

bool is_contain(polygon_t *pol1, polygon_t *pol2) {
    int size[2] = {pol1->size, pol2->size};
    double cross_prev = cross(&pol1->side[0], &pol2->side[0].p1);
    //printf("cross_prev:%lf\n", cross_prev);
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++) {
            double cros = cross(&pol1->side[i], &pol2->side[j].p1);
            //printf("side[%d]xside[%d]:%lf\n", i, j, cros);
            if (cros * cross_prev < 0) {
                return 0;
            }
            cross_prev = cros;
        }
    }
    return 1;
}

void rot(point_t *p, double theta) {
    double theta_rad = (theta * 3.14159) / 180.0;
    double x = p->x;
    double y = p->y;
    //printf("theta_rad:%f | p.x:%f | p.y:%f\n",theta_rad, p->x, p->y);
    p->x = (x * cos(theta_rad) - y * sin(theta_rad));
    p->y = (x * sin(theta_rad) + y * cos(theta_rad));
    //printf("x:%f|y:%f\n",p->x,p->y);
}

void trans(point_t *p, double trans_x, double trans_y) {
    p->x = p->x + trans_x;
    p->y = p->y + trans_y;
}
