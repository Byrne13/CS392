#include "utils.h"
#include <stdlib.h>

/*
	You are free to use any data type you like in this file.
    However, other than the five functions declared in "utils.h",
    DO NOT create any other functions.
	
*/

int cmpr_int(void* x , void* y) {
    int a = *(int*)x;
    int b = *(int*)y;

    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    } else {
        return 0;
    }
}

int cmpr_float(void* x,void* y) {
    float a = *(float*)x;
    float b = *(float*)y;

    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    } else {
        return 0;
    }
}

void print_int(void* x) {
	int num = *(int*)x;
	printf("%d\n", num);
}

void print_float(void* x) {
	float num = *(float*)x;
	printf("%f\n", num);
}


void* read_array(char* filename, char* format, size_t* len) {
	FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File failed to open.");
        exit(EXIT_FAILURE);
    }

    size_t arrlen = 0;
    int value;
    while (fscanf(file, format, &value) == 1) {
        arrlen++;
    }

    //Memory allocation for array
    void* array = malloc(arrlen * sizeof(int));

    //Resets file pointer
    rewind(file);

    size_t i = 0;
    while (fscanf(file, format, &value) == 1) {
        //Creates a pointer to next spot in the array and sets value to it
        int* ptr = i + (int*)array;
        *ptr = value;
        i++;
    }
    
    *len = arrlen;

    fclose(file);

    return array;
}