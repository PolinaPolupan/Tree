// TreeBase.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "TreeNode.h"
#include "Iterator.h"
#include "IteratorReversed.h"
#include <iostream>
#include <unordered_set>

template<typename NodeT>
class TreeBase {

public:
	using ValueT = typename NodeT::ValueT;

	TreeBase(): _root(nullptr) {};
	TreeBase(NodeT*& root) : _root(root) {};
	TreeBase(const TreeBase& other): TreeBase<NodeT>() {
		clear(_root);
		_root = deepCopy(other._root);
	}
	~TreeBase() { clear(_root); _root = nullptr; }
	TreeBase& operator=(const TreeBase& other) {
		if (this == &other) return *this;

		// Clean up existing tree
		clear(_root);
		_root = deepCopy(other._root);
		return *this;
	}

	void traverse();
	void traverseR();
	void printTree(bool weight = false);

	Iterator<NodeT> begin() { return Iterator<NodeT>(_root); }
	IteratorReversed<NodeT> rbegin() { return IteratorReversed<NodeT>(_root); }

	Iterator<NodeT> end() { return Iterator<NodeT>(nullptr); }
	IteratorReversed<NodeT> rend() { return IteratorReversed<NodeT>(nullptr); }
	NodeT* rotateLeft(NodeT* node);
	NodeT* rotateRight(NodeT* node);

	NodeT* getRoot() { return _root; }

protected:
	NodeT* _root;
	std::unordered_set<NodeT*> _visited;

	NodeT* getRight(NodeT* root);
	void clear(NodeT* root);
	NodeT* deepCopy(NodeT* node);

	void traverseRec(NodeT* root);
	void traverseRRec(NodeT* root);
	void printTreeRec(NodeT* root, int level = 0, bool weight = false);
};

template<typename NodeT>
void TreeBase<NodeT>::clear(NodeT* root) {
	if (root == nullptr)
		return;

	clear(root->getChild());
	clear(root->getSibling());
//	std::cout << "Remove: " << root->getValue() << "\n";
	delete root;
}

template<typename NodeT>
void TreeBase<NodeT>::traverse() {
	traverseRec(_root);
	std::cout << "\n";
}

template<typename NodeT>
void TreeBase<NodeT>::traverseR() {
	traverseRRec(_root);
	std::cout << "\n";
}

template<typename NodeT>
void TreeBase<NodeT>::printTree(bool weight) {
	printTreeRec(_root, 0, weight);
	std::cout << "\n";
}

template<typename NodeT>
void TreeBase<NodeT>::traverseRec(NodeT* root) {
	if (root == NULL)
		return;

	traverseRec(root->getChild()); // First, if child exists, traverse child. No return statement following here.
	std::cout << root->getValue() << " ";
	traverseRec(root->getSibling());
}

template<typename NodeT>
void TreeBase<NodeT>::traverseRRec(NodeT* root) {
	if (root == NULL || _visited.count(root))
		return;

	_visited.insert(root);

	std::cout << " " << root->getValue();

	traverseRRec(getRight(root));
	traverseRRec(root->getChild());
}

template<typename NodeT>
void TreeBase<NodeT>::printTreeRec(NodeT* root, int level, bool weight) {
	if (!root) return;

	// Print the current node with its level for clarity
	std::cout << std::string(level * 2, ' ') << root->getValue();
	if (weight) {
		std::cout << " (" << root->getWeight() << ") \n";
	}
	else {
		std::cout << "\n";
	}

	// Print the child subtree
	printTreeRec(root->getChild(), level + 1, weight);

	// Print the sibling subtree
	printTreeRec(root->getSibling(), level, weight);
}

template<typename NodeT>
inline NodeT* TreeBase<NodeT>::rotateRight(NodeT* node) {
	NodeT* newRoot = node->getChild();
	node->setChild(newRoot->getChild()->getSibling());
	newRoot->getChild()->setSibling(node);
	node->getChild()->setSibling(newRoot->getSibling());
	newRoot->setSibling(nullptr);

	_root = newRoot;
	return newRoot;
}

template<typename NodeT>
inline NodeT* TreeBase<NodeT>::rotateLeft(NodeT* node) {
	NodeT* newRoot = node->getChild()->getSibling();
	node->getChild()->setSibling(newRoot->getChild());
	node->setSibling(newRoot->getChild()->getSibling());
	node->getChild()->getSibling()->setSibling(nullptr);
	newRoot->setChild(node);

	_root = newRoot;
	return newRoot;
}

template<typename NodeT>
NodeT* TreeBase<NodeT>::getRight(NodeT* root) {

	if (root->getChild()) root = root->getChild();
	while (root->getSibling()) root = root->getSibling();

	return root;
}

template<typename NodeT>
NodeT* TreeBase<NodeT>::deepCopy(NodeT* node) {
	if (node == nullptr) return nullptr;
	NodeT* newNode = new NodeT(node->getValue(), node->getWeight());
	newNode->setChild(deepCopy(node->getChild()));
	newNode->setSibling(deepCopy(node->getSibling()));
	return newNode;
}