#include "TreeNode.h"
#include "TreeBase.h"
#include "Iterator.h"
#include "Obst.h"
#include "IteratorReversed.h"
#include <iostream>
#include <filesystem>
#include "tree2img.h"

using namespace std;

int main()
{
	std::filesystem::path baseDir = std::filesystem::path(PROJECT_DIR);

	TreeNode<int>* node1 = new TreeNode<int>(1);
	TreeNode<int>* node2 = new TreeNode<int>(2);
	TreeNode<int>* node3 = new TreeNode<int>(3);
	TreeNode<int>* node4 = new TreeNode<int>(4);
	TreeNode<int>* node5 = new TreeNode<int>(5);
	TreeNode<int>* node6 = new TreeNode<int>(6);
	TreeNode<int>* node7 = new TreeNode<int>(7);

	node1->setChild(node2);        // node2 is the child of node1
	node2->setSibling(node3);      // node3 is the sibling of node2
	node2->setChild(node4);        // node4 is the child of node2
	node4->setSibling(node5);      // node5 is the sibling of node4
	node3->setChild(node6);        // node6 is the child of node3
	node6->setSibling(node7);      // node7 is the sibling of node6

	TreeBase<TreeNode<int>> tree(node1);

    tree.traverseR();
	std::cout << "\n";
	tree.traverse();
	tree.printTree();

	for (Iterator<TreeNode<int>> iter = tree.begin(); iter != tree.end(); iter++) {
		std::cout << (*iter)->getValue() << " ";
	}

	std::cout << "\n";

	for (IteratorReversed<TreeNode<int>> iter = tree.rbegin(); iter != tree.rend(); iter++) {
		if (*iter) std::cout << (*iter)->getValue() << " ";
	}

	std::cout << "\n";

	tree.rotateRight(node1);
	tree.rotateLeft(node2);

	tree.printTree();
	
	std::vector<int> frequencies = { 1, 12, 13, 42, 54, 65, 70, 20, 31, 3};
	std::vector<char> keys = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'k', 'q', 'u'};

	Obst<TreeNode<char>> obst(keys, frequencies);

	obst.printTree(true);
	obst.traverse();

	TreeNode<char>* node = new TreeNode<char>('h', 8);
	obst.addNodeStatic(node);
	obst.printTree(true);

	TreeNode<char>* nodea = new TreeNode<char>('a', 1);
	obst.removeNodeStatic(nodea);
	obst.printTree(true);

	printImage<TreeNode<char>>((baseDir / "output" / "obst").string(), "png", obst);

	std::vector<int> frequencies1 = { 2, 12, 13, 42, 54, 65, 70 };
	std::vector<char> keys1 = { 'o', 'b', 'c', 'd', 'e', 'f', 'g' };
	Obst<TreeNode<char>> obst1(keys1, frequencies1);
	printImage<TreeNode<char>>((baseDir / "output" / "obst1").string(), "png", obst1);

	Obst<TreeNode<char>> difference = obst.difference(obst1);

	difference.printTree();

	printImage<TreeNode<char>>((baseDir / "output" / "graph").string(), "png", difference);

	return 0;
}