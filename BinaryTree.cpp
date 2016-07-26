#include <iostream>
#include <string.h>
#include <queue>
#include <stack>

template <typename A>
struct Node
{
	Node()
	{}

	Node(A val)
		: _val(val)
		, pLeft(NULL)
		, pRight(NULL)
	{}

	A _val;
	Node *pLeft;
	Node *pRight;
};


template <typename T>
class BinaryTree
{
public:
	BinaryTree(T data[], int n)
		: pRoot(NULL)
	{
		int index = 0;
		create(data, n, pRoot, index);
	}

	void PreOrder_Recur()
	{
		_PreOrder_Recur(pRoot, Print);
	}

	void InOrder_Recur()
	{
		_InOrder_Recur(pRoot, Print);
	}
	
	void PostOrder_Recur()
	{
		_PostOrder_Recur(pRoot, Print);
	}

	void Level()
	{
		std::queue<Node<T> *> q;
		if (NULL != pRoot)
			q.push(pRoot);

		while (!q.empty())
		{
			Node<T> *tmp = q.front();
			q.pop();
			Print(tmp);
			if (NULL != tmp->pLeft)
				q.push(tmp->pLeft);
			if (NULL != tmp->pRight)
				q.push(tmp->pRight);
		}
	}
	
	void PreOrder()
	{
		std::stack<Node<T> *> s;
		if (NULL != pRoot)
			s.push(pRoot);

		while (!s.empty())
		{
			Node<T> *tmp;
			tmp = s.top();
			s.pop();
			Print(tmp);
			
			if (NULL != tmp->pLeft)
				s.push(tmp->pLeft);
			if (NULL != tmp->pRight)
				s.push(tmp->pRight);
		}
	}

	void InOrder()
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
				std::cout << pCur->_val << " ";
				pCur = pCur->pRight;
			}
		}
	}
		
//	0
// 1		2
//3  4	      5
	void PostOrder()
	{
		std::stack<Node *> s;
		
		Node *pCur = pRoot;
		Node *pPre = NULL;
		while (pCur || !s.empty())
		{
			while (NULL != pCur)
			{
				s.push(pCur);
				pCur = pCur->pLeft;
			}
			
			Node *tmp = s.top();
			if (NULL != tmp->pRight && tmp->pRight != pPre)
			{
				pCur = tmp->pRight;
				pPre = pCur;
			}
			else
			{
				std::cout << s.top()->_val << " ";
				s.pop();
			}
		}
	}

	size_t Size()
	{
		return _Size(pRoot);
	}

	size_t Depth()
	{
		return _Depth(pRoot);
	}

	size_t Leaves()
	{
		return _Leaves(pRoot);
	}

	BinaryTree<T> Mirror()
	{
		BinaryTree<T> ret(*this);
		_Mirror(ret.pRoot);
		return ret;
	}

	Node<T> * Find(const T &data)
	{
		return _Find(pRoot, data);
	}
	
	bool IsInTree(Node<T> *pNode)
	{
		return _IsInTree(pRoot, pNode);
	}
	
	Node<T> * NearestAncestor(Node<T> *p1, Node<T> *p2)
	{
		if (!_IsInTree(pRoot, p1) || !_IsInTree(pRoot, p2))
			return NULL;
		Node<T> *pCur = pRoot;
		
		bool p1_left; 
                bool p1_right;
                bool p2_left; 
		bool p2_right;
		while (pCur)
		{
			p1_left = _IsInTree(pCur->pLeft, p1);
			p1_right = _IsInTree(pCur->pRight, p1);
			p2_left = _IsInTree(pCur->pLeft, p2);
			p2_right = _IsInTree(pCur->pRight, p2);
			
			if ((p1_left && p2_right) || (p1_right && p2_left) || p1 == pCur || p2 == pCur)
				return pCur;
			if (p1_left)	//两结点均在左子树
			{
				pCur = pCur->pLeft;
			}
			else		//右
			{
				pCur = pCur->pRight;
			}
		}
		
		return NULL;
	}
	
	Node<T> * NearestAncestor2(Node<T> *p1, Node<T> *p2)
	{
		std::stack<Node<T> *> s1;
		std::stack<Node<T> *> s2;
		Road(s1, p1);
		Road(s2, p2);

		int size = s1.size() - s2.size();
		if (size > 0)	
		{
			while (size--)
				s1.pop();
		}
		else
		{
			while (size++)
				s2.pop();
		}
		
		while (!s1.empty())
		{
			if (s1.top() == s2.top())
				return s1.top();
			else
			{
				s1.pop();
				s2.pop();
			}
		}

		return NULL;
	}



