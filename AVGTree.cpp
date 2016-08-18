#include <iostream>
#include <stdlib.h>

template <typename K, typename V>
struct AVGNode
{
	K _key;
	V _value;
	AVGNode *_left;
	AVGNode *_right;
	AVGNode *_parent;

	int _bl;		// 平衡因子

	AVGNode(const K &key, const V &value)
		: _key(key)
		, _value(_value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _bl(0)
	{}
};

template <typename K, typename V>
class AVGTree
{
	typedef AVGNode<K, V> Node;
public:
	AVGTree()
		: _root(NULL)
	{}

	bool Insert(const K &key, const V &value)
	{
		// 空树，直接插
		if (NULL == _root)
		{
			_root = new Node(key, value);		
			return true;
		}
		
		Node *cur = _root;
		Node *parent = NULL;
		while (NULL != cur)
		{
			parent = cur;
			if (cur->_key > key)
				cur = cur->_left;
			else if (cur->_key < key)
				cur = cur->_right;
			else
				return false;
		}
		
		if (parent->_key > key)
		{
			parent->_left = new Node(key, value);
			parent->_left->_parent = parent;
			cur = parent->_left;
		}
		else 
		{
			parent->_right = new Node(key, value);
			parent->_right->_parent = parent;
			cur = parent->_right;
		}

		// 调整平衡因子
		while (NULL != parent)
		{
			if (cur == parent->_right)
				parent->_bl++;
			else
				parent->_bl--;

			if (0 == parent->_bl)
				break;
			else if (-1 == parent->_bl || 1 == parent->_bl)
			{
				cur = parent;
				parent = cur->_parent;
			}
			else	// 2 or -2
			{
				if (2 == parent->_bl)		// 右高，左单旋
				{
					if (1 == cur->_bl)
						RotateL(parent);
					else
						RotateRL(parent);
				}
				else if (-2 == parent->_bl)	// 左高，右单旋
				{
					if (-1 == cur->_bl)
					{
						RotateR(parent);
					}
					else
						RotateLR(parent);
				}
				break;
			}
		}

		return true;
	}
	
	bool IsBalance()
	{
		int height;
		return _IsBalance(_root, height);
	}

	void InOrder()
	{
		_InOrder(_root);
		std::cout << std::endl;
	}
private:
	bool _IsBalance(Node *root, int &height)
	{
		if (NULL == root)
		{
			height = 0;
			return true;
		}
		int left, right;
		if (!_IsBalance(root->_left, left))
			return false;
		
		if (!_IsBalance(root->_right, right))
			return false;
		
		height = left > right ? left + 1 : right + 1;

		if (root->_bl != right - left)
			std::cout << root->_key << std::endl;

		if (abs(left - right) > 1)
			return false;
		else
			return true;
	}

	void _InOrder(Node *root)
	{
		if (NULL == root)
			return;

		_InOrder(root->_left);
		std::cout << root->_key << " ";
		_InOrder(root->_right);
	}

	void RotateRL(Node *parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		int bl = subRL->_bl;

		RotateR(parent->_right);
		RotateL(parent);
		
		if (bl == 1)
		{
			subR->_bl = 0;
			parent->_bl = -1;
		}
		else if (bl == -1)
		{
			subR->_bl = 1;
			parent->_bl = 0;
		}
		else
			subR->_bl = parent->_bl = 0;
		subRL->_bl = 0;
	}

	void RotateLR(Node *parent)
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		int bl = subLR->_bl;

		RotateL(parent->_left);
		RotateR(parent);

		if (bl == 1)
		{
			subL->_bl = -1;
			parent->_bl = 0;
		}
		else if (bl == -1)
		{
			subL->bl = 0;
			parent->_bl = 1;
		}
		else
			subL->_bl = parent->_bl = 0;
		subLR = 0;
	}

	void RotateL(Node *root)
	{
		Node *subR = root->_right;
		Node *subRL = subR->_left;
		Node *root_parent = root->_parent;
		
		subR->_left = root;
		root->_parent = subR;
		subR->_bl = 0;

		root->_right = subRL;
		if (NULL != subRL)
			subRL->_parent = root;
		root->_bl = 0;

		subR->_parent = root_parent;
		if (root == _root)
		{
			_root = subR;
		}
		else
		{
			if (root_parent->_left == root)
				root_parent->_left = subR;
			else
				root_parent->_right = subR;
		}
	}
	
	void RotateR(Node *root)
	{
		Node *subL = root->_left;
		Node *subLR = subL->_right;
		Node *root_parent = root->_parent;
		
		subL->_right = root;
		root->_parent = subL;
		subL->_bl = 0;

		root->_left = subLR;
		if (NULL != subLR)
			subLR->_parent = root;
		root->_bl = 0;
		
		subL->_parent = root_parent;
		if (root == _root)
		{
			_root = subL;
		}
		else
		{
			if (root_parent->_left == root)
				root_parent->_left = subL;
			else
				root_parent->_right = subL;
		}
	}
private:
	Node *_root;	
};

int main()
{
	AVGTree<int, int> t;
	t.Insert(1, 1);
	t.Insert(2, 2);
	t.Insert(3, 3);
	t.Insert(4, 4);
	t.Insert(5, 5);
	t.Insert(6, 6);
	t.Insert(7, 7);
	t.Insert(8, 8);
	t.Insert(9, 9);
	t.Insert(10, 10);

	t.InOrder();

	if (t.IsBalance())
		std::cout << "yes" << std::endl;
	else 
		std::cout << "no" << std::endl;
	
	return 0;
}
