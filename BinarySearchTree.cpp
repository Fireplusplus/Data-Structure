/*
 * 二叉搜索树/二叉排序树
 */

#include <iostream>
#include <queue>
#include <stack>

template <typename T>
struct BSTNode
{
	BSTNode(const T &key)
		: _key(key)
		, pLeft(NULL)
		, pRight(NULL)
	{}

	BSTNode *pLeft;
	BSTNode *pRight;
	T	_key;
};

template <typename T>
class BSTree
{
	typedef BSTNode<T> Node;
public:
	BSTree()
		: pRoot(NULL)
	{}
	
	bool Insert(const T &key)
	{
		// 空树直接插入
		if (NULL == pRoot)
		{
			pRoot = new Node(key);
		}
		else
		{
			Node *pParent = NULL;
			Node *pCur = pRoot;
			
			// 查找要插入的位置
			while (NULL != pCur)
			{
				if (pCur->_key > key)
				{
					pParent = pCur;
					pCur = pCur->pLeft;
				}
				else if (pCur->_key < key)
				{
					pParent = pCur;
					pCur = pCur->pRight;
				}
				else
					return false;
			}
			
			if (key > pParent->_key)
				pParent->pRight = new Node(key);
			else
				pParent->pLeft = new Node(key);
		}
	
		return true;
	}

	bool Remove(const T &key)
	{
		if (NULL == pRoot)
			return false;

		// 先找被删除结点，同时保存其双亲结点
		Node *pParent = NULL;
		Node *pCur = pRoot;
		
		while (NULL != pCur)
		{
			if (key > pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->pRight;
			}
			else if (key < pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->pLeft;
			}
			else
				break;
		}

		// 没找到
		if (NULL == pCur)
			return false;

		// 1.被删除结点无左孩子
		// 2.被删除结点无右孩子
		// 3.被删除结点左右孩子同时有
		// 注意根节点并无双亲结点，特殊处理！
		if (NULL == pCur->pLeft)
		{
			if (pCur == pRoot)
				pRoot = pCur->pRight;			
			else
			{
				if (pCur == pParent->pLeft)
					pParent->pLeft = pCur->pRight;
				else
					pParent->pRight = pCur->pRight;
			}
		}
		else if (NULL == pCur->pRight)
		{
			if (pCur == pRoot)
				pRoot = pCur->pLeft;
			else
			{
				if (pCur == pParent->pLeft)
					pParent->pLeft = pCur->pLeft;
				else
					pParent->pRight = pCur->pLeft;
			}
		}
		else
		{
			// 将右子树的最左结点(大于当前被删除结点的最小值)的值赋给之前的结点，
			// 转换为删除这个结点
			pParent = pCur;
			pCur = pCur->pRight;
			while (NULL != pCur->pLeft)
			{
				pParent = pCur;
				pCur = pCur->pLeft;
			}
			pParent->_key = pCur->_key;
			if (pCur == pParent->pLeft)
				pParent->pLeft = pCur->pRight;
			else
				pParent->pRight = pCur->pRight;
		}
		delete pCur;
	}
	
	Node * Find(const T &key)
	{
		Node *pCur = pRoot;
		
		while (NULL != pCur)
		{
			if (pCur->_key > key)	
				pCur = pCur->pLeft;
			else if (pCur->_key < key)
				pCur = pCur->pRight;
			else
				return pCur;
		}
		
		return NULL;
	}

	void InOrder()
	{
		_InOrder(pRoot);
		std::cout << std::endl;
	}
	
	int Height()
	{
		return _Height(pRoot);
	}

	int LeafNums()
	{
		return _LeafNums(pRoot);
	}

	void LevelOrder()
	{
		std::queue<Node *> q;	
		if (NULL != pRoot)
			q.push(pRoot);	
		
		while (!q.empty())
		{
			Node *pCur = q.front();
			q.pop();
			std::cout << pCur->_key << " ";
			
			if (NULL != pCur->pLeft)
				q.push(pCur->pLeft);
			if (NULL != pCur->pRight)
				q.push(pCur->pRight);
		}
		std::cout << std::endl;
	}