private:
	void Road(std::stack<Node<T> *> &s, Node<T> *pNode)
	{
		if (NULL != pRoot && NULL != pNode)
			s.push(pRoot);
		
		if (pRoot == pNode)
			return;

		Node<T> *pCur;
		Node<T> *pPreLeft = NULL;
		Node<T> *pPreRight = NULL;
		while (!s.empty())
		{
			pCur = s.top();
			
			while (NULL != pCur->pLeft && pCur->pLeft != pPreLeft)
			{
				s.push(pCur->pLeft);
				pCur = pCur->pLeft;
				if (pCur == pNode)
					return;
			}
			
			if (NULL != pCur->pRight && pCur->pRight != pPreRight)
			{
				s.push(pCur->pRight);
				pPreRight = pCur->pRight;
				if (pPreRight == pNode)
					return;
			}
			else
			{
				if (s.top() != pPreRight)
					pPreLeft = pCur;
				s.pop();
			}
		}
	}


	bool _IsInTree(Node<T> *pRoot, Node<T> *pNode)
	{
		if (NULL == pRoot || NULL == pNode)
			return false;

		if (pRoot == pNode)
			return true;

		bool ret = _IsInTree(pRoot->pLeft, pNode);
		if (ret)
			return true;
		return _IsInTree(pRoot->pRight, pNode);
	}

	Node<T> * _Find(Node<T> *pRoot, const T &data) 
	{
		if (NULL != pRoot)
		{
			if (pRoot->_val == data)
				return pRoot;
			else
			{	Node<T> *ret;
				ret = _Find(pRoot->pLeft, data);
				if (ret)
					return ret;
				ret = _Find(pRoot->pRight, data);
				if (ret)
					return ret;
			}
		}
		return NULL;
	}

	void _Mirror(Node<T> *pRoot)
	{
		if (NULL != pRoot)
		{
			std::swap(pRoot->pLeft, pRoot->pRight);
			_Mirror(pRoot->pLeft);
			_Mirror(pRoot->pRight);
		}
	}

	size_t _Leaves(Node<T> * pRoot)
	{
		if (NULL == pRoot)
			return 0;
		if (NULL == pRoot->pLeft && NULL == pRoot->pRight)
			return 1;

		return _Leaves(pRoot->pLeft) + _Leaves(pRoot->pRight);
	}

	size_t _Depth(Node<T> *pRoot)
	{
		if (NULL == pRoot)
			return -1;

		if (NULL == pRoot->pLeft && NULL == pRoot->pRight)
			return 0;
		
		size_t left = _Depth(pRoot->pLeft);
		size_t right = _Depth(pRoot->pRight);

		return left > right ? left + 1 : right + 1;
	}

	size_t _Size(Node<T> *pRoot)
	{
		if (NULL != pRoot)
		{
			return 1 + _Size(pRoot->pLeft) + _Size(pRoot->pRight);	
		}

		return 0;
	}

	void _PreOrder_Recur(Node<T> *_pRoot, void (*visit)(Node<T> *))
	{
		if (NULL != _pRoot)
		{
			visit(_pRoot);
			_PreOrder_Recur(_pRoot->pLeft, visit);
			_PreOrder_Recur(_pRoot->pRight, visit);
		}
	}
	
	void _InOrder_Recur(Node<T> *_pRoot, void (*visit)(Node<T> *))
	{
		if (NULL != _pRoot)
		{
			_InOrder_Recur(_pRoot->pLeft, visit);
			visit(_pRoot);
			_InOrder_Recur(_pRoot->pRight, visit);
		}
	}

	void _PostOrder_Recur(Node<T> *_pRoot, void (*visit)(Node<T> *))
	{
		if (NULL != _pRoot)
		{
			_PostOrder_Recur(_pRoot->pLeft, visit);
			_PostOrder_Recur(_pRoot->pRight, visit);
			visit(_pRoot);
		}
	}
	
	static void Print(Node<T> *pNode)
	{
		std::cout << pNode->_val << " ";
	}

	void create(T data[], int n, Node<T> * &_pRoot, int &index)
	{
		if (index < n && data[index] != '#')
		{
			_pRoot = new Node<T>(data[index]);
			create(data, n, _pRoot->pLeft, ++index);
			create(data, n, _pRoot->pRight, ++index);
		}
	}

private:
	Node<T> *pRoot;
};

//	0
//  1	    2
//3   4   5
int main()
{
	char PreOrder[] = "013##4##25";
	BinaryTree<char> b(PreOrder, strlen(PreOrder));
	
//	b.PreOrder_Recur();
//	std::cout << std::endl;
//	b.PreOrder_Recur();
//	std::cout << std::endl;
//	
//	std::cout << std::endl;
//
//	b.InOrder_Recur();
//	std::cout << std::endl;
//	b.InOrder();
//	std::cout << std::endl;
//	
//	std::cout << std::endl;
//	
	b.PostOrder_Recur();
	std::cout << std::endl;
	b.PostOrder();
	std::cout << std::endl;
//	
//	std::cout << std::endl;
//	
//	b.Level();
//	std::cout << std::endl;
//
//	std::cout << b.Size() << std::endl;
//	std::cout << b.Depth() << std::endl;
//	std::cout << b.Leaves() << std::endl;
//	
//	BinaryTree<char> ret = b.Mirror();
//	ret.PreOrder_Recur();
//	std::cout << std::endl;

//	Node<char> *ret = b.Find('4');
//	std::cout << b.IsInTree(ret) << std::endl;
	

//	Node<char> *p1 = b.Find('3');
//	Node<char> *p2 = b.Find('4');
//	Node<char> *ret = b.NearestAncestor(p1, p2);
//	std::cout << ret->_val << std::endl;

//	Node<char> *ret = b.NearestAncestor2(p1, p2);
//	std::cout << ret->_val << std::endl;

	return 0;
}
