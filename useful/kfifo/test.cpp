#include "zfifo.h"

int main()  
{  
    zFifo<int*> zf;  
    int a = 1;  
    zf.enqueue(&a);  
    printf("a=%d\n", &a);  
    int* b = 0;  
    zf.dequeue(&b);  
    printf("b=%d\n", b);  
  
    return 0;  
}