	void PrevOrderNonR()
	{
		std::stack<Node *> s;	
		if (NULL != pRoot)
			s.push(pRoot);
		
		while (!s.empty())
		{
			Node *pCur = s.top();
			s.pop();

			std::cout << pCur->_key << " ";
			if (NULL != pCur->pRight)
				s.push(pCur->pRight);
			if (NULL != pCur->pLeft)
				s.push(pCur->pLeft);	
		}
		std::cout << std::endl;
	}

	void InOrderNonR()
	{	
		std::stack<Node *> s;
		Node *pCur = pRoot;
		
		while (pCur || !s.empty())
		{
			if (NULL != pCur)
			{
				s.push(pCur);
				pCur = pCur->pLeft;
			}
			else
			{
				pCur = s.top();
				s.pop();
				std::cout << pCur->_key << " ";
				pCur = pCur->pRight;
			}
		}
		std::cout << std::endl;
	}

	void PostOrderNonR()
	{
		if (NULL == pRoot)
			return;

		std::stack<Node *> s;

		Node *pCur = pRoot, *pPrev = NULL;

		// 先将pCur移动到左子树最下边
		while (NULL != pCur)
		{
			s.push(pCur);
			pCur = pCur->pLeft;
		}
		
		while (!s.empty())
		{
			// 到这里，pCur都是空，并且已经遍历到左子树底端
			pCur = s.top();
			// 一个根节点被访问的前提是无右子树或右子树被访问过
			if (NULL == pCur->pRight || pPrev == pCur->pRight)
			{
				s.pop();
				std::cout << pCur->_key << " ";
				pPrev = pCur;	
			}
			else	// 左子树刚被访问过，需进入右子树(有右子树且未被访问过)
			{
				// 进入右子树，且右子树一定不为空
				pCur = pCur->pRight;
				while (NULL != pCur)
				{
					s.push(pCur);
					pCur = pCur->pLeft;
				}
			}
		}
		std::cout << std::endl;
	}

private:
	int _LeafNums(Node *root)
	{
		if (NULL == root)
			return 0;
		if (NULL == root->pLeft && NULL == root->pRight)
			return 1;

		return _LeafNums(root->pLeft) + _LeafNums(root->pRight);
	}

	int _Height(Node *root)
	{
		if (NULL == root)
			return 0;
		if (NULL == root->pLeft && NULL == root->pRight)
			return 1;

		int left = _Height(root->pLeft);
		int right = _Height(root->pRight);
		
		return left > right ? left + 1 : right + 1;
	}

	void _InOrder(Node *root)
	{
		if (NULL != root)
		{
			_InOrder(root->pLeft);
			std::cout << root->_key << " ";
			_InOrder(root->pRight);
		}
	}

private:
	Node *pRoot;
};

int main()
{
	BSTree<int> t;
	
	t.Insert(5);
	t.Insert(3);
	t.Insert(7);
	t.Insert(1);
	t.Insert(4);
	t.Insert(6);
	t.Insert(8);
	t.Insert(0);
	t.Insert(2);
	t.Insert(9);
	
//	t.InOrder();
//
//	t.Remove(0);
//	t.Remove(1);
//	t.Remove(9);
//	t.Remove(7);
//
//	t.InOrder();
//
//	for (int i = 0; i < 10; i++)
//		t.Remove(i);
//	t.InOrder();

//	t.Find(9);
//	for (int i = 0; i <= 10; i++)
//	{
//		BSTNode<int> *ret = t.Find(i);
//		if (NULL != ret)
//			std::cout << ret->_key << std::endl;
//		else
//			std::cout << i << " not found !" << std::endl;
//	}

//	std::cout << t.Height() << std::endl;
//	std::cout << t.LeafNums() << std::endl;

//	t.LevelOrder();
//	t.PrevOrderNonR();
//	t.InOrderNonR();
	t.PostOrderNonR();

	return 0;
}
