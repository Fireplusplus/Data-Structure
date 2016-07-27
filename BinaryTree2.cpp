#include <iostream>
#include <string.h>
#include <queue>
#include <stack>

struct Node
{
	Node (char val)
		: _val(val)
		, pLeft(NULL)
		, pRight(NULL)
	{}

	char _val;
	Node *pLeft;
	Node *pRight;
};



void Create(Node *&_pRoot, char arr[], int n, int &index)
{
	if (index < n && arr[index] != '#')
	{
		_pRoot = new Node(arr[index]);
		Create(_pRoot->pLeft, arr, n, ++index);
		Create(_pRoot->pRight, arr, n, ++index);
	}
}

void PreOrderRecur(Node *_pRoot)
{
	if (NULL != _pRoot)
	{
		std::cout << _pRoot->_val << " ";
		PreOrderRecur(_pRoot->pLeft);
		PreOrderRecur(_pRoot->pRight);
	}
}

void InOrderRecur(Node *_pRoot)
{
	if (NULL != _pRoot)
	{
		InOrderRecur(_pRoot->pLeft);
		std::cout << _pRoot->_val << " ";
		InOrderRecur(_pRoot->pRight);			
	}
}

void PostOrderRecur(Node *_pRoot)
{
	if (NULL != _pRoot)
	{
		PostOrderRecur(_pRoot->pLeft);
		PostOrderRecur(_pRoot->pRight);
		std::cout << _pRoot->_val << " ";
	}
}

void LevelTraverse(Node *_pRoot)
{
	std::queue<Node *> q;

	if (NULL != _pRoot)
		q.push(_pRoot);

	while (!q.empty())
	{
		Node *pCur = q.front();
		std::cout << pCur->_val << " ";
		q.pop();
		if (NULL != pCur->pLeft)
		{
			q.push(pCur->pLeft);
		}
		if (NULL != pCur->pRight)
		{
			q.push(pCur->pRight);
		}
	}
}

void PreOrder(Node *_pRoot)
{
	std::stack<Node *> s;

	if (NULL != _pRoot)
		s.push(_pRoot);

	while (!s.empty())
	{
		Node *pCur = s.top();
		s.pop();
		std::cout << pCur->_val << " ";
		
		if (NULL != pCur->pRight)
		{
			s.push(pCur->pRight);
		}
		if (NULL != pCur->pLeft)
		{
			s.push(pCur->pLeft);
		}
	}
}

void InOrder(Node *_pRoot)
{
	std::stack<Node *> s;
	
	Node *pCur = _pRoot;
	while (pCur || !s.empty())
	{
		while (NULL != pCur)	//根指针进栈，遍历左子树
		{
			s.push(pCur);
			pCur = pCur->pLeft;
		}
	//	else			//根指针退栈，遍历右子树
	//	{
			pCur = s.top();
			s.pop();
			std::cout << pCur->_val << " ";
			pCur = pCur->pRight;
	//	}
	}
}

