// Exception thrown when an attempt is made to
// use an Iter that exceeds the range of the
// underlying object.
//
class OutOfRangeExc
{
    // Add functionality if needed by your application.
};


// An iterator-like class for cycling through arrays
// that are pointed to by GCPtrs. Iter pointers
// ** do not ** participate in or affect garbage
// collection. Thus, an Iter pointing to
// some object does not prevent that object
// from being recycled.
//
template <class T>
class Iter
{
    T *ptr;
    T *end;
    T *begin;
    unsigned length;
public:
    Iter()
    {
        ptr = end = begin = NULL;
        length = 0;
    }
    Iter(T *p, T *first, T *last)
    {
        ptr = p;
        end = last;
        begin = first;
        length = last - first;
    }

    unsigned size() { return length; }

    T &operator*()
    {
        if ((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return *ptr;
    }

    T *operator->()
    {
        if ((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return ptr;
    }
 
    Iter operator++()
    {
        ptr++;
        return *this;
    }

    Iter operator--()
    {
        ptr--;
        return *this;
    }

    Iter operator++(int notused)
    {
        T *tmp = ptr;
        ptr++;
        return Iter<T>(tmp, begin, end);
    }

    Iter operator--(int notused)
    {
        T *tmp = ptr;
        ptr--;
        return Iter<T>(tmp, begin, end);
    }

    T &operator[](int i)
    {
        if ((i < 0) || (i >= (end - begin)))
            throw OutOfRangeExc();
        return ptr[i];
    }
  
    bool operator==(Iter op2)
    {
        return ptr == op2.ptr;
    }
    bool operator!=(Iter op2)
    {
        return ptr != op2.ptr;
    }
    bool operator<(Iter op2)
    {
        return ptr < op2.ptr;
    }
    bool operator<=(Iter op2)
    {
        return ptr <= op2.ptr;
    }
    bool operator>(Iter op2)
    {
        return ptr > op2.ptr;
    }
    bool operator>=(Iter op2)
    {
        return ptr >= op2.ptr;
    }

    Iter operator-(int n)
    {
        ptr -= n;
        return *this;
    }

    Iter operator+(int n)
    {
        ptr += n;
        return *this;
    }

    int operator-(Iter<T> &itr2)
    {
        return ptr - itr2.ptr;
    }
};
