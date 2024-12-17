#pragma once


template<typename _ValueT>
class TreeNode {

public:
	using ValueT = _ValueT;

	TreeNode(): TreeNode<ValueT>(0, 0) {}
	TreeNode(const ValueT& value): TreeNode<ValueT>(value, 0) {}
	TreeNode(const ValueT& value, const int& weight): 
		_value(value), 
		_weight(weight), 
		_child(nullptr), 
		_sibling(nullptr) {}
	TreeNode(const TreeNode&) = delete; // do not create a shallow copy
	TreeNode& operator=(const TreeNode&) = delete; // do not allow shalllow copying
	bool operator==(const TreeNode& other) const { return _value == other._value && _weight == other._weight; }

	/* ---------- Setters and getters ----------- */

	inline void setChild(TreeNode* child) { _child = child; }
	inline TreeNode* getChild() const { return _child; }
	inline void setSibling(TreeNode* sibling) { _sibling = sibling; }
	inline TreeNode* getSibling() const { return _sibling; }
	inline void setValue(const ValueT& value) { _value = value; }
	inline const ValueT& getValue() const  { return _value; }
	inline const int getWeight() const { return _weight; }
	inline void setWeight(int weight) { _weight = weight; }

	/* ------------------------------------------- */

private:
	ValueT _value;
	TreeNode* _child;
	TreeNode* _sibling;
	int _weight;
};