#include <stdio.h>



void inc (int n)
{
  n++ ;
  printf ("n vale %d\n", n) ;
}

int main()
{
  int a = 0 ;
  printf ("a vale %d\n", a) ;
  inc (a) ;
  printf ("a vale %d\n", a) ;
  return (0) ;
}


/*

void inc (int *n)
{
  (*n)++ ;
}
 
int main ()
{
  int a = 0 ;
  printf ("a vale %d\n", a) ;
  inc (&a) ; 
  printf ("a vale %d\n", a) ;
  return (0) ;
}

*/


  
