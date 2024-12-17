#pragma once

#include <stack>
#include <vector>
#include <unordered_set>
#include "IteratorBase.h"

template<typename NodeT>
class IteratorReversed: public IteratorBase<NodeT> {

public:
    using ValueT = typename NodeT::ValueT;

    IteratorReversed(NodeT* parent): IteratorBase<NodeT>(parent) { this->_stack.push(parent); }
    IteratorReversed(const IteratorReversed& other): IteratorBase<NodeT>(other) { this->_visited = other._visited; };
    IteratorReversed& operator++(int) {
        this->increment();
        IteratorReversed<NodeT> temp = IteratorReversed(*this);
        return temp;
    }
    /// Return a reference to the item to which this iterator is pointing.
    NodeT* operator*() override { 
        if (getCurrent()) return this->getCurrent();
        return 0;
    }

protected:
    NodeT* getCurrent() override {
        NodeT* current = this->_stack.top();

        if (_visited.count(current) == 0) return current;
        return nullptr;
    }
    void increment() override;

private:
    std::unordered_set<NodeT*> _visited;

    NodeT* getRight(NodeT* node);
};

template<typename NodeT>
NodeT* IteratorReversed<NodeT>::getRight(NodeT* node) {
    if (node->getChild()) node = node->getChild();
    while (node->getSibling()) node = node->getSibling();

    return node;
}

template<typename NodeT>
void IteratorReversed<NodeT>::increment() {
    NodeT* current = this->_stack.top();

    _visited.insert(current); // Mark as visited

    if (getRight(current) && _visited.count(getRight(current)) == 0) {
        this->_stack.push(getRight(current));
    }
    else if (current->getChild() && _visited.count(current->getChild()) == 0) {
        this->_stack.push(current->getChild());
    }
    else {
        this->_stack.pop();
    }

    if (this->_stack.size() == 0) {
        this->_parentNode = nullptr;
        return;
    }
}