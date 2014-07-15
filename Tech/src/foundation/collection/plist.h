// plist.h
// double-linked list in C++ template. 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PLIST_H
#define PLIST_H

#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>

#define P_LIST_IS_VALID_ITERATOR(i) \
    {PASSERTINFO(i.m_i != P_NULL, "invalid iterator");}

template<typename T>
class P_DLLEXPORT PList
{
public:
    struct Node
    {
        Node* m_p; // previous node
        Node* m_n; // next node
        T m_t;
    };

    typedef T  type_value;
    typedef T* pointer;
    typedef T& reference;

    PList();
    PList(const PList<T>& o);
    ~PList();

    class iterator
    {
    public:
        Node* m_i;

        P_INLINE iterator()                   { m_i = P_NULL; }
        P_INLINE iterator(Node* n)            { m_i = n;  }
        P_INLINE iterator(const iterator& o)  { m_i = o.m_i;  }

        P_INLINE T& operator*()  const         { return m_i->m_t;  };
        P_INLINE T* operator->() const         { return &m_i->m_t; };

        P_INLINE pbool operator==(const iterator &o) const { return m_i == o.m_i; }
        P_INLINE pbool operator!=(const iterator &o) const { return m_i != o.m_i; }
        P_INLINE const iterator& operator+=(puint32 i) { while (i > 0) { m_i = m_i->m_n; i--; } return *this; }
        P_INLINE const iterator& operator-=(puint32 i) { while (i > 0) { m_i = m_i->m_p; i--; } return *this; }

        P_INLINE iterator& operator++() 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            m_i = m_i->m_n;
            return *this;
        }
        P_INLINE iterator operator++(int) 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            iterator ret(*this);
            m_i = m_i->m_n;
            return ret;
        }
        P_INLINE iterator& operator--() 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            m_i = m_i->m_p;
            return *this;
        }
        P_INLINE iterator operator--(int) 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            iterator ret(*this);
            m_i = m_i->m_p;
            return ret;
        }
    };
    
    class const_iterator
    {
    public:
        Node* m_i;

        P_INLINE const_iterator()                         { m_i = P_NULL; }
        P_INLINE const_iterator(Node* n)                  { m_i = n;  }
        P_INLINE const_iterator(const const_iterator& o)  { m_i = o.m_i;  }

        P_INLINE const T& operator*()  const         { return m_i->m_t;  };
        P_INLINE const T* operator->() const         { return &m_i->m_t; };

        P_INLINE pbool operator==(const const_iterator &o) const { return m_i == o.m_i; }
        P_INLINE pbool operator!=(const const_iterator &o) const { return m_i != o.m_i; }
        P_INLINE const const_iterator& operator+=(puint32 i) { while (i > 0) { m_i = m_i->m_n; i--; } return *this; }
        P_INLINE const const_iterator& operator-=(puint32 i) { while (i > 0) { m_i = m_i->m_p; i--; } return *this; }

        P_INLINE const_iterator& operator++() 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            m_i = m_i->m_n;
            return *this;
        }
        P_INLINE const_iterator operator++(int) 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            const_iterator ret(*this);
            m_i = m_i->m_n;
            return ret;
        }
        P_INLINE const_iterator& operator--() 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            m_i = m_i->m_p;
            return *this;
        }
        P_INLINE const_iterator operator--(int) 
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            const_iterator ret(*this);
            m_i = m_i->m_p;
            return ret;
        }
    };

    P_INLINE iterator begin() { return iterator(m_h->m_n);  }
    P_INLINE iterator end()   { return iterator(m_t);         }
    P_INLINE const_iterator begin()   const { return const_iterator(m_h->m_n); }
    P_INLINE const_iterator end()     const { return const_iterator(m_t);     }

    iterator insert(iterator before, const T& t);
    void erase(iterator pos);
    void erase(iterator first, iterator last);
    
    void append(const T& t);
    void append(const PList<T>& t);
    void prepend(const T& t);
    void prepend(const PList<T>& t);
    P_INLINE void removeFirst() { erase(begin());                    }
    P_INLINE void removeLast()  { iterator i = end(); --i; erase(i); }
    T& first();
    const T& first() const;
    T& last();
    const T& last() const;
    
    P_INLINE puint32 count() const { return m_size;         }
    P_INLINE pbool isEmpty() const   { return m_size == 0;    }
    void clear();           
    iterator find(const T& t); 
    const_iterator find(const T& t) const;

    // STL compatible
    P_INLINE void push_back(const T& t)  { append(t);        }
    P_INLINE void push_front(const T& t) { prepend(t);       }
    P_INLINE T& front()                  { return first();   }
    P_INLINE const T& front() const      { return first();   }
    P_INLINE T& back()                   { return last();    }
    P_INLINE const T& back() const       { return last();    }
    P_INLINE void pop_front()            { removeFirst();    }
    P_INLINE void pop_back()             { removeLast();     }
    P_INLINE pbool empty() const          { return isEmpty(); }
    P_INLINE puint32 size() const       { return count();   }
    void swap(PList<T>& o);
    
    const PList<T>& operator=(const PList<T>& o);
    PList<T> &operator+=(const PList<T> &o); 
    P_INLINE PList<T> operator+(const PList<T> &l) const { PList n = *this; n += l; return n; }
    P_INLINE PList<T>& operator+=(const T &t)             { append(t); return *this;            }
    P_INLINE PList<T>& operator<< (const T &t)            { append(t); return *this;            }
    P_INLINE PList<T>& operator<<(const PList<T> &l)     { *this += l; return *this;           }

