#pragma once

#include <stack>
#include <vector>
#include <unordered_set>
#include "IteratorBase.h"

template<typename NodeT>
class Iterator: public IteratorBase<NodeT> {

public:
    using ValueT = typename NodeT::ValueT;

    Iterator(NodeT* parent): IteratorBase<NodeT>(parent) { pushLeft(parent); }
    Iterator(const Iterator& other): IteratorBase<NodeT>(other) {};
    
    Iterator& operator++(int) { 
        this->increment();
        Iterator<NodeT> temp = Iterator(*this);
        return temp; 
    }
    Iterator& operator++() {
        this->increment();
        return *this;
    }
    /// Return a reference to the item to which this iterator is pointing.
    NodeT* operator*() override { return this->getCurrent(); }
   
protected:
    void increment() override;

private:
    NodeT* getCurrent() override { return this->_stack.top(); }
    void pushLeft(NodeT* node);
    NodeT* getRight(NodeT* node);
};

template<typename NodeT>
void Iterator<NodeT>::pushLeft(NodeT* node) {
    while (node != nullptr) {
        this->_stack.push(node);
        node = node->getChild();
    }
}

template<typename NodeT>
NodeT* Iterator<NodeT>::getRight(NodeT* node) {      
    if (node->getChild()) node = node->getChild();
    while (node->getSibling()) node = node->getSibling();

    return node;
}

template<typename NodeT>
void Iterator<NodeT>::increment() {
    NodeT* node = this->_stack.top();
    this->_stack.pop();

    if (node->getSibling() != nullptr) {
        pushLeft(node->getSibling());
    }

    if (this->_stack.size() == 0) {
        this->_parentNode = nullptr;
        return;
    }
}