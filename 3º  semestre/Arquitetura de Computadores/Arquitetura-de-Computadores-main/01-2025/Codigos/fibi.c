#include <stdio.h>

int main()
{
    int N = 10; 
    int first = 0;
    int next = 0;
    int second = 1;
    int i = 0;
    while (i < N)
    {
        printf("%d ", first);
        next = first + second;
        first = second;
        second = next;
        i++;
    }
    return 0;
}





