#include <stdio.h>

#define M 4

//declaring the variable outside the loop
int main(){
    int array[M];
    int sum = 0;
    size_t i;

    for(i=0; i<M; ++i)
        array[i] = i;

    for(i=0; i<M; ++i){
        printf("%d ", array[i]);
        sum += array[i];
    }
    printf("\nSum: %d\n", sum);

    return 0;
}