// pset.h
// A set of unique values.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PSET_H
#define PSET_H

#include "prbtree.h"

#include <PFoundation/pfunctional.h>

#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>

template<typename T, typename Op = PLess<T> >
class P_DLLEXPORT PSet
{
public:
    PSet();
    ~PSet();
    PSet(const PSet<T, Op> &other);
    const PSet<T, Op> &operator=(const PSet<T, Op> &other); 

private:
    PRbTree<T, Op> m_rbTree;

public:
    class iterator
    {
    public:
        typename PRbTree<T, Op>::Node *m_i;
        PRbTree<T, Op>                *m_tree;

        P_INLINE iterator() 
        { m_tree = P_NULL; m_i = P_NULL; }
        P_INLINE iterator(typename PRbTree<T, Op>::Node *n, PRbTree<T, Op> *tree)    
        { m_i = n;  m_tree = tree; }
        P_INLINE iterator(const iterator &o)        
        { m_i = o.m_i;  m_tree = o.m_tree; }

        P_INLINE T &operator*() { return m_i->data(); };
        P_INLINE T *operator->() { return &(m_i->data()); };
        P_INLINE pbool operator==(const iterator &o) const { return m_i == o.m_i; }
        P_INLINE pbool operator!=(const iterator &o) const { return m_i != o.m_i; }

        P_INLINE void increment()
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            m_i = m_tree->successor(m_i);
        }

        P_INLINE iterator &operator++() 
        {
            PASSERTINFO(m_i != P_NULL && m_i != m_tree->getHeader(), 
                "invalid iterator");
            increment();
            return *this;
        }
        P_INLINE iterator operator++(int) 
        {
            PASSERTINFO(m_i != P_NULL && m_i != m_tree->getHeader(), 
                "invalid iterator");
            iterator ret(*this);
            increment();
            return ret;
        }
    };

    class const_iterator
    {
    public:
        typename PRbTree<T, Op>::Node *m_i;
        PRbTree<T, Op> *m_tree;

        P_INLINE const_iterator() 
        { m_tree = P_NULL; m_i = P_NULL; }
        P_INLINE const_iterator(typename PRbTree<T, Op>::Node *n, PRbTree<T, Op> *tree)    
        { m_i = n;  m_tree = tree; }
        P_INLINE const_iterator(const iterator &o)        
        { m_i = o.m_i;  m_tree = o.m_tree; }
        P_INLINE const_iterator(const const_iterator &o)        
        { m_i = o.m_i;  m_tree = o.m_tree; }

        P_INLINE const T &operator*() { return m_i->data(); };
        P_INLINE const T *operator->() { return &(m_i->data()); };
        P_INLINE pbool operator==(const const_iterator &o) const { return m_i == o.m_i; }
        P_INLINE pbool operator!=(const const_iterator &o) const { return m_i != o.m_i; }

        P_INLINE void increment()
        {
            PASSERTINFO(m_i != P_NULL, "invalid iterator");
            m_i = m_tree->successor(m_i);
        }

        P_INLINE const_iterator &operator++() 
        {
            PASSERTINFO(m_i != P_NULL && m_i != m_tree->getHeader(), 
                "invalid const_iterator");
            this->increament();
            return *this;
        }
        P_INLINE const_iterator operator++(int) 
        {
            PASSERTINFO(m_i != P_NULL && m_i != m_tree->getHeader(), 
                "invalid const_iterator");
            const_iterator ret(*this);
            this->increament();
            return ret;
        }
    };

    P_INLINE iterator begin() 
    { return iterator(m_rbTree.getMin(), &m_rbTree); }
    P_INLINE const_iterator begin() const 
    { return const_iterator(m_rbTree.getMin(), &m_rbTree); }
    P_INLINE iterator end() 
    { return iterator(m_rbTree.getHeader(), &m_rbTree); }
    P_INLINE const_iterator end() const 
    { return const_iterator(m_rbTree.getHeader(), &m_rbTree); }
     
    iterator insert(const T &t);
    void erase(iterator iter);
    iterator find(const T &t);
    const_iterator find(const T &t) const;

    void clear();

    P_INLINE puint32 count() const { return m_rbTree.count(); }
    P_INLINE pbool isEmpty() const { return m_rbTree.count() == 0; }

    // STL compatible
    P_INLINE pbool empty() const { return isEmpty(); }
    P_INLINE puint32 size() const { return count(); }
    void swap(PSet<T, Op> &other);
};

template<typename T, typename Op>
PSet<T, Op>::PSet()
{
}

template<typename T, typename Op>
PSet<T, Op>::~PSet()
{
}

template<typename T, typename Op>
PSet<T, Op>::PSet(const PSet<T, Op> &other)
    : m_rbTree(other.m_rbTree)
{
    
}

template<typename T, typename Op>
const PSet<T, Op> &PSet<T, Op>::operator=(const PSet<T, Op> &other)
{
    if (this != &other)
    {
        m_rbTree = other.m_rbTree;
    }

    return *this;
}
    
template<typename T, typename Op>
typename PSet<T, Op>::iterator PSet<T, Op>::insert(const T &t)
{
    return iterator(m_rbTree.insertUnique(t), &m_rbTree);
}

template<typename T, typename Op>
void PSet<T, Op>::erase(iterator iter)
{
    m_rbTree.erase(iter.m_i);
}

template<typename T, typename Op>
typename PSet<T, Op>::iterator PSet<T, Op>::find(const T &t)
{
    typename PRbTree<T, Op>::Node *ret = m_rbTree.find(t);        
    return iterator(ret, &m_rbTree);
}

template<typename T, typename Op>
typename PSet<T, Op>::const_iterator PSet<T, Op>::find(const T &t) const
{
    typename PRbTree<T, Op>::Node *ret = m_rbTree.find(t);        
    return const_iterator(ret, &m_rbTree);
}

template<typename T, typename Op>
void PSet<T, Op>::swap(PSet<T, Op> &other)
{
    m_rbTree.swap(other.m_rbTree);
}

template<typename T, typename Op>
void PSet<T, Op>::clear()
{
    return m_rbTree.clear();
}

#endif // !PSET_H
