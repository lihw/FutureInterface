// prbtree.h
// A red black tree.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com and Hongwei Li lihw81@gmail.com
//


#ifndef PRBTREE_H
#define PRBTREE_H

#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>

#define P_RBTREE_IS_VALID_ITERATOR(i) \
    {PASSERTINFO(i.m_i != P_NULL, "invalid iterator");}


template<typename T, typename Op>
class P_DLLEXPORT PRbTree
{
public:
    enum PRbColor
    {
        P_COLOR_RED,
        P_COLOR_BLACK,
    };

public:
    struct Node
    {
        Node        *m_left;
        Node        *m_right;
        Node        *m_parent;
        PRbColor     m_color;
        T            m_data;
        
        T& data() 
        {
            return m_data;
        }

        static Node* findSuccessor(Node *node) 
        {
            Node *ret = P_NULL;

            if (node == P_NULL)
            {
                return P_NULL;
            }


            if (node->m_right == P_NULL)
            {
                ret = node->m_parent;
                while (node == ret->m_right)
                {
                    node = ret; 
                    ret = ret->m_parent;
                }
            }
            else
            {
                ret = node->m_right;
                while (ret->m_left != P_NULL)
                {
                    ret = ret->m_left;
                }
            }

            return ret;
        }
    };


    PRbTree();
    ~PRbTree();
    PRbTree(const PRbTree &other);
    const PRbTree &operator=(const PRbTree &other);

    Node* insert(const T &t);
    Node* insertUnique(const T &t);
    void erase(Node *node);

    Node* find(const T &t) const;
    P_INLINE Node *getHeader() const { return m_header; }
    P_INLINE puint32 count() const { return m_count; }
    Node *successor(Node *node);
    Node *successor(Node *node) const;
    void clear();
    void swap(PRbTree &other);

    P_INLINE Node *getMin() const
    {
        if (m_root == P_NULL)
        {
            return m_header;
        }
        Node *ret = m_root;
        while (ret->m_left != P_NULL)
        {
            ret = ret->m_left;
        }
        return ret;
    }
    P_INLINE Node *getMax() const
    {
        if (m_root == P_NULL)
        {
            return m_header;
        }
        Node *ret = m_root;
        while (ret->m_right != P_NULL)
        {
            ret = ret->m_right;
        }
        return ret;
    }
    
private:
    Node* insert(Node *parent, const T &t);
    void rebalanceAfterInsert(Node *node);
    void rebalanceAfterErase(Node *node);
    void leftRotate(Node *node);
    void rightRotate(Node *node);
    Node *newNode(const T& key);
    void deleteTree(Node *node);

private:
    puint32         m_count;
    Node            *m_header; // farther of root and its farther is root.
    Node            *m_root;  
    Op              m_less;
};


template<typename T, typename Op>
PRbTree<T, Op>::PRbTree()
{
    m_count = 0;

    m_header = reinterpret_cast<Node*>(PNEWARRAY(puint8, sizeof(Node)));

    m_header->m_left = m_header;
    m_header->m_right = m_header;
    m_header->m_parent = m_root;

    m_root = P_NULL;
}

template<typename T, typename Op>
PRbTree<T, Op>::~PRbTree()
{
    puint8 *headerData = reinterpret_cast<puint8*>(m_header);
    PDELETEARRAY(headerData);
    deleteTree(m_root);
}

template<typename T, typename Op>
PRbTree<T, Op>::PRbTree(const PRbTree<T, Op> &o)
{
    Node* t = o.getMin();
    while (t != P_NULL)
    {
        insert(t->data());
        t = successor(t);
    }
}

template<typename T, typename Op>
typename PRbTree<T, Op>::Node * PRbTree<T, Op>::successor(Node *node)
{
    Node *ret = Node::findSuccessor(node);
    if (ret == P_NULL)
    {
        ret = m_header;
    }
    
    return ret;
}

template<typename T, typename Op>
typename PRbTree<T, Op>::Node * PRbTree<T, Op>::successor(Node *node) const
{
    Node *ret = Node::findSuccessor(node);
    if (ret == P_NULL)
    {
        ret = m_header;
    }
    
    return ret;
}

template<typename T, typename Op>
const PRbTree<T, Op> &PRbTree<T, Op>::operator=(const PRbTree<T, Op> &o)
{
    if (this != &o)
    {
        clear();
        
        Node* t = o.getMin();
        while (t != P_NULL)
        {
            insert(t->data());
            t = successor(t);
        }
    }

    return *this;
}


