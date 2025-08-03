#include <stdio.h>

int main()
{
/*
    int x,i;

    scanf("%d", &n);

    for (i=1;i<n;i++)
    {
        if (i*(i+1)*(i+2) == n)
        {

        }
    }
*/
    
    int v1,v2,seq;

    seq = 1;

    scanf("%d", &v1);

    while (v1 != 0)
    {
        scanf ("%d", &v2);
        if (v1*v1 != v2)
            seq = 0;
        

    }
    
    

    return 0;
}