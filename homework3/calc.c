#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

double additive_expr(char **str);
double multiplicative_expr(char **str);
double paranthesis_expr(char **str);
double num_literal(char **str);
double digits(char **str);
int digit(char **str);

void skip_whitespace(char **str) {
    while (isspace(*str[0])) {
        (*str)++;
    }
}

char peak(char **str) {
    skip_whitespace(str);
    return *str[0];
}

char look(char **str) {
    return *str[0];
}

char parse_token(char **str) {
    skip_whitespace(str);
    char token = *str[0];
    if (!token) {
        fprintf(stderr, "parsing error. unexpected end of string\n");
        exit(1);
    }
    (*str)++;
    return token;
}

int digit(char **str) {
    if (isdigit(look(str))) {
        return (int)(parse_token(str) - '0');
    }
    fprintf(stderr, "Parse error. Not a digit!\n");
    exit(1);
}

double digits(char **str) {
    double big_value = 0;
    double small_value = 0;
    while (isdigit(look(str))) {
        big_value = big_value * 10 + digit(str);
    }
    //printf("we have %c\n", peak(str));
    if (peak(str) == '.') {
        char waste = parse_token(str);
        double div = 0.1;
        while (isdigit(look(str))) {
            //printf("%c\n", **str);
            small_value = small_value + digit(str) * div;
            div /= 10.0;
        }
    }
    return big_value + small_value;
}

double num_literal(char **str) {
    int sign = 1;
    double value = 0;
    if (peak(str) == '+' || peak(str) == '-') {
        if (peak(str) == '-') {
            sign = -1;
        }
        char waste = parse_token(str);
    }
    if (isdigit(peak(str))) {
        value = digits(str);
        //printf("%c\n", peak(str));
    } else {
        if (peak(str) != '.' && peak(str) != '\0') {
            char check = peak(str);
            fprintf(stderr, "parsing error. expected number (0-9,+,-,.) got '%c'\n", check);
            exit(1);
        }
        value = digits(str);
    }
    // char check = peak(str);
    // if (check != '+' && check != '-' && check != '*' && check != '/' &&
    //     check != ')' && check != '\0') {
    //     fprintf(stderr, "parsing error. unexpected junk at end. got '%c'\n", check);
    //     exit(1);
    // }
    return (sign * value);
}

double paranthesis_expr(char **str) {
    double value = 0;
    if (peak(str) != '(') {
        //parse_token(str);
        value = num_literal(str);
        return value;
    }
    char waste = parse_token(str);
    value += additive_expr(str);
    if (look(str) != ')') {
        fprintf(stderr, "parsing error. expected ')' got '%c'\n", peak(str));
        exit(1);
    }
    //printf("%c\n", peak(str));
    // if (parse_token(str) != ')') {
    //     fprintf(stderr, "Missing closing brackets\n");
    //     exit(1);
    // }
    return value;
}

double multiplicative_expr(char **str) {
    double value = 0;
    value = paranthesis_expr(str);
    while (peak(str) == '*' || peak(str) == '/') {
        if (parse_token(str) == '*') {
            value *= paranthesis_expr(str);
        } else {
            value /= paranthesis_expr(str);
        }
    }
    return value;
}

double additive_expr(char **str) {
    double value = 0;
    value = multiplicative_expr(str);
    while (peak(str) == '+' || peak(str) == '-') {
        if (parse_token(str) == '+') {
            value += multiplicative_expr(str);
        } else {
            value -= multiplicative_expr(str);
        }
    }
    return value;
}

int main(int argc, char **argv) {
    char *str = argv[1];
    float result = additive_expr(&str);
    if (look(&str) != '\0') {
        fprintf(stderr, "parsing error. unexpected junk at end. got '%c'\n", peak(&str));
        exit(1);
    }
    printf("%f\n", result);
}
