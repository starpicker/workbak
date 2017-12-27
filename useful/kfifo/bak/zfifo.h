#include "kfifo_lite.h"

template <typename T>  
class zFifo  
{  
    private:  
        kfifo_lite* _kfifo_lite;  
    public:  
        zFifo()  
        {  
            _kfifo_lite = kfifo_lite_alloc(MAX_KFIFO_SIZE);  
        }  
        ~zFifo()  
        {  
            if(NULL != _kfifo_lite)  
              kfifo_lite_free(_kfifo_lite);  
        }  
        bool enqueue(T data);  
        bool dequeue(T* data);  
};  
  
template <typename T>  
bool zFifo<T>::enqueue(T data)  
{  
    int ret = 0;  
    ret = __kfifo_lite_put(_kfifo_lite, (const unsigned char *)&data, sizeof(T));  
    if(ret == 0)  
        return true;  
    else  
        return false;  
}  
  
template <typename T>  
bool zFifo<T>::dequeue(T* data)  
{
    int ret = __kfifo_lite_get(_kfifo_lite, (unsigned char *)data, sizeof(T));  
    if(ret == 0)  
        return true;  
    else  
        return false;  
}  