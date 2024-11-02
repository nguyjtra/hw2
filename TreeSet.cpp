#ifndef TREE_SET_CPP
#define TREE_SET_CPP
#include <stdlib.h>
#include "TreeSet.hpp"
using namespace std;
template <typename T>
TreeSet<T>::TreeSet() : _root(NULL), _size(0)
{
    _comparator = [](T left, T right)
    {
        if (left < right)
        {
            return -1;
        }
        else if (left > right)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    };
}
template <typename T>
TreeSet<T>::TreeSet(std::function<int(T, T)> comparator) : _root(NULL), _size(0), _comparator(comparator){};

template <typename T>
size_t TreeSet<T>::size() const
{
    return _size;
}

template <typename T>
TreeSet<T>::TreeSet(const std::vector<T> &items)
{
    for (auto a : items)
    {
        add(a);
    };
};

template <typename T>
TreeSet<T>::TreeSet(const std::vector<T> &items, std::function<int(T, T)> comparator) : _root(NULL), _size(0), _comparator(comparator)
{
    for (auto a : items)
    {
        add(a);
    };
};
template <typename T>
bool TreeSet<T>::contains(T value) const
{
    BinaryTreeNode<T> *x = _root;
    while (x != NULL)
    {
        int check = _comparator(value, x->value);
        if (check == 0)
        {
            return true;
        }
        else if (check == 1)
        {
            x = x->_right;
        }
        else
        {
            x = x->_left;
        }
    }
    return false;
};

template <typename T>
void TreeSet<T>::add(T value)
{
    BinaryTreeNode<T> *z = new BinaryTreeNode(value);
    BinaryTreeNode<T> *y = NULL;
    BinaryTreeNode<T> *x = _root;
    while (x != NULL)
    {
        y = x;
        int check = _comparator(z->value, x->value);
        if (check == 0)
        {
            x->value = z->value;
            delete z;
            return;
        }
        else if (check == 1)
        {
            x = x->_right;
        }
        else
        {
            x = x->_left;
        }
    }
    z->_parent = y;
    int check = _comparator(z->value, y->value);
    if (y == NULL)
    {
        _root = z;
    }
    else if (check == 1)
    {
        y->_right = z;
    }
    else if (check == -1)
    {
        y->_left = z;
    }

    z->_left = NULL;
    z->_right = NULL;
    z->_color = Red;

    _size++;
    fix_violation(z);
};

template <typename T>
bool TreeSet<T>::is_empty() const
{
    if (_size == 0)
        return true;
    else
        return false;
};

template <typename T>
std::optional<T> TreeSet<T>::min() const
{
    BinaryTreeNode<T> *x = _root;
    if (!x || x == NULL)
    {
        return std::nullopt;
    }
    while (x->_left != NULL)
    {
        x = x->_left;
    }
    return x->value;
};

template <typename T>
std::optional<T> TreeSet<T>::max() const
{
    BinaryTreeNode<T> *x = _root;
    if (!x || x == NULL)
    {
        return std::nullopt;
    }
    while (x->_right != NULL)
    {
        x = x->_right;
    }
    return x->value;
};

template <typename T>
std::vector<T> TreeSet<T>::to_vector() const
{
    vector<T> a;
    BinaryTreeNode<T> *x = _root;
    function<void(BinaryTreeNode<T> *)> inOrder = [&](BinaryTreeNode<T> *b)
    {
        if (b == NULL)
            return;
        inOrder(b->_left);
        a.push_back(b->value);
        inOrder(b->_right);
    };
    inOrder(x);
    return a;
};

template <typename T>
std::optional<T> TreeSet<T>::get(T value) const
{
    BinaryTreeNode<T> *x = _root;
    while (x != NULL)
    {
        int check = _comparator(value, x->value);
        if (check == 0)
        {
            return value;
        }
        else if (check == 1)
        {
            x = x->_right;
        }
        else
        {
            x = x->_left;
        }
    }
    return std::nullopt;
};

