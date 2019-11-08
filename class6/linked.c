#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *prev;
    struct node *next;
    int value;
} node_t;

typedef struct list {
    node_t *start;
    node_t *end;
    int num_items;
} list_t;

list_t *list_create(void) {
    list_t *ll = malloc(sizeof(list_t));
    ll->start = malloc(sizeof(node_t));
    ll->end = malloc(sizeof(node_t));
    ll->num_items = 0;
    return ll;
}

void list_push_start(list_t *list, int value) {
    node_t *n = malloc(sizeof(node_t));
    if (list->num_items == 0) {
        list->start->next = n;
        list->end->prev = n;
        n->prev = list->start;
        n->next = list->end;
        n->value = value;
    } else {
        list->start->next->prev = n;
        n->next = list->start->next;
        n->prev = list->start;
        list->start->next = n;
        n->value = value;
    }
    list->num_items++;
    //printf("pushed %d at start\n", list->start->next->value);
}

void list_push_end(list_t *list, int value) {
    node_t *n = malloc(sizeof(node_t));
    if (list->num_items == 0) {
        list->start->next = n;
        list->end->prev = n;
        n->prev = list->start;
        n->next = list->end;
        n->value = value;
        //printf("%d\n", list->end->prev->value);
    } else {
        list->end->prev->next = n;
        n->prev = list->end->prev;
        n->next = list->end;
        list->end->prev = n;
        n->value = value;
    }
    list->num_items++;
    //printf("pushed %d at end\n", list->end->prev->value);
}

int list_pop_start(list_t *list) {
    //printf("start: %d\n", list->start->next->value);
    if (list->num_items == 0) {
        fprintf(stderr, "can't pop from an empty list!\n");
        exit(0);
    }
    node_t *temp = list->start->next;
    int value = temp->value;
    list->start->next->next->prev = list->start->next->prev;
    list->start->next = list->start->next->next;
    list->num_items--;
    //printf("popped %d from start\n", value);
    free(temp);
    return value;
}

int list_pop_end(list_t *list) {
    if (list->num_items == 0) {
        fprintf(stderr, "can't pop from an empty list!\n");
        exit(0);
    }
    node_t *temp = list->end->prev;
    int value = temp->value;
    list->end->prev->prev->next = list->end->prev->next;
    list->end->prev = list->end->prev->prev;
    list->num_items--;
    //printf("popped %d from end\n", value);
    free(temp);
    return value;
}

void list_destroy(list_t *list) {
    while (list->num_items != 0) {
        list_pop_end(list);
    }
    free(list->start);
    free(list->end);
    free(list);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "could not open %s: ", argv[1]);
        perror("");
        return 1;
    }

    list_t *list = list_create();

    while (!feof(f)) {
        char command[16];
        int res = fscanf(f, "%10s", command);
        if (res != 1) {
            if (!feof(f)) {
                fprintf(stderr, "trouble parsing command. stopping.\n");
            }
            break;
        }

        if (strcmp("push_start", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_start(list, value);
        } else if (strcmp("push_end", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_end(list, value);
        } else if (strcmp("pop_start", command) == 0) {
            int v = list_pop_start(list);
            printf("%d\n", v);
        } else if (strcmp("pop_end", command) == 0) {
            int v = list_pop_end(list);
            printf("%d\n", v);
        } else {
            fprintf(stderr, "did not recognize command '%s'. stopping.\n", command);
            break;
        }
    }

    list_destroy(list);

    fclose(f);

    return 0;
}
