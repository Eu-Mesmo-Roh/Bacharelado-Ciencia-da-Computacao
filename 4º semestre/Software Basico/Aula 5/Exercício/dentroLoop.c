#include <stdio.h>

#define M 4

//declaring the variable inside the loop initialization
int main(){
    int array[M];
    int sum = 0;

    for(size_t i=0; i<M; ++i)
        array[i] = i;

    for(size_t i=0; i<M; ++i){
        printf("%d ", array[i]);
        sum += array[i];
    }
    printf("\nSum: %d\n", sum);

    return 0;
}