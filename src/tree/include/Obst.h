#pragma once

#include <algorithm>
#include "TreeBase.h"

template<typename NodeT>
class Obst: public TreeBase<NodeT> {
public:
    using ValueT = typename NodeT::ValueT;

	Obst(): TreeBase<NodeT>() {}
	Obst(const std::vector<ValueT>& keys,
		const std::vector<int>& frequencies):
        TreeBase<NodeT>(),
        _keys(keys),
        _frequencies(frequencies) {
        build();
	}
    Obst(const Obst<NodeT>& other): 
        TreeBase<NodeT>(other),
        _keys(other._keys),
        _frequencies(other._frequencies) {
        build();
    }

    void printCost() {
        for (int i = 0; i < _keys.size(); i++) {
            for (int j = 0; j < _keys.size(); j++) {
                std::cout << _cost[i][j] << " \t";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        for (int i = 0; i < _keys.size(); i++) {
            for (int j = 0; j < _keys.size(); j++) {
                std::cout << _rootIndex[i][j] << " \t";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void addNodeStatic(NodeT* node);
    void removeNodeStatic(NodeT* node);
    int getTotalCost() { return _cost[0][_cost.size() - 1]; }
    Obst<NodeT> difference(Obst<NodeT>& other);

private:
	// A utility function to get sum of array elements 
	// freq[i] to freq[j] 
	int freqSumRange(int i, int j);
	/* A Dynamic Programming based function that calculates
	minimum cost of a Binary Search Tree. */
	int init(int i, int j);
    void build();
    NodeT* construct(int i, int j);
    NodeT* findNode(NodeT* find, NodeT* node);
    void differenceRec(NodeT* node, Obst<NodeT>& other, Obst<NodeT>& result);

private:
    std::vector<std::vector<int>> _rootIndex;
    std::vector<std::vector<int>> _cost;
	std::vector<ValueT> _keys;
	std::vector<int> _frequencies;
};

template<typename NodeT>
inline void Obst<NodeT>::build() {
    int size = _keys.size();
    _cost.assign(size, std::vector(size, 0));
    _rootIndex.assign(size, std::vector(size, -1));

    for (int i = 0; i < size; i++) {
        _rootIndex[i][i] = i;
    }

    init(0, size - 1);

    this->_root = construct(0, size - 1);
}

template<typename NodeT>
int Obst<NodeT>::freqSumRange(int i, int j) {
	int s = 0;
	for (int k = i; k <= j; k++)
		s += _frequencies[k];
	return s;
}

template<typename NodeT>
int Obst<NodeT>::init(int i, int j) {
    int size = _keys.size();
    // Reuse cost already calculated for the subproblems.
    // Since we initialize cost matrix with 0 and frequency for a tree of one node,
    // it can be used as a stop condition
    if (!(i > size - 1 || j < 0) && _cost[i][j])
        return _cost[i][j];

    // Get sum of freq[i], freq[i+1], ... freq[j]
    int fsum = freqSumRange(i, j);

    // Initialize minimum value
    int Min = INT_MAX;

    // One by one consider all elements as
    // root and recursively find cost of
    // the BST, compare the cost with min
    // and update min if needed
    for (int r = i; r <= j; r++) {
        int c = init(i, r - 1) + init(r + 1, j) + fsum;
        if (c < Min) {
            Min = c;
            // replace cost with new optimal calc
            _cost[i][j] = c;
            _rootIndex[i][j] = r;
        }
    }

    // Return minimum value
    if (i > size - 1 || j < 0) return 0;
    return _cost[i][j];
}

template<typename NodeT>
inline NodeT* Obst<NodeT>::construct(int i, int j) {
    if (i > j) return nullptr;

    int r = _rootIndex[i][j];
    NodeT* node = new NodeT(_keys[r]);
    node->setWeight(_frequencies[r]);

    NodeT* child = construct(i, r - 1);

    if (child) {
        node->setChild(child);
        child->setSibling(construct(r + 1, j));
    } else {
        node->setChild(construct(r + 1, j));
    }
    
    return node;
}

template<typename NodeT>
inline NodeT* Obst<NodeT>::findNode(NodeT* target, NodeT* node) {
    if (*node == *target) return node;
       
    if (node->getChild()) { 
        NodeT* child = findNode(target, node->getChild());
        if (child) return child;
    }
    if (node->getSibling()) { 
        NodeT* sibling = findNode(target, node->getSibling());
        if (sibling) return sibling;
    }

    return nullptr;
}

template<typename NodeT>
inline void Obst<NodeT>::addNodeStatic(NodeT* node) {
    for (int i = 0; i < _keys.size(); i++) {
        if (_keys[i] == node->getValue()) {
            _frequencies[i]++;
            return;
        }
    }
  
    _keys.push_back(node->getValue());
    _frequencies.push_back(node->getWeight());

    build();
}

template<typename NodeT>
inline void Obst<NodeT>::removeNodeStatic(NodeT* node) {
    NodeT* toRemove = findNode(node, this->_root);

    if (toRemove) {
        for (int i = 0; i < _keys.size(); i++) {
            if (_keys[i] == node->getValue()) {
                _frequencies[i]--;
                if (_frequencies[i] == 0) {
                    _keys.erase(_keys.begin() + i);
                    _frequencies.erase(_frequencies.begin() + i);
                    delete toRemove;
                }
            }
        }        
    }

    build();
}

template<typename NodeT>
inline Obst<NodeT> Obst<NodeT>::difference(Obst<NodeT>& other) {
    Obst<NodeT> newTree = Obst<NodeT>();

    differenceRec(this->_root, other, newTree);
    return newTree;
}

template<typename NodeT>
inline void Obst<NodeT>::differenceRec(NodeT* node, Obst<NodeT>& other, Obst<NodeT>& result) {
    if (!node) return;
   
    // If node is not in "other", add it to "result"
    NodeT* find = other.findNode(node, other._root);
    if (!find || find->getWeight() != node->getWeight()) {
        result.addNodeStatic(node);
    }

    // Recurse for left and right subtrees
    differenceRec(node->getChild(), other, result);
    differenceRec(node->getSibling(), other, result);
}