template<typename T, typename Op>
typename PRbTree<T, Op>::Node *PRbTree<T, Op>::insert(const T &t)
{
    Node *parent = m_header;
    Node *insertPos = m_root;

    while (insertPos != P_NULL)
    {
        parent = insertPos;
        if (m_less(t, insertPos->m_data))
        {
            insertPos = insertPos->m_left;
        }
        else
        {
            insertPos = insertPos->m_right;
        }
    }

    return insert(parent, t);
}

template<typename T, typename Op>
typename PRbTree<T, Op>::Node *PRbTree<T, Op>::insertUnique(const T &t)
{
    Node *parent = m_header;
    Node *insertPos = m_root;
    
    while (insertPos != P_NULL)
    {
        parent = insertPos;
        if (m_less(insertPos->m_data, t))
        {
            insertPos = insertPos->m_right;
        }
        else if (m_less(t, insertPos->m_data))
        {
            insertPos = insertPos->m_left;
        }
        else
        {
            return P_NULL;
        }
    }

    return insert(parent, t);
}

template<typename T, typename Op>
typename PRbTree<T, Op>::Node *PRbTree<T, Op>::insert(Node *parent, const T &t)
{
    typename PRbTree<T, Op>::Node *node = newNode(t);
    node->m_parent = parent;
    if (parent == m_header)
    {
        node->m_color = P_COLOR_BLACK;

        m_root = node;
        m_header->m_parent = m_root;
    }
    else
    {
        if (m_less(t, parent->m_data))
        {
            Node *oldLeft = parent->m_left;
            parent->m_left = node;
            node->m_left = oldLeft;
        }
        else
        {
            Node *oldRight = parent->m_right;
            parent->m_right = node;
            node->m_right = oldRight;			
        }
    }

    ++m_count;

    rebalanceAfterInsert(node);

    return node;
}

// The algoirthm is revised according to 
// http://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
template<typename T, typename Op>
void PRbTree<T, Op>::rebalanceAfterInsert(Node *node)
{
#define PARENT(node) (node->m_parent)
#define GRANDPARENT(node) (node->m_parent->m_parent)

    while (node != m_root && PARENT(node)->m_color == P_COLOR_RED)
    {  
        if (PARENT(node) == GRANDPARENT(node)->m_left)
        {  
            Node *uncle = GRANDPARENT(node)->m_right;
            if (uncle != P_NULL && uncle->m_color == P_COLOR_RED)
            {  
                // Flip the colors of two children of grandparent.
                // We does not change the number of black nodes after flipping
                // and thus ensure the crteria 4 of red-black tree. We can assure
                // uncle's children must be not be black otherwise it is break
                // criteria 3 before flipping.
                PARENT(node)->m_color = P_COLOR_BLACK;
                uncle->m_color = P_COLOR_BLACK;          
                GRANDPARENT(node)->m_color = P_COLOR_RED; 
                node = GRANDPARENT(node);
            }  
            else // No uncle or uncle is black
            {  
                if (node == PARENT(node)->m_right)
                {  
                    leftRotate(node->m_parent);
                    node = node->m_left;
                }  
                PARENT(node)->m_color = P_COLOR_BLACK;
                GRANDPARENT(node)->m_color = P_COLOR_RED;  
                rightRotate(GRANDPARENT(node));
            }  
        }  
        else // Mirror reflection.
        {  
            Node *uncle = GRANDPARENT(node)->m_left;
            if (uncle != P_NULL && uncle->m_color == P_COLOR_RED)
            {  
                PARENT(node)->m_color = P_COLOR_BLACK;
                uncle->m_color = P_COLOR_BLACK;          
                GRANDPARENT(node)->m_color = P_COLOR_RED;
                node = GRANDPARENT(node);
            }  
            else
            {  
                if (node == PARENT(node)->m_left)
                {  
                    rightRotate(PARENT(node));
                    node = node->m_right;  
                }  
                PARENT(node)->m_color = P_COLOR_BLACK;
                GRANDPARENT(node)->m_color = P_COLOR_RED;  
                leftRotate(GRANDPARENT(node));
            }  
        }  
    }

    m_root->m_color = P_COLOR_BLACK;

#undef PARENT
#undef GRANDPARENT
}


// The illustration of the left rotate
//         +---+                          +---+
//         | Q |                          | P |
//         +---+                          +---+
//        /     \     right rotation     /     \
//     +---+   +---+  ------------->  +---+   +---+
//     | P |   | Z |                  | X |   | Q |
//     +---+   +---+  <-------------  +---+   +---+
//    /     \          left rotation         /     \
// +---+   +---+                          +---+   +---+
// | X |   | Y |                          | Y |   | Z |
// +---+   +---+                          +---+   +---+

