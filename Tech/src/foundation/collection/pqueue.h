// pqueue.h
// A queue is a sorted list.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PQUEUE_H
#define PQUEUE_H

#include <PFoundation/plist.h>

// TODO: write a binary operator function class and make PQueueLess derive this 
// binary operator function class.
template<typename T>
class PQueueLess
{
public:
    pbool operator()(const T &left, const T &right) const
    { return left < right; }
};

template<typename T, typename LessOp = PQueueLess<T> >
class P_DLLEXPORT PQueue : public PList<T>
{
public:
    PQueue();
    ~PQueue();
    PQueue(const PQueue &other);
    const PQueue &operator=(const PQueue &other);

    class iterator : public PList<T>::iterator 
    {
    public:
        P_INLINE iterator() : PList<T>::iterator() {}
        P_INLINE iterator(typename PList<T>::Node *n) : PList<T>::iterator(n) {}
        P_INLINE iterator(const iterator &o) : PList<T>::iterator(o) {}
    };
    class const_iterator : public PList<T>::const_iterator
    {
        P_INLINE const_iterator() : PList<T>::const_iterator() {}
        P_INLINE const_iterator(typename PList<T>::Node *n) : PList<T>::const_iterator(n) {}
        P_INLINE const_iterator(const iterator &o) : PList<T>::const_iterator(o) {}
    };
	
	P_INLINE iterator begin() { return iterator(this->m_h->m_n);  }
    P_INLINE iterator end()   { return iterator(this->m_t);         }
    P_INLINE const_iterator begin()   const { return const_iterator(this->m_h->m_n); }
    P_INLINE const_iterator end()     const { return const_iterator(this->m_t);     }

    void pop();
    void insert(const T &t);
    iterator find(const T& t);
    const_iterator find(const T& t) const;

private:
    // Disable some PList functions which should be exposed in PQueue.
    iterator insert(iterator before, const T& t) { return end(); }
    void append(const T& t) {}
    void append(const PList<T>& t) {}
    void prepend(const T& t) {}
    void prepend(const PList<T>& t) {}
    P_INLINE void push_back(const T& t)  { }
    P_INLINE void push_front(const T& t) { }

private:
    LessOp   m_lessOp;
};

template<typename T, typename LessOp>
PQueue<T, LessOp>::PQueue()
    : PList<T>()
{
    m_lessOp = LessOp();
}

template<typename T, typename LessOp>
PQueue<T, LessOp>::~PQueue()
{
}

template<typename T, typename LessOp>
PQueue<T, LessOp>::PQueue(const PQueue &other)
    : PList<T>(other)
{
    m_lessOp = LessOp(other.m_lessOp);
}

template<typename T, typename LessOp>
const PQueue<T, LessOp> &PQueue<T, LessOp>::operator=(const PQueue<T, LessOp> &other)
{
    if (this != &other)
    {
        PList<T>::operator=(other);
        m_lessOp = LessOp(other.m_lessOp);
    }

    return *this;
}

template<typename T, typename LessOp>
void PQueue<T, LessOp>::pop()
{
    PList<T>::removeFirst();
}

template<typename T, typename LessOp>
void PQueue<T, LessOp>::insert(const T &t)
{
    typename PList<T>::Node* i = this->m_h->m_n;
    typename PList<T>::Node* e = this->m_t;
    while (i != e && m_lessOp(i->m_t, t))
    {
        i = i->m_n;
    }

    if (i == e)
    {
        PList<T>::append(t);
    }
    else
    {
        PList<T>::insert(iterator(i->m_p), t);
    }
}

template<typename T, typename LessOp>
typename PQueue<T, LessOp>::iterator PQueue<T, LessOp>::find(const T &t)
{
    typename PList<T>::iterator it = PList<T>::find(t);
    return iterator(it.m_i);
}

template<typename T, typename LessOp>
typename PQueue<T, LessOp>::const_iterator PQueue<T, LessOp>::find(const T &t) const
{
    typename PList<T>::const_iterator it = PList<T>::find(t);
    return const_iterator(it.m_i);
}

#endif // !PQUEUE_H

