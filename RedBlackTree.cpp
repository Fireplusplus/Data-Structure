#include <iostream>

enum Color
{
	red,
	black
};

template <typename K, typename V>
struct RBNode
{
	RBNode(const K &key, const V &value)
		: _key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _color(red)
	{}

	K _key;
	V _value;
	RBNode *_left;
	RBNode *_right;
	RBNode *_parent;
	Color _color;	
};

template <typename K, typename V>
class RBTree
{
	typedef RBNode<K, V> Node;
public:
	RBTree()
		: _root(NULL)
	{}

	~RBTree()
	{
		_Destroy(_root);
		_root = NULL;
	}

	bool Insert(const K &key, const V &value)
	{
		// 空树直接插
		if (NULL == _root)
		{
			_root = new Node(key, value);
			_root->_color = black;
			return true;
		}

		// 非空树，寻找插入位置
		Node *parent;
		Node *cur = _root;
		while (NULL != cur)
		{
			parent = cur;
			if (cur->_key > key)
				cur = cur->_left;
			else
				cur = cur->_right;
		}
		
		cur = new Node(key, value); 
		if (parent->_key > key)
			parent->_left = cur;
		else
			parent->_right = cur;
		cur->_parent = parent; 
		
		// 调整
		while (NULL != parent && parent->_color == red)
		{
			Node *grandp = parent->_parent;
			Node *uncle;
			if (parent == grandp->_left)
				uncle = grandp->_right;
			else
				uncle = grandp->_left;
			
			// 1.u为红
			if (NULL != uncle && red == uncle->_color)
			{
				grandp->_color = red;
				parent->_color = black;
				uncle->_color = black;

				// 继续向上调
				cur = grandp;
				parent = cur->_parent;
			}
			else	// 2.不存在或为黑
			{

				if (parent == grandp->_left)
				{
					if (cur == parent->_left)
						RotateR(grandp);
					else
					{
						RotateL(parent);
						std::swap(parent, cur);	//与颜色设置有关,因为parent会被移下去，然后回到if的情况
						RotateR(grandp);
					}
				}
				else
				{
					if (cur == parent->_right)
						RotateL(grandp);
					else
					{
						RotateR(parent);
						std::swap(parent, cur);	//与颜色设置有关,因为parent会被移下去，然后回到if的情况
						RotateL(grandp);
					}
				}
				parent->_color = black;
				grandp->_color = red;
				break;
			}
		}
		_root->_color = black;

		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		std::cout << std::endl;
	}
	
	bool IsBalance()
	{
		if (NULL == _root)
			return true;
		if (_root->_color != black)
			return false;

		// 判断是否有连续红结点
		if (IsRedContinue(_root))
		{
			//std::cout << "存在连续红结点" << std::endl;
			return false;
		}
		// 判断每条路径黑结点个数是否相等
		int nums = 0;
		Node *cur = _root;
		while (NULL != cur)
		{
			if (cur->_color == black)
				nums++;
			cur = cur->_left;
		}

		return IsBlackEqual(_root, nums, 0);
	}

	
private:
	bool IsBlackEqual(Node *root, int base, int n)
	{
		if (NULL == root)
			return true;

		if (root->_color == black)
			n++;

		if (root->_left == NULL && root->_right == NULL)
		{
			if (n == base)
				return true;
			else
			{
				//std::cout << "key: " << root->_key << std::endl;
				return false;
			}
		}
	

		return IsBlackEqual(root->_left, base, n) && IsBlackEqual(root->_right, base, n);
	}

	void _Destroy(Node *_root)
	{
		if (NULL == _root)
			return;
		
		_Destroy(_root->_left);
		_Destroy(_root->_right);
		delete _root;
	}

	bool IsRedContinue(Node *root)
	{
		if (NULL == root)
			return false;
		
		Node *parent = root->_parent;
		if (root->_color == red && parent->_color == red)
		{	
			//std::cout << "root: " << root->_key << "parent: " << parent->_key << std::endl;
			return true;
		}

		if (IsRedContinue(root->_left))
			return true;
		
		return IsRedContinue(root->_right);
	}

	void _InOrder(Node *root)
	{
		if (NULL == root)
			return;

		_InOrder(root->_left);
		std::cout << root->_key << " ";
		_InOrder(root->_right);
	}

	void RotateL(Node *root)
	{
		Node *subR = root->_right;
		Node *subRL = subR->_left;
		Node *ppNode = root->_parent;

		subR->_left = root;
		root->_parent = subR;

		root->_right = subRL;
		if (NULL != subRL)
			subRL->_parent = root;
		
		if (NULL == ppNode)
			_root = subR;
		else
		{
			if (root == ppNode->_left)
				ppNode->_left = subR;
			else
				ppNode->_right = subR;
		}	
		subR->_parent = ppNode; //!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	void RotateR(Node *root)
	{
		Node *subL = root->_left;
		Node *subLR = subL->_right;
		Node *ppNode = root->_parent;

		subL->_right = root;
		root->_parent = subL;

		root->_left = subLR;
		if (NULL != subLR)
			subLR->_parent = root;

		if (NULL == ppNode)
			_root = subL;
		else
		{
			if (root == ppNode->_left)
				ppNode->_left = subL;
			else
				ppNode->_right = subL;
		}
		subL->_parent = ppNode; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!不要再忘了！！！
	}

	
private:
	Node *_root;
};

int main()
{
	int arr[] = {20, 5, 6, 33, 38, 40, 39, 10, 9};
	
	RBTree<int, int> t;
	for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
	{
		std::cout << arr[i] << std::endl;
		t.Insert(arr[i], i);
	}

	t.InOrder();

	if (t.IsBalance())
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
		
	return 0;
}
