#include <stdio.h>
#include <stdlib.h>

typedef struct vector {
    int *data;
    int size;
    int cap;
} vector_t;

void vector_init(vector_t *v) {
    v->cap = 2;
    v->data = malloc(sizeof(*(v->data)) * v->cap);
    v->size = 0;
}

void vec_append(vector_t *v, int item) {
  if (v->cap == v->size) {
      v->data = realloc(v->data, sizeof(*(v->data)) * (2*v->cap));
      v->cap = 2*v->cap;
      v->data[v->size] = item;
      v->size++;
      printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
  } else {
      v->data[v->size] = item;
      v->size++;
      printf("item added:%d | size:%d | capacity:%d\n",v->data[v->size-1], v->size, v->cap);
  }
}

int main(void) {
    vector_t v;
    vector_init(&v);
    for (int i = 1; i < 10; i++) {
        vec_append(&v,i);
    }
    free((v.data));
    return 0;
}
