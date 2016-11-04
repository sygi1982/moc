#ifndef __SP_H__
#define __SP_H__

template <typename T>
class SP
{
    T * pData;

public:
    SP(T *p);
    ~SP();
    T& operator* ();
    T* operator-> ();
    SP();
    SP(const SP<T>& sp);
    SP<T>& operator= (const SP<T>& sp);

    class Holder
    {
        int clients;
    public:
        Holder() : clients(0) { ; }
        ~Holder() { ; }
        void Get() { clients++; }
        bool Put() { return --clients ? false : true; }
        int State() { return clients; }
    } * holder;
};

template <typename T>
SP<T>::SP(T *p) : pData(p), holder(0)
{
    holder = new Holder();
    holder->Get();
}

template <typename T>
SP<T>::~SP()
{
    if (holder->Put())
    {
        delete pData;
        delete holder;
        pData = 0;
    }
}

template <typename T>
T& SP<T>::operator*()
{
    return *pData;
}

template <typename T>
T* SP<T>::operator-> ()
{
    return pData;
}

template <typename T>
SP<T>::SP() : pData(0), holder(0)
{
    // Create a new reference
    holder = new Holder();
    // Increment the reference count
    holder->Get();
}

template <typename T>
SP<T>::SP(const SP<T>& sp) : pData(sp.pData), holder(sp.holder)
{
    holder->Get();
}

template <typename T>
SP<T>& SP<T>::operator= (const SP<T>& sp)
{
    if (this != &sp)
    {
         if (holder->Put())
         {
             delete pData;
             delete holder;
         }
         pData = sp.pData;
         holder = sp.holder;
         holder->Get();
    }
    return *this;
}

#endif // __SP_H__
