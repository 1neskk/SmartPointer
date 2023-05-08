#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
#include "Details.h"
#include "Iterator.h"
/*
    Pointer implements a pointer type that uses
    garbage collection to release unused memory.
    A Pointer must only be used to point to memory
    that was dynamically allocated using new.
    When used to refer to an allocated array,
    specify the array size.
*/
template <class T, int size = 0>
class Pointer{
public:

    typedef Iter<T> GCiterator;

    Pointer(){
        Pointer(NULL);
    }
    Pointer(T*);

    Pointer(const Pointer &);

    ~Pointer();

    static bool collect();
 
    T *operator=(T *t);

    Pointer &operator=(Pointer &rv);

    T &operator*(){
        return *addr;
    }
    T *operator->() { return addr; }
    T &operator[](int i){ return addr[i];}
    operator T *() { return addr; }
    Iter<T> begin(){
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr, addr, addr + _size);
    }
    Iter<T> end(){
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr + _size, addr, addr + _size);
    }

    static int refContainerSize() { return refContainer.size(); }
    static void showlist();
    static void shutdown();

private:
    static std::list<PtrDetails<T> > refContainer;
    T *addr;
    bool isArray;
    unsigned arraySize; 
    static bool first;
    typename std::list<PtrDetails<T> >::iterator findPtrInfo(T *ptr);

};

template <class T, int size>
std::list<PtrDetails<T> > Pointer<T, size>::refContainer;
template <class T, int size>
bool Pointer<T, size>::first = true;

template<class T,int size>
Pointer<T,size>::Pointer(T *t){
    if (first)
        atexit(shutdown);
    first = false;

    typename std::list <PtrDetails<T> >::iterator p = findPtrInfo(t);
    if (p != refContainer.end())
        p->refcount++;
    else
    {
        refContainer.push_back(PtrDetails<T>(t, size));
    }
    addr = t;
    isArray = (size > 0) ? true : false;
    arraySize = size;
}

template< class T, int size>
Pointer<T,size>::Pointer(const Pointer &ob){

    typename std::list<PtrDetails<T> >::iterator p;
    p = findPtrInfo(ob.addr);
    p->refcount++;
    addr = ob.addr;
    isArray = ob.isArray;
    arraySize = ob.arraySize;

}

template <class T, int size>
Pointer<T, size>::~Pointer(){
    typename std::list<PtrDetails<T> >::iterator p;
    p = findPtrInfo(addr);
    if (p->refcount)
        p->refcount--;
    collect();
}

template <class T, int size>
bool Pointer<T, size>::collect(){
    typename std::list<PtrDetails<T> >::iterator p;
    bool memfreed = false;
    do
    {
        for (p = refContainer.begin(); p != refContainer.end(); p++)
        {
            if (p->refcount > 0)
                continue;
            if (p->memPtr)
            {
                if (p->isArray)
                {
                    delete[] p->memPtr;
                }
                else
                {
                    delete p->memPtr;
                    refContainer.erase(p--);
                }
            }
            break;
        }
    }while (p != refContainer.end());
    return memfreed;
}

template <class T, int size>
T *Pointer<T, size>::operator=(T *t)
{
    typename std::list<PtrDetails<T> >::iterator p = findPtrInfo(addr);
    p->refcount--;
    p = findPtrInfo(t);
    if (p != refContainer.end())
        p->refcount++;
    else
    {
        PtrDetails<T> newPtr(t, size);
        refContainer.push_back(newPtr);
        if (size > 0)
        {
            arraySize = size;
            isArray = true;
        }
        else{
            refContainer.push_back(newPtr);
        }
    }
    addr = t;
    return t;
}

template <class T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(Pointer &rv){
    typename std::list<PtrDetails<T> >::iterator p;
    p = findPtrInfo(addr);
    p->refcount--;
    p = findPtrInfo(rv.addr);
    p->refcount++;
    addr = rv.addr;
    return rv;
}

template <class T, int size>
void Pointer<T, size>::showlist(){
    typename std::list<PtrDetails<T> >::iterator p;
    std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n";
    std::cout << "memPtr refcount value\n ";
    if (refContainer.begin() == refContainer.end())
    {
        std::cout << " Container is empty!\n\n ";
    }
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        std::cout << "[" << (void *)p->memPtr << "]"
             << " " << p->refcount << " ";
        if (p->memPtr)
            std::cout << " " << *p->memPtr;
        else
            std::cout << "---";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <class T, int size>
typename std::list<PtrDetails<T> >::iterator
Pointer<T, size>::findPtrInfo(T *ptr){
    typename std::list<PtrDetails<T> >::iterator p;
    for (p = refContainer.begin(); p != refContainer.end(); p++)
        if (p->memPtr == ptr)
            return p;
    return p;
}

template <class T, int size>
void Pointer<T, size>::shutdown(){
    if (refContainerSize() == 0)
        return;
    typename std::list<PtrDetails<T> >::iterator p;
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        p->refcount = 0;
    }
    collect();
}
