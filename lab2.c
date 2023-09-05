// Credit: Shudong Hao

#include <stdio.h>

// TODO: addone() must accept parameters that include a function pointer,
// the number of elements in the array and the size of each element
// it must NOT accept any parameters directly revealing the data type

// THEN: add the functionality to find the maximum in an array. 
// The function should return the index of the maximum in the array.
// Implementing it for one type is sufficient.


int main() {

    int test1[] = {1,2,3,4,5};
    addone(, &test1, sizeof(test1)/sizeof(test1[0]), sizeof(test1[0]));
    for (size_t i = 0; i < 5; i ++) printf("%d\n", test1[i]);


    //double test2[] = {0.3, -0.2, 1.34};
    //addone(/* pass parameters */);
    //for (size_t i = 0; i < 3; i ++) printf("%lf\n", test2[i]);

    //char test3[] = {'3', 'a', 'Z', '?'};
    //addone(/* pass parameters */);
    //for (size_t i = 0; i < 4; i ++) printf("%c\n", test3[i]);

    

    return 0;
}

void addone(void (*fun_ptr)(void *), const void *a, int length, int elem_size){
    
    printf("%ld\n", length);
    
    for (size_t i = 0; i < length; i ++){
        printf("%c\n", ((char*)a[i]);
        
        //printf("hello\n"/*,((char*)a[i]*/);
    } 
    

}
/*

void addone(double array){



}
void addone(char array){



}
*/