protected:
    Node* m_h; // head;
    Node* m_t; // tail
    puint32 m_size; // number of elements

    Node* alloc(const T& t);
    void release(Node* n);
};
    
template<typename T>
PList<T>::PList()
{
    m_h = reinterpret_cast<Node*>(PNEW(puint8 [sizeof(Node)]));
    m_t = reinterpret_cast<Node*>(PNEW(puint8 [sizeof(Node)]));

    m_h->m_p = P_NULL;
    m_h->m_n = m_t;
    m_t->m_p = m_h;
    m_t->m_n = P_NULL;

    m_size = 0;
}

template<typename T>
PList<T>::PList(const PList<T>& o)
{
    m_h = reinterpret_cast<Node*>(PNEWARRAY(puint8, sizeof(Node)));
    m_t = reinterpret_cast<Node*>(PNEWARRAY(puint8, sizeof(Node)));
    m_h->m_p = P_NULL;
    m_h->m_n = m_t;
    m_t->m_p = m_h;
    m_t->m_n = P_NULL;

    if (o.m_size > 0)
    {
        append(o);
    }
    else
    {
        m_size = 0;
    }
}

template<typename T>
PList<T>::~PList()
{
    clear();
    
    puint8* h = reinterpret_cast<puint8*>(m_h);
    puint8* t = reinterpret_cast<puint8*>(m_t);
    PDELETEARRAY(h);
    PDELETEARRAY(t);
}
    
template <typename T>
typename PList<T>::iterator PList<T>::insert(iterator before, const T& t)
{
    P_LIST_IS_VALID_ITERATOR(before);

    if (before == end())
    {
        // It is to add item to the tail.
        append(t);
        return iterator(m_t->m_p);
    }

    PList<T>::Node* p = before.m_i;
    PList<T>::Node* n = alloc(t);

    n->m_n = p->m_n;
    n->m_p = p;
    p->m_n->m_p = n;
    p->m_n = n;

    ++m_size;

    return iterator(n);
}

template <typename T>
void PList<T>::erase(iterator pos)
{
    P_LIST_IS_VALID_ITERATOR(pos);

    if (pos == end() || m_size == 0)
    {
        return ;
    }

    PList<T>::Node* p = pos.m_i;
    p->m_p->m_n = p->m_n;
    p->m_n->m_p = p->m_p;
    release(p);

    --m_size;
}

template <typename T>
void PList<T>::erase(iterator first, iterator last)
{
    P_LIST_IS_VALID_ITERATOR(first);
    P_LIST_IS_VALID_ITERATOR(last);

    PList<T>::Node* p = first.m_i->m_p;
    PList<T>::Node* i = first.m_i;
    PList<T>::Node* e = last.m_i;
    while (i != e)
    {
        PList<T>::Node* tmp = i->m_n;
        release(i);
        i = tmp;
        --m_size;
    }

    p->m_n = e;
    e->m_p = p;
} 

template <typename T>
void PList<T>::append(const T& t)
{
    PList<T>::Node* n = alloc(t);

    n->m_p = m_t->m_p;
    n->m_n = m_t;
    m_t->m_p->m_n = n;
    m_t->m_p = n;

    ++m_size;
}

template <typename T>
void PList<T>::append(const PList<T>& t)
{
    PASSERTINFO(this != &t, "Can't append list to itself");
    PList<T>::const_iterator i, b = t.begin(), e = t.end();
    for (i = b; i != e; ++i)
    {
        append(*i);
    }
}

template <typename T>
void PList<T>::prepend(const T& t)
{
    PList<T>::Node* n = alloc(t);

    n->m_p = m_h;
    n->m_n = m_h->m_n;
    m_h->m_n->m_p = n;
    m_h->m_n = n;

    ++m_size;
}

template <typename T>
void PList<T>::prepend(const PList<T>& t)
{
    PASSERTINFO(this != &t, "can't append list to itself");
    PList<T>::const_iterator i = t.end(), b = t.begin(), e = t.end();
    do
    {
        --i;
        prepend(*i);
    } while (i != b);
}

template<typename T>
T& PList<T>::first()
{
    PASSERTINFO(m_size != 0, "empty list");

    Node* n = m_h->m_n;
    return n->m_t;
}

template<typename T>
const T& PList<T>::first() const
{
    PASSERTINFO(m_size != 0, "empty list");

    Node* n = m_h->m_n;
    return n->m_t;
}

template<typename T>
T& PList<T>::last()
{
    PASSERTINFO(m_size != 0, "empty list");

    Node* n = m_t->m_p;
    return n->m_t;
}

template<typename T>
const T& PList<T>::last() const
{
    PASSERTINFO(m_size != 0, "empty list");

    Node* n = m_t->m_p;
    return n->m_t;
}

template <typename T>
void PList<T>::swap(PList<T>& t)
{
    pSwap(m_h, t.m_h);
    pSwap(m_t, t.m_t);
    pSwap(m_size, t.m_size);
}

template <typename T>
void PList<T>::clear()
{
    PList<T>::Node* i = m_h->m_n;
    PList<T>::Node* e = m_t;
    while (i != e)
    {
        PList<T>::Node* tmp = i->m_n;
        release(i);
        i = tmp;
    }
    m_size = 0;

    m_h->m_p = P_NULL;
    m_h->m_n = m_t;
    m_t->m_p = m_h;
    m_t->m_n = P_NULL;

}
    
template<typename T>
typename PList<T>::iterator PList<T>::find(const T& t) 
{
    PList<T>::Node* i = m_h->m_n;
    PList<T>::Node* e = m_t;
    while (i != e)
    {
        if (i->m_t == t)
        {
            return iterator(i);
        }
        i = i->m_n;
    }

    return end();
}

template<typename T>
typename PList<T>::const_iterator PList<T>::find(const T& t) const
{
    PList<T>::Node* i = m_h->m_n;
    PList<T>::Node* e = m_t;
    while (i != e)
    {
        if (i->m_t == t)
        {
            return const_iterator(i);
        }
        i = i->m_n;
    }

    return end();
}

template <typename T>
PList<T>& PList<T>::operator+=(const PList<T> &o)
{
    append(o);
    return *this;
}
    
template <typename T>
const PList<T>& PList<T>::operator=(const PList<T>& o)
{
    if (this != &o)
    {
        clear();
        append(o);
    }

    return *this;
}

template <typename T>
typename PList<T>::Node* PList<T>::alloc(const T& t)
{
    Node* n = reinterpret_cast<Node*>(PNEW(puint8 [sizeof(Node)]));
    if (PTypeInfo<T>::isComplex)
    {
        new (&(n->m_t)) T(t);
    }
    else
    {
        n->m_t = t;
    }

    n->m_n = P_NULL;
    n->m_p = P_NULL;

    return n;
}

template <typename T>
void PList<T>::release(Node* n)
{
    if (PTypeInfo<T>::isComplex)
    {
        n->m_t.~T();
    }

    puint8 *tn = reinterpret_cast<puint8 *>(n);
    PDELETEARRAY(tn);
}


#endif // !P_LIST_H
