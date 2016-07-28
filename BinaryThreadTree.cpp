#include <iostream>

enum Flag
{
	LINK,
	THREAD
};

template <typename T1>
struct Node
{
	Node(T1 &val)
		: _val(val)
		, pLeft(NULL)
		, pRight(NULL)
		, left(LINK)
		, right(LINK)
	{}

	T1 _val;
	Node *pLeft;
	Node *pRight;
	Flag left;
	Flag right;
};

template <typename T>
class BinaryThreadTree
{
public:
	BinaryThreadTree()
		: _pRoot(NULL)
	{}

	BinaryThreadTree(T *arr, int size)
		: _pRoot(NULL)
	{
		if (NULL == arr || size <= 0)
			return;
		int index = 0;
		Create(_pRoot, arr, size, index);
		Node<T> *pPre = NULL;
		Threading(_pRoot, pPre);
	}
	
	void PreOrder()
	{
		_PreOrder(_pRoot);
	}

private:
	void _PreOrder(Node<T> *pRoot)
	{
		if (NULL != pRoot)
			std::cout << pRoot->_val << " ";
			
		while (NULL != pRoot->pLeft && LINK == pRoot->left)
		{
			pRoot = pRoot->pLeft;
			std::cout << pRoot->_val << " ";
		}

		if (NULL != pRoot->pRight)
			_PreOrder(pRoot->pRight);
	}

	void Threading(Node<T> * &pRoot, Node<T> * &pPre)
	{
		if (NULL == pRoot)
			return;
		
		if (NULL == pRoot->pLeft)
		{
			pRoot->left = THREAD;
			pRoot->pLeft = pPre;
		}
		if (NULL != pPre && NULL == pPre->pRight)
		{
			pPre->right = THREAD;
			pPre->pRight = pRoot;
		}
		
		pPre = pRoot;
		if (NULL != pRoot->pLeft && LINK == pRoot->left)
			Threading(pRoot->pLeft, pPre);
		if (NULL != pRoot->pRight && LINK == pRoot->right)
			Threading(pRoot->pRight, pPre);
	}

	void Create(Node<T> * &pRoot, char arr[], int size, int &index)
	{
		if (index < size && arr[index] != '#')
		{
			pRoot = new Node<T>(arr[index]);
			Create(pRoot->pLeft, arr, size, ++index);
			Create(pRoot->pRight, arr, size, ++index);
		}
	}

private:
	Node<T> *_pRoot;
};

int main()
{
	char arr[] = "013##4##25";
	BinaryThreadTree<char> b(arr, sizeof(arr)/sizeof(arr[0]) - 1);
	
	b.PreOrder();
	std::cout << std::endl;

	return 0;
}