template <typename T>
TreeSet<T> TreeSet<T>::operator+(const TreeSet &other)
{
    TreeSet<T> ans(*this);
    vector<T> tmp = other.to_vector();
    for (T a : tmp)
    {
        add(a);
    }
    return ans;
};

template <typename T>
TreeSet<T> &TreeSet<T>::operator+=(const TreeSet &other)
{
    vector<T> tmp = other.to_vector();
    for (T a : tmp)
    {
        add(a);
    }
    return *this;
};

template <typename T>
TreeSet<T> TreeSet<T>::operator&(const TreeSet &other)
{
    TreeSet<T> ans;
    for (T a : this->to_vector())
    {
        if (other.contains(a))
        {
            ans.add(a);
        }
    }
    return ans;
};

template <typename T>
bool TreeSet<T>::operator==(const TreeSet &other) const
{
    return this->to_vector() == other.to_vector();
};

template <typename T>
bool TreeSet<T>::operator!=(const TreeSet &other) const
{
    if (this->to_vector() == other.to_vector())
    {
        return false;
    }
    else
        return true;
};

template <typename T>
void TreeSet<T>::clear()
{
    BinaryTreeNode<T> *x = _root;
    function<void(BinaryTreeNode<T> *)> inOrder = [&](BinaryTreeNode<T> *b)
    {
        if (b == NULL)
            return;
        inOrder(b->_left);
        inOrder(b->_right);
        delete b;
    };
    inOrder(x);
    _root = NULL;
    _size = 0;
};

template <typename T>
TreeSet<T>::~TreeSet()
{
    clear();
}

template <typename T>
void TreeSet<T>::rotate_left(BinaryTreeNode<T> *x)
{
    BinaryTreeNode<T> *y = x->_right;
    x->_right = y->_left;
    if (y->_left != NULL)
    {
        y->_left->_parent = x;
    }
    y->_parent = x->_parent;
    if (x->_parent == nullptr)
    {
        _root = y;
    }
    else if (x == x->_parent->_left)
    {
        x->_parent->_left = y;
    }
    else
    {
        x->_parent->_right = y;
    }
    y->_left = x;
    x->_parent = y;
};

template <typename T>
void TreeSet<T>::rotate_right(BinaryTreeNode<T> *x)
{
    BinaryTreeNode<T> *y = x->_left;
    x->_left = y->_right;
    if (y->_right != NULL)
    {
        y->_right->_parent = x;
    }
    y->_parent = x->_parent;
    if (x->_parent == nullptr)
    {
        _root = y;
    }
    else if (x == x->_parent->_right)
    {
        x->_parent->_right = y;
    }
    else
    {
        x->_parent->_left = y;
    }
    y->_right = x;
    x->_parent = y;
}

template <typename T>
void TreeSet<T>::fix_violation(BinaryTreeNode<T> *k)
{
    while (k != _root && k->_parent->_color == Red)
    {
        if (k->_parent == k->_parent->_parent->_left)
        {
            BinaryTreeNode<T> *u = k->_parent->_parent->_right;
            if (u->_color == Red)
            {
                k->_parent->_color = Black;
                u->_color = Black;
                k->_parent->_parent->_color = Red;
                k = k->_parent->_parent;
            }
            else
            {
                if (k == k->_parent->_right)
                {
                    k = k->_parent;
                    rotate_left(k);
                }
                k->_parent->_color = Black;
                k->_parent->_parent->_color = Red;
                rotate_right(k->_parent->_parent);
            }
        }
        else
        {
            BinaryTreeNode<T> *u = k->_parent->_parent->_left;
            if (u->_color == Red)
            {
                k->_parent->_color = Black;
                u->_color = Black;
                k->_parent->_parent->_color = Red;
                k = k->_parent->_parent;
            }
            else
            {
                if (k == k->_parent->_left)
                {
                    k = k->_parent;
                    rotate_right(k);
                }
                k->_parent->_color = Black;
                k->_parent->_parent->_color = Red;
                rotate_left(k->_parent->_parent);
            }
        }
    }
    _root->_color = Black;
}

#endif