template<typename T, typename Op>
void PRbTree<T, Op>::leftRotate(Node *p)
{  
    // Swap the position p and q.
    Node *q = p->m_right;
    if (q == P_NULL)
    {
        return ;
    }

    if (p == m_root)
    {
        m_root = q;  
    }
    else if (p == p->m_parent->m_left)
    {
        p->m_parent->m_left = q;  
    }
    else
    {
        p->m_parent->m_right = q;  
    }

    q->m_parent = p->m_parent;
    p->m_parent = q;
    
    // Move y from q to p.
    Node *y = q->m_left;
    q->m_left = p;
    
    p->m_right = y;
    if (y != P_NULL)
    {
        y->m_parent = p;
    }
}  


template<typename T, typename Op>
void PRbTree<T, Op>::rightRotate(Node *q)  
{  
    Node *p = q->m_left;
    if (p == P_NULL)
    {
        return ;
    }
    
    // Swap the q and p
    if (q == m_root)
    {
        m_root = p;  
    }
    else if (q== q->m_parent->m_right)
    {
        q->m_parent->m_right = p;
    }
    else
    {
        q->m_parent->m_left = p;  
    }
    p->m_parent = q->m_parent;
    q->m_parent = p;

    // Move x from p to q.
    Node *y = p->m_right;
    p->m_right = q;

    q->m_left = y;
    if (y != P_NULL)
    {
        y->m_parent = q;
    }
}  

template<typename T, typename Op>
typename PRbTree<T, Op>::Node *PRbTree<T, Op>::find(const T &t) const
{
    Node *result = m_root;

    while (result != P_NULL)
    {
        if (m_less(result->m_data, t))
        {
            result = result->m_right;
        }
        else if (m_less(t, result->m_data))
        {
            result = result->m_left;
        }
        else
        {
            return result;
        }
    }
    
    return m_header;
}

template<typename T, typename Op>
void PRbTree<T, Op>::erase(Node *node)
{
    if (node == m_header)
    {
        return ; 
    }

    PASSERT(node != P_NULL);
    
    // When this node has two children, replace this node with the smallest
    // node in its right subtree.
    if (node->m_left != P_NULL && node->m_right != P_NULL)
    {
        Node *succesor = successor(node);
        node->m_data = succesor->m_data;

        node = succesor;
    }
    
    Node *balanceNode = P_NULL;
    if (node->m_right != P_NULL)
    {
        if (node == m_root)
        {
            m_root = node->m_right;
            m_root->m_parent = m_header;
            m_header->m_parent = m_root;
            balanceNode = m_root;
        }
        else
        {
            if (node->m_parent->m_left == node)
            {
                node->m_parent->m_left = node->m_right;
            }
            else
            {
                node->m_parent->m_right = node->m_right;
            }
            node->m_right->m_parent = node->m_parent;
            balanceNode = node->m_right;
        }

    }    
    // Mirror reflection
    else if (node->m_left != P_NULL)
    {
        if (node == m_root)
        {
            m_root = node->m_left;
            m_root->m_parent = m_header;
            m_header->m_parent = m_root;
            balanceNode = m_root;
        }
        else
        {
            if (node->m_parent->m_left == node)
            {
                node->m_parent->m_left = node->m_left;
            }
            else
            {
                node->m_parent->m_right = node->m_left;
            }
            node->m_left->m_parent = node->m_parent;
            balanceNode = node->m_left;
        }
    }
    else
    {
        if (node == m_root)
        {
            m_header->m_parent = P_NULL;
        }
        else
        {
            if (node->m_parent->m_left == node)
            {
                node->m_parent->m_left = P_NULL;
            }
            else
            {
                node->m_parent->m_right = P_NULL;
            }
        }
        balanceNode = node;
    }
    
    if (node->m_color == P_COLOR_BLACK)
    {
        rebalanceAfterErase(balanceNode);
    }

    if (node == m_root)
    {
        m_root = P_NULL;
    }

    PDELETEARRAY(node);

    --m_count;
}

