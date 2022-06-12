#include "set.h"

Node::Node(int n, Node *xt, bool m)
          :value{n}, next{xt}, marked{m} {}

Set::Set()
{
    _init();
}

Set::~Set()
{
    _clear();
}

bool Set::add(const int &v)
{
    while (true) {
        Node *prev = _head;
        Node *next = _head->next;

        while (next->value < v)
        {
            prev = next; next = next->next;
        }

        std::unique_lock<std::mutex> prev_lock(prev->lock);
        std::unique_lock<std::mutex> curr_lock(next->lock);

        if(_check(prev, next))
        {
            if (next->value == v) 
            {
                return false;
            } 
            else 
            {
                Node *newn = new Node(v, next);
                prev->next = newn;
                return true;
            }
        }
    }
}

bool Set::remove(const int &v)
{
    while (true) {
        Node *prev = _head;
        Node *tdel = _head->next;

        while (tdel->value < v)
        {
            prev = tdel; tdel = tdel->next;
        }

        std::unique_lock<std::mutex> prev_lock(prev->lock);
        std::unique_lock<std::mutex> curr_lock(tdel->lock);

        if(_check(prev, tdel))
        {
            if (tdel->value != v) 
            {
                return false;
            } 
            else 
            {
                tdel->marked = true;
                prev->next = tdel->next;
                //delete tdel;
                return true;
            }
        }
    }
}

bool Set::contains(const int &v)
{
    Node *node = _head;
    while(node->value < v && node != _tail)
    {
        node = node->next;
    }
    return node->value == v and !node->marked;
}

void Set::_init()
{
    _head = new Node(INT_MIN);
    _tail = new Node(INT_MAX);
    _head->next = _tail;
}

bool Set::_check(const Node *a, const Node *b)
{
    return !a->marked && !b->marked && a->next == b;
}

void Set::_clear()
{
    Node *p = _head->next;
    while(p != _tail)
    {
        Node *next = p->next;
        remove(p->value);
        p = next;
    }
}