void PostOrder(Node *_pRoot)
{
	std::stack<Node *> s;
	
	Node *pCur = _pRoot;
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

size_t Height(Node *_pRoot)
{
	if (NULL == _pRoot)
		return 0;
	if (NULL == _pRoot->pLeft && NULL == _pRoot->pRight)
		return 1;

	size_t left = Height(_pRoot->pLeft);
	size_t right = Height(_pRoot->pRight);
	return left > right ? left + 1 : right + 1;
}

size_t Leaves(Node *_pRoot)
{
	if (NULL == _pRoot)
		return 0;
	if (NULL == _pRoot->pLeft && NULL == _pRoot->pRight)
		return 1;
	
	return Leaves(_pRoot->pLeft) + Leaves(_pRoot->pRight);
}

size_t Size(Node *_pRoot)
{
	if (NULL == _pRoot)
		return 0;
	if (NULL == _pRoot->pLeft && NULL == _pRoot->pRight)
		return 1;
	
	return 1 + Size(_pRoot->pLeft) + Size(_pRoot->pRight);
}

void Mirror(Node *_pRoot)
{
	if (NULL == _pRoot)
		return;
	std::swap(_pRoot->pLeft, _pRoot->pRight);
	Mirror(_pRoot->pLeft);
	Mirror(_pRoot->pRight);
}

Node * Find(Node *_pRoot, char data)
{
	if (NULL == _pRoot || _pRoot->_val == data)
		return _pRoot;
	
	Node *ret = Find(_pRoot->pLeft, data);
	if (NULL != ret)
		return ret;
	return Find(_pRoot->pRight, data);
}

bool IsInTree(Node *_pRoot, Node *pNode)
{
	if (NULL == _pRoot || NULL == pNode)
		return false;
	
	if (_pRoot == pNode)
		return true;

	if (IsInTree(_pRoot->pLeft, pNode))
		return true;
	return IsInTree(_pRoot->pRight, pNode);	
}

//结点不是自身的祖先
Node * NearestAncestor(Node *_pRoot, Node *p1, Node *p2)
{
	if (NULL == _pRoot || !IsInTree(_pRoot, p1) || !IsInTree(_pRoot, p2))
		return NULL;
	
	bool p1_left;
	bool p2_left;
	bool p1_right;
	bool p2_right;
	
	Node *pCur = _pRoot;
	Node *pPre = NULL;
	while (NULL != pCur)
	{
		p1_left = IsInTree(pCur->pLeft, p1);
		p2_left = IsInTree(pCur->pLeft, p2);
		p1_right = IsInTree(pCur->pRight, p1);
		p2_right = IsInTree(pCur->pRight, p2);
	
		if ((p1_left && p2_right) || (p1_right && p2_left))
			return pCur;
		else if ((!p1_left && !p1_right) || (!p2_left && !p2_right))
			return pPre;
		
		else if(p1_left) //均在左子树
		{
			pPre = pCur;
			pCur = pCur->pLeft;
		}
		else 
		{
			pPre = pCur;
			pCur = pCur->pRight;
		}
	}

	return NULL;
}

bool RoadRecur(std::stack<Node *> &s, Node *_pRoot, Node *pNode)
{
	if (NULL == pNode)
		return false;
	
	s.push(_pRoot);
	if (NULL == _pRoot)
	{
		s.pop();
		return false;
	}
	else if (_pRoot == pNode)
	{
		return true;
	}
	else
	{
		if (RoadRecur(s, _pRoot->pLeft, pNode))
		{
			return true;
		}

		return RoadRecur(s, _pRoot->pRight, pNode);
	}

	return false;
}

void Road(std::stack<Node *> &s, Node *_pRoot, Node *pNode)
{
	if (NULL != _pRoot || NULL != pNode)
		s.push(_pRoot);
	
	if (_pRoot == pNode)
		return;

	Node *pCur = _pRoot;
	Node *left = NULL;
	Node *right = NULL;
	while (!s.empty())
	{
		pCur = s.top();
		while (NULL != pCur->pLeft && left != pCur->pLeft)
		{
			s.push(pCur->pLeft);
			pCur = pCur->pLeft;
			if (pCur == pNode)
				return;
		}
		
		if (NULL != pCur->pRight && pCur->pRight != right)
		{
			pCur = pCur->pRight;
			s.push(pCur);
			if (pCur == pNode)
				return;
			right = pCur;
		}
		else
		{
			if (s.top() != right)
				left = pCur;
			s.pop();
		}

	}
}

Node * NearestAncestor2(Node *_pRoot, Node *p1, Node *p2)
{
	std::stack<Node *> s1;
	std::stack<Node *> s2;

	Road(s1, _pRoot, p1);
	Road(s2, _pRoot, p2);

	int sz = s1.size() - s2.size();
	if (sz > 0)
	{
		while (sz--)
			s1.pop();
	}
	else
	{
		while (sz++)
		{
			s2.pop();
		}
	}
	
	while (!s1.empty())
	{
		if (s1.top() == s2.top())
		{
			if (p1 == s1.top() || p2 == s2.top())
				s1.pop();
			
			if (!s1.empty())
				return s1.top();
			return NULL;
		}
		s1.pop();
		s2.pop();
	}

	return NULL;
}

//找到第一个度不为2的结点，则后续所有结点皆不能有孩子
bool IsComplete(Node *_pRoot)
{
	if (NULL == _pRoot)
		return true;
	
	std::queue<Node *> q;
	q.push(_pRoot);
	
	bool flag = false;
	while (!q.empty())
	{
		if (flag)
		{
			Node *pCur = q.front();
			if (NULL != pCur->pLeft || NULL != pCur->pRight)
				return false;
			q.pop();
		}
		
		if (!q.empty())
		{
			Node *tmp = q.front();
			q.pop();
			if (NULL == tmp->pLeft && NULL != tmp->pRight)
				return false;
			
			if (NULL == tmp->pLeft || NULL == tmp->pRight)
				flag = true;
			
			if (NULL != tmp->pLeft)
				q.push(tmp->pLeft);
			if (NULL != tmp->pRight)
				q.push(tmp->pRight);
		}
	}
	return true;
}

int main()
{
	//char arr[] = "013##4##25";
	//char arr[] = "9013##4##25";
	//char arr[] = "01";
	//char arr[] = "0#1";
	char arr[] = "012";
	Node *pRoot;
	
	int index = 0;
	Create(pRoot, arr, strlen(arr), index);
	
//	PreOrderRecur(pRoot);
//	std::cout << std::endl;
//	PreOrder(pRoot);
//	std::cout << std::endl;
//	
//	InOrderRecur(pRoot);
//	std::cout << std::endl;
//	InOrder(pRoot);
//	std::cout << std::endl;
//	
//	PostOrderRecur(pRoot);
//	std::cout << std::endl;
//	PostOrder(pRoot);
//	std::cout << std::endl;
//
//	LevelTraverse(pRoot);
//	std::cout << std::endl;
//	
//	std::cout << "Height: " << Height(pRoot) << std::endl;
//	std::cout << "Leaves: " << Leaves(pRoot) << std::endl;
//	std::cout << "Size: " << Size(pRoot) << std::endl;

//	Node *p1 = Find(pRoot, '3');
//	Node *p2 = Find(pRoot, '0');
////	std::cout << p1->_val << std::endl;
//////	std::cout << p2->_val << std::endl;
//////	std::cout << IsInTree(pRoot, p1) << std::endl;
//////	std::cout << IsInTree(pRoot, NULL) << std::endl;
//	Node *ret = NearestAncestor2(pRoot, p1, p2);
//		std::cout << ret->_val << std::endl;
	
	bool ret = IsComplete(pRoot);
	std::cout << ret << std::endl;

	return 0;
}
