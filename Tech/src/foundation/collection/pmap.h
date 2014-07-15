// pmap.h
// A set of key value pairs
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PMAP_H
#define PMAP_H

#include "prbtree.h"

#include <PFoundation/pfunctional.h>

#include <PFoundation/passet.h>
#include <PFoundation/pnew.h>


template<typename Key, typename Value, typename Op = PLess<Key> >
class P_DLLEXPORT PMap
{
public:
    PMap();
    ~PMap();
    PMap(const PMap<Key, Value, Op> &other);
    const PMap<Key, Value, Op> &operator=(const PMap<Key, Value, Op> &other); 

public:
    struct Node
    {
        Key key;
        Value value;

        Node(const Key &k, const Value &v)
            : key(k)
            , value(v)
        {
        }
    };

    class NodeLess
    {
    public:
        pbool operator()(const Node &a, const Node &b) const
        {
            return m_operator(a.key, b.key);
        }
    private:
        Op m_operator;
    };

    PRbTree<Node, NodeLess> m_rbTree;

public:
    class iterator
    {
    public:
        typename PRbTree<Node, NodeLess>::Node *m_i;
        PRbTree<Node, NodeLess>                *m_tree;

        P_INLINE iterator() 
        { m_tree = P_NULL; m_i = P_NULL; }
        P_INLINE iterator(typename PRbTree<Node, NodeLess>::Node *n, PRbTree<Node, NodeLess> *tree)    
        { m_i = n;  m_tree = tree; }
        P_INLINE iterator(const iterator &o)        
        { m_i = o.m_i;  m_tree = o.m_tree; }

        P_INLINE Key &key()           { return m_i->data().key;  }
        P_INLINE Value &value()       { return m_i->data().value; }

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
        typename PRbTree<Node, NodeLess>::Node *m_i;
        const PRbTree<Node, NodeLess>          *m_tree;

        P_INLINE const_iterator() 
        { m_tree = P_NULL; m_i = P_NULL; }
        P_INLINE const_iterator(typename PRbTree<Node, NodeLess>::Node *n, const PRbTree<Node, NodeLess> *tree)     
        { m_i = n;  m_tree = tree; }
        P_INLINE const_iterator(const iterator &o)        
        { m_i = o.m_i;  m_tree = o.m_tree; }

        P_INLINE const Key &key()  const         { return m_i->data().key;  }
        P_INLINE const Value &value() const      { return m_i->data().value; }

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
            increment();
            return *this;
        }
        P_INLINE const_iterator operator++(int) 
        {
            PASSERTINFO(m_i != P_NULL && m_i != m_tree->getHeader(), 
                "invalid const_iterator");
            const_iterator ret(*this);
            increment();
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

    iterator insert(const Key &k, const Value &v);
    void erase(iterator iter);
    iterator find(const Key &k);
    const_iterator find(const Key &k) const;
    P_INLINE const Value& operator[](const Key &k) const;
    P_INLINE Value& operator[](const Key &k);

    void clear();

    P_INLINE puint32 count() const { return m_rbTree.count(); }
    P_INLINE pbool isEmpty() const { return m_rbTree.count() == 0; }

    // STL compatible
    P_INLINE pbool empty() const { return isEmpty(); }
    P_INLINE puint32 size() const { return count(); }
    void swap(PMap<Key, Value, Op> &other);
private:
    Node *m_findNode;
};
    
template<typename Key, typename Value, typename Op>
PMap<Key, Value, Op>::PMap()
{
    m_findNode = reinterpret_cast<Node*>(PNEWARRAY(puint8, sizeof(Node)));
}

template<typename Key, typename Value, typename Op>
PMap<Key, Value, Op>::~PMap()
{
    puint8 *nn = reinterpret_cast<puint8 *>(m_findNode);
    PDELETEARRAY(nn);
}

template<typename Key, typename Value, typename Op>
PMap<Key, Value, Op>::PMap(const PMap<Key, Value, Op> &other)
    : m_rbTree(other.m_rbTree)
{
    m_findNode = reinterpret_cast<Node*>(PNEWARRAY(puint8, sizeof(Node)));
}

template<typename Key, typename Value, typename Op>
const PMap<Key, Value, Op> &PMap<Key, Value, Op>::operator=(const PMap<Key, Value, Op> &other)
{
    if (this != &other)
    {
        m_rbTree = other.m_rbTree;
    }

    return *this;
}
    
template<typename Key, typename Value, typename Op>
typename PMap<Key, Value, Op>::iterator PMap<Key, Value, Op>::insert(const Key &k, const Value &v)
{
    Node node(k, v);
    typename PRbTree<Node, NodeLess>::Node *ret = m_rbTree.insertUnique(node);
    return iterator(ret, &m_rbTree);
}

template<typename Key, typename Value, typename Op>
void PMap<Key, Value, Op>::erase(iterator iter)
{
    m_rbTree.erase(iter.m_i);
}

template<typename Key, typename Value, typename Op>
typename PMap<Key, Value, Op>::iterator PMap<Key, Value, Op>::find(const Key &k)
{
    pmemcpy(&(m_findNode->key), &k, sizeof(k));
    typename PRbTree<Node, NodeLess>::Node *ret = m_rbTree.find(*m_findNode); 
    return iterator(ret, &m_rbTree);
}

template<typename Key, typename Value, typename Op>
typename PMap<Key, Value, Op>::const_iterator PMap<Key, Value, Op>::find(const Key &k) const
{
    pmemcpy(const_cast<Key *>(&(m_findNode->key)), &k, sizeof(k));
    typename PRbTree<Node, NodeLess>::Node *ret = m_rbTree.find(*m_findNode); 
    return const_iterator(ret, &m_rbTree);
}

template<typename Key, typename Value, typename Op>
const Value& PMap<Key, Value, Op>::operator[](const Key &k) const
{
    const_iterator it = find(k);
    PASSERT(it != end());
    return it->value();
}

template<typename Key, typename Value, typename Op>
Value& PMap<Key, Value, Op>::operator[](const Key &k)
{
    iterator it = find(k);
    PASSERT(it != end());
    return it.value();
}
    
template<typename Key, typename Value, typename Op>
void PMap<Key, Value, Op>::swap(PMap<Key, Value, Op> &other)
{
    m_rbTree.swap(other.m_rbTree);
}

template<typename Key, typename Value, typename Op>
void PMap<Key, Value, Op>::clear()
{
    m_rbTree.clear();
}

#endif // !PMAP_H
