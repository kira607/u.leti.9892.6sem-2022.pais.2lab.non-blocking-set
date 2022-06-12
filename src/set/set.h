#ifndef INC_SET_H
#define INC_SET_H


#include <mutex>
#include <climits>

#include "../log.h"


struct Node
{
    Node(int n = 0, Node *xt = nullptr, bool m = false);

    int value;
    Node *next;
    bool marked;
    std::mutex lock;
};

class Set
{
public:
    Set();
    ~Set();

    bool add(const int &v);
    bool remove(const int &v);
    bool contains(const int &v);

private:
    Node *_head, *_tail;

    void _init();
    bool _check(const Node *a, const Node *b);
    void _clear();
};


#endif //INC_SET_H