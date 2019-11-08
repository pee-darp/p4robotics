#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define WORD_MAX_LEN 256
int call_num = 0;
// ternary search tree node
typedef struct tst_node {
    char c;
    // just in this structure definition we have to call the type
    // "struct tst_node" because the tst_node_t definition doesn't exist yet
    struct tst_node *low;
    struct tst_node *equal;
    struct tst_node *high;
} tst_node_t;

// ternary search tree
typedef struct tst {
    tst_node_t *node;
} tst_t;

tst_node_t *tst_node_create(char c) {
    tst_node_t *node = calloc(1, sizeof(*node)); // calloc also zeros out the memory
    node->c = c;
    return node;
}

void tst_node_destroy(tst_node_t *node) {
    if (!node->low && !node->equal && !node->high) {
        free(node);
        return;
    }
    if (node->low) {
        tst_node_destroy(node->low);
    }
    if (node->high) {
        tst_node_destroy(node->high);
    }
    if (node->equal) {
        tst_node_destroy(node->equal);
    }
    free(node);
}

tst_t *tst_create(void) {
    return calloc(1, sizeof(tst_t)); // calloc also zeros out the memory
}

void tst_destroy(tst_t *tst) {
    tst_node_destroy(tst->node);
    //free(tst->node);
    free(tst);
}

void tst_add(tst_t *tst, const char *word) {
    if (!tst->node) {
        tst->node = tst_node_create(word[0]);
    }
    tst_node_t *check = tst->node;
    while (true) {
        if (word[0] < check->c) {
            tst_node_t *temp = check;
            check = check->low;
            if (!check) {
                check = tst_node_create(word[0]);
                temp->low = check;
            }
        } else if (word[0] > check->c) {
            tst_node_t *temp = check;
            check = check->high;
            if (!check) {
                check = tst_node_create(word[0]);
                temp->high = check;
            }
        } else {
            tst_node_t *temp = check;
            check = check->equal;
            if (!word[0]) {
                return;
            }
            word++;
            if (!check) {
                check = tst_node_create(word[0]);
                temp->equal = check;
            }
        }
    }
}

void tst_node_search(tst_node_t *node, char *word, char *suggestion, char *sugg_start, int errs) {
    //call_num++;
    //if(node) {printf("@call %d : %c\n",call_num, node->c);}
    tst_node_t *currNode = node;
    while (currNode) {
        if (errs > 0) {
            //Insertion fix
            tst_node_search(currNode, word + 1, suggestion, sugg_start, errs - 1);

            //Deletion fix
            suggestion[0] = currNode->c;
            tst_node_search(currNode->equal, word, suggestion + 1, sugg_start, errs - 1);

            //Replacement fix
            suggestion[0] = currNode->c;
            tst_node_search(currNode->equal, word + 1, suggestion + 1, sugg_start, errs - 1);

            //Transposition fix
            if (strlen(word) >= 2) {
                char *temp = malloc(strlen(word) + 1);
                strcpy(temp, word);
                temp[0] = word[1];
                temp[1] = word[0];
                temp[2] = word[2];
                tst_node_search(currNode, temp, suggestion, sugg_start, errs - 1);
                free(temp);
            }
        }
        if (word[0] < currNode->c && word[0] != '\0') {
            tst_node_search(currNode->high, word, suggestion, sugg_start, errs);
            currNode = currNode->low;
        } else if (word[0] > currNode->c) {
            tst_node_search(currNode->low, word, suggestion, sugg_start, errs);
            currNode = currNode->high;
        } else {
            tst_node_search(currNode->low, word, suggestion, sugg_start, errs);
            tst_node_search(currNode->high, word, suggestion, sugg_start, errs);
            if (word[0] != currNode->c) {
                return;
            }
            suggestion[0] = currNode->c;
            if (currNode->c == '\0') {
                printf("%s\n", sugg_start);
                //errs--;
                return;
            }
            currNode = currNode->equal;
            word++;
            suggestion++;
        }
    }
}

void tst_search(tst_t *tst, char *word) {
    if (!tst->node) {
        return;
    }
    char word_suggestion[WORD_MAX_LEN];
    tst_node_search(tst->node, word, &word_suggestion[0], word_suggestion, 1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <word>+\n", argv[0]);
        return 1;
    }

    FILE *f = fopen("words.txt", "r");
    if (!f) {
        perror("Failed to open words.txt");
        return 1;
    }

    tst_t *tst = tst_create();

    char str_buffer[WORD_MAX_LEN];
    while (1) {
        int ret = fscanf(f, "%255s", str_buffer);
        if (ret != 1) {
            break;
        }
        tst_add(tst, str_buffer);
    }
    fclose(f);

    // tst_add(tst, "hi");
    // tst_add(tst, "hello");
    // tst_search(tst, "helol");

    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) >= WORD_MAX_LEN) {
            fprintf(stderr, "word '%s' is too long!\n", argv[i]);
            continue;
        }

        tst_search(tst, argv[i]);
        printf("\n");
    }

    tst_destroy(tst);
    return 0;
}
