#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *property1 = "age";
    char *property2 = "height";
    char *property3 = "smart";
    char *property4 = "foobab";
    char *property5 = "foobabi";
    char *property6 = "foobability";

    int prop_value = 0;
    int *prop_value_p = &prop_value;

    hashtable_t *student = hashtable_create();

    hashtable_set(student, property1, 21);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, property2, 170);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, property3, 90);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, property4, 30);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, property5, 30);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, property6, 1005);

    hashtable_get(student, property6, prop_value_p);

    printf("%d\n", *prop_value_p);

    hashtable_destroy(student);
    return 0;
}
