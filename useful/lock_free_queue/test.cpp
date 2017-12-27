#include "Queue.h"
using namespace lock_free;

#include "stdio.h"
main()
{
    Queue *q = new Queue(1024);
    
    q->Push((void*)1236554);
    printf("%d\n", q->Pop());
}