#include <stdio.h>
#include <math.h>

/*
*/

int main()
{
    float b, c, raizdiscriminante;
    scanf("%f", &b);
    scanf("%f", &c);

    raizdiscriminante = sqrt(b*b - 4*c);
    printf("%f\n", (b - raizdiscriminante) / 2);
    printf("%f\n", (b + raizdiscriminante) / 2);
    
    return 0;

}
