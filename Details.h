// This class defines an element that is stored
// in the garbage collection information list.
//
template <class T>
class PtrDetails
{
public:
    unsigned refcount;
    T *memPtr;
    bool isArray = false;
    unsigned arraySize;

    PtrDetails(T* p, unsigned size = 0)
    {
        refcount = 1;
        memPtr = p;
        if (size != 0)
        {
            isArray = true;
            arraySize = size;
        }else
        {
            isArray = false;
            arraySize = 0;
        }
    }
};

template <class T>
bool operator==(const PtrDetails<T> &ob1, const PtrDetails<T> &ob2)
{
    return (ob1.memPtr == ob2.memPtr) && (ob1.arraySize == ob2.arraySize);
}