template<typename T, typename Op>
void PRbTree<T, Op>::rebalanceAfterErase(Node *node)
{
    while (node != m_root && node->m_color == P_COLOR_BLACK)    
    {    
        if (node == node->m_parent->m_left || node->m_parent->m_left == P_NULL)    
        {    
            Node *brother = node->m_parent->m_right;    
            if (brother->m_color == P_COLOR_RED)
            {    
                leftRotate(node->m_parent);    
                brother->m_color = P_COLOR_BLACK;    
                node->m_parent->m_color = P_COLOR_RED;    
            }    
            else
            {    
                if ((brother->m_left == P_NULL || brother->m_left->m_color == P_COLOR_BLACK) && 
                    (brother->m_right == P_NULL || brother->m_right->m_color == P_COLOR_BLACK))
                {    
                    brother->m_color = P_COLOR_RED;    
                    node = node->m_parent;    
                }    
                else  
                {  
                    if ((brother->m_right == P_NULL || brother->m_right->m_color == P_COLOR_BLACK))
                    {  
                        brother->m_color = P_COLOR_RED;  
                        if (brother->m_left != P_NULL)
                        {
                            brother->m_left->m_color = P_COLOR_BLACK;  
                        }
                        rightRotate(brother);  
                        brother = node->m_parent->m_right;
                    }  
                  
                    brother->m_color = node->m_parent->m_color;    
                    node->m_parent->m_color = P_COLOR_BLACK;    
                    if (brother->m_right != P_NULL)
                    {
                        brother->m_right->m_color = P_COLOR_BLACK;
                    }
                    leftRotate(node->m_parent);    
                    node = m_root;  
                } 
            }
        }    
        else
        {    
            Node *brother = node->m_parent->m_left;    
            if (brother->m_color == P_COLOR_RED)    
            {    
                brother->m_color = P_COLOR_BLACK;    
                node->m_parent->m_color = P_COLOR_RED;    
                rightRotate(node->m_parent);    
            }    
            else    
            {    
                if ((brother->m_left == P_NULL || brother->m_left->m_color == P_COLOR_BLACK) && 
                    (brother->m_right == P_NULL || brother->m_right->m_color == P_COLOR_BLACK))
                {    
                    brother->m_color = P_COLOR_RED;    
                    node = node->m_parent;    
                }    
                else  
                {  
                    if (brother->m_left == P_NULL || brother->m_left->m_color == P_COLOR_BLACK)    
                    {    
                        brother->m_color = P_COLOR_RED;    
                        if (brother->m_right != P_NULL)
                        {
                            brother->m_right->m_color = P_COLOR_BLACK;
                        }
                        leftRotate(brother);  
                        brother = node->m_parent->m_left;
                    }  
                    brother->m_color = node->m_parent->m_color;    
                    node->m_parent->m_color = P_COLOR_BLACK;    
                    if (brother->m_left != P_NULL)
                    {
                        brother->m_left->m_color = P_COLOR_BLACK;
                    }
                    rightRotate(node->m_parent);    
                    node = m_root;    
                }    
            }    
        }    
    }
    node->m_color = P_COLOR_BLACK;
}

template<typename T, typename Op>
void PRbTree<T, Op>::clear()
{
    if (m_root != P_NULL)
    {
        deleteTree(m_root);
    }
    
    m_count = 0;
}

template<typename T, typename Op>
void PRbTree<T, Op>::deleteTree(Node *node)
{
    if (node == P_NULL)
    {
        return ;
    }

    if (node->m_left != P_NULL)
    {
        deleteTree(node->m_left);
    }

    if (node->m_right != P_NULL)
    {
        deleteTree(node->m_right);
    }
    
    if (m_root == node)
    {
        m_root = P_NULL;
    }

    if (PTypeInfo<T>::isComplex)
    {
        node->m_data.~T();
    }
    puint8 *data = reinterpret_cast<puint8*>(node);
    PDELETEARRAY(data);
}
    
template<typename T, typename Op>
typename PRbTree<T, Op>::Node *PRbTree<T, Op>::newNode(const T &t)
{
    Node* n = reinterpret_cast<Node*>(PNEW(puint8 [sizeof(Node)]));
    if (PTypeInfo<T>::isComplex)
    {
        new (&(n->m_data)) T(t);
    }
    else
    {
        n->m_data = t;
    }

    n->m_left = P_NULL;
    n->m_right = P_NULL;
    n->m_color = P_COLOR_RED;
    n->m_parent = P_NULL;

    return n;
}

template<typename T, typename Op>
void PRbTree<T, Op>::swap(PRbTree &other)
{
    pSwap(m_root, other.m_root);
    pSwap(m_header, other.m_header);
    pSwap(m_count, other.m_count);
}

#endif // !PRBTREE_H
