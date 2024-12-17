#pragma once

#include <stack>
#include <vector>
#include <unordered_set>

template<typename NodeT>
class IteratorBase {

public:
    using ValueT = typename NodeT::ValueT;

    IteratorBase() : _parentNode(nullptr) {};
    IteratorBase(NodeT* parent) : _parentNode(parent) {};
    IteratorBase(const IteratorBase& other) {
        _parentNode = other._parentNode;
        _stack = other._stack;
    };
    IteratorBase& operator=(const IteratorBase&) = default;
    IteratorBase& operator++() { this->increment(); return *this; }
    virtual IteratorBase& operator++(int) = 0;
    virtual NodeT* operator*() = 0;
    bool operator==(const IteratorBase& other) const { return _parentNode == other._parentNode; }
    bool operator!=(const IteratorBase& other) const { return !(*this == other); }

protected:
    NodeT* _parentNode;
    std::stack<NodeT*> _stack;

    virtual void increment() = 0;
    virtual NodeT* getCurrent() = 0;
};