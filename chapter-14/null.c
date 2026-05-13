#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct vector {
    int *memory_pointer;
    int size;
};

bool append_value(struct vector *vector, int value) {
    int *temp = realloc(vector->memory_pointer, (vector->size + 1) * sizeof(int));

    if (temp == NULL) return false;

    vector->memory_pointer = temp;
    *(vector->memory_pointer + (vector->size)) = value;
    vector->size++;

    return true;
}

bool insert_value(struct vector *vector, int value, int position) {
    if (vector->memory_pointer == NULL || position < 0 || position > vector->size) {
        return false;
    }

    // Insertion logic.
    //
    // The `position` indicates where the new value will be. Existing
    // elements will move to the right.
    //
    // If `position` is equal to `size`, we add the new value at the
    // end of the vector, this is behavior of append thus we call
    // the `append` function.

    if (position == vector->size) {
        return append_value(vector, value);
    }

    int *temp = realloc(vector->memory_pointer, sizeof(int) * (vector->size + 1));

    if (temp == NULL) return false;

    vector->memory_pointer = temp;

    for (int i = vector->size - 1; i >= position; i--) {
        vector->memory_pointer[i + 1] = vector->memory_pointer[i];
    }

    vector->memory_pointer[position] = value;
    vector->size++;

    return true;
}

bool modify_value(struct vector *vector, int new_value, int position) {
    if (position < 0 || position > vector->size || vector->memory_pointer == NULL) {
        return false;
    }

    *(vector->memory_pointer + position) = new_value;

    return true;
}

bool pop(struct vector *vector, int *popped_value) {
    if (vector->size < 1 || vector->memory_pointer == NULL) {
        return false;
    }

    *popped_value = *(vector->memory_pointer + (vector->size - 1));
    
    if (vector->size > 1) {
        // Only realloc when new size is one or greater.
        // Reallocating for a zero size is a waste of operation, so just keep the last 
        int *temp = realloc(vector->memory_pointer, (vector->size - 1) * sizeof(int));

        if (temp != NULL) {
            // Update pointer only if realloc succeeded. If it failed for some reason, we
            // just keep the slightly oversized block. Not a big deal.
            vector->memory_pointer = temp;
        }
    }

    vector->size -= 1;

    return true;
}

bool create_vector(struct vector *vector, size_t size) {
    if (size == 0) {
        // Design desicion:
        // We allow user to create "zero" sized vector, but in reality we allocate one element.
        // Purpose for this is so that we don't call `calloc` with size of zero where the
        // behavior is different between platforms.
        vector->memory_pointer = calloc(1, sizeof(int));
    } else {
        vector->memory_pointer = calloc(size, sizeof(int));
    }

    if (vector->memory_pointer == NULL) return false;

    vector->size = size;

    return true;
}

void free_vector(struct vector *vector) {
    free(vector->memory_pointer);
}

void print_vector(struct vector *vector) {
    if (vector->size <= 0) {
        return;
    }

    for (int i = 0; i < vector->size; i++) {

        printf("[%d] %d, ",i , *(vector->memory_pointer + i));
    }
    printf("\n");
}


int main(int argc, char* argv[]) {
    struct vector v;

    create_vector(&v, 0);

    print_vector(&v);

    append_value(&v, 0);
    append_value(&v, 1);
    append_value(&v, 2);
    append_value(&v, 3);
    append_value(&v, 4);
    append_value(&v, 5);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    modify_value(&v, 6, 0);
    modify_value(&v, 7, 1);
    modify_value(&v, 8, 2);
    modify_value(&v, 9, 3);
    modify_value(&v, 10, 4);
    modify_value(&v, 11, 5);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    int popped_value;

    if (pop(&v, &popped_value)) {
        printf("popped %d.\n", popped_value);
    }

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    if (pop(&v, &popped_value)) {
        printf("popped %d.\n", popped_value);
    }

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    if (pop(&v, &popped_value)) {
        printf("popped %d.\n", popped_value);
    }

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    if (pop(&v, &popped_value)) {
        printf("popped %d.\n", popped_value);
    }

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    if (pop(&v, &popped_value)) {
        printf("popped %d.\n", popped_value);
    }

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    if (pop(&v, &popped_value)) {
        printf("popped %d.\n", popped_value);
    }

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    if (pop(&v, &popped_value)) {
        printf("popped %d.\n", popped_value);
    }

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    printf("I can still access the last lol. %d\n", v.memory_pointer[0]);

    append_value(&v, 11);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    append_value(&v, 12);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    append_value(&v, 13);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    append_value(&v, 14);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    append_value(&v, 17);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    insert_value(&v, 16, 4);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    insert_value(&v, 18, 6);

    print_vector(&v);
    printf("vector size: %d\n", v.size);
    
    insert_value(&v, 10, 0);

    print_vector(&v);
    printf("vector size: %d\n", v.size);

    free_vector(&v);

    return 0;
}

// int main(int argc, char* argv[])
// {
    // int *malloc_test = (int *)malloc(1000);
    // int *p = NULL;
    // printf("p points to %p, it has a value of %d.\n", p, *p);

    // int *one_hundred_ints = (int *)malloc(sizeof(int) * 100);
    // one_hundred_ints[33596] = 0;

    // for (int i = 30000; i < 35000; i++) {
        // printf("i: %d\n", i);
        // one_hundred_ints[i] = 0;
    // }

    // int *one_hundred_ints = (int *)malloc(sizeof(int) * 100);

    // for (int i = 0; i < 100; i++) {
    //     printf("one_hundred_ints[%d] = %d\n", i, one_hundred_ints[i]);
    // }

    // free(one_hundred_ints);

    // for (int i = 0; i < 100; i++) {
    //     printf("one_hundred_ints[%d] = %d\n", i, one_hundred_ints[i]);
    // }


    // int *one_hundred_ints = (int *)malloc(sizeof(int) * 100);

    // for (int i = 0; i < 100; i++) {
    //     printf("one_hundred_ints[%d] = %d\n", i, one_hundred_ints[i]);
    // }

    // int *free_in_middle = (one_hundred_ints + 49);

    // free(free_in_middle);

    // printf("After free\n");

    // for (int i = 0; i < 49; i++) {
    //     printf("one_hundred_ints[%d] = %d\n", i, one_hundred_ints[i]);
    // }

    // return 0;
// }