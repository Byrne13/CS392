#include "insertion.h"
#include "utils.h"



/*
	We emphasize this: because this is a generic implementation, in insertion.c you must not use if-else to
	discriminate different data types in your code. Your code must be able to deal with any data type possible.
	Therefore, we limit the usage of the following primitive types:
	- int or size_t: only allowed as loop iterators;
	- char or u_int8_t: can be used as you wish;
	- Types that are used in the declarations of iSort() and iPrint(): feel free to use them for their _original_
      purpose;
	- All other types are not allowed to appear in insertion.c: no exceptions, no matter if you actually used
	  them, or what purpose it is. Once it appears, you will receive 30% reduction of the homework.

	You are free to create helper functions only in insertion.c; you must declare and implement them in the
    .c file instead of the header files. Do not change any of the header files.
	
*/


void iSort(void* base, size_t nel, size_t width, int (*compare)(void*,void*)) {
	
    for (size_t i = 1; i < nel; i++) {

		//Casts array pointer to char for arithmetic
		char* curr = (char*)base + i * width;
        for (size_t j = i; j > 0 ; j--) {
			
			char* prev = (char*)base + (j-1) * width;
			//If current value is less than prev value than swap them
			if(compare(curr, prev) < 0){
				//Swap 
				char* prv = (char*)prev;
				char* cur = (char*)curr;
				char tmp;

				for (size_t i = 0; i < width; i++) {
					tmp = prv[i];
					prv[i] = cur[i];
					cur[i] = tmp;
				}
            	curr = prev;
			} else{
				break;
			}
        }
    }

}


void iPrint(void* base, size_t nel, size_t width, void (*print)(void*)) {
	char* val = (char*)base;
    for (size_t i = 0; i < nel; i++) {
        print(val);
        val += width;
    }
}