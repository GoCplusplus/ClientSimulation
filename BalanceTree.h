#ifndef CBALANCETREE__H
#define CBALANCETREE__H

// ƽ�������ģ��
#include <functional>

template<class T>
struct TreeNode
{
	T data;
	int nHigh; // ���ĸ߶�
	struct TreeNode* lTree;
	struct TreeNode* rTree;
	TreeNode()
	{
		nHigh = 0; // ��ʼ�߶�Ϊ0
		lTree = nullptr;
		rTree = nullptr;
	}
};


template<class T>
class CBalanceTree
{
public:
	CBalanceTree();
	~CBalanceTree();

public:
	void InsertElement(T data);
	bool DeleteElement(T data);
	bool FindElement(T& data); // �����Ƿ����
	void DoSomthing(std::function<void(T& t)>);
	bool Empty(); // �Ƿ��޽ڵ�
	void Clear(); // ɾ������
private:
	void InsertElement(T data, struct TreeNode<T>*& pNode);
	void SigleLeftLeft(struct TreeNode<T>*& pNode); // ������� ����ת
	void DoubleLeftRight(struct TreeNode<T>*& pNode); // ������� ˫��ת
	void SigleRightRight(struct TreeNode<T>*& pNode); // ������� ����ת
	void DoubleRightLeft(struct TreeNode<T>*& pNode); // ������� ˫��ת
	bool DeleteElement(T data, struct TreeNode<T>*& pNode);
	bool FindElement(T& data, struct TreeNode<T>* pNode);
	void DoSomthing(std::function<void(T&t)>& func, struct TreeNode<T>* pNode);
	void Delete(struct TreeNode<T>*& pNode);
	inline int  GetHigh(TreeNode<T>* pNode);
	inline int  Max(int a, int b);
private:
	struct TreeNode<T>* m_pRoot;
};

template<class T>
int CBalanceTree<T>::GetHigh(struct TreeNode<T>* pNode)
{
	if (!pNode)
		return -1;
	else
		return pNode->nHigh;
}

template<class T>
int CBalanceTree<T>::Max(int a, int b)
{
	return (a > b) ? a : b;
}

template<class T>
CBalanceTree<T>::CBalanceTree()
{
	m_pRoot = nullptr;
}

template<class T>
CBalanceTree<T>::~CBalanceTree()
{
	Delete(m_pRoot);
}

template<class T>
void CBalanceTree<T>::SigleLeftLeft(struct TreeNode<T>*& pNode)
{
	struct TreeNode<T>* pK2 = pNode->lTree;
	pNode->lTree = pK2->rTree;
	pNode->nHigh = Max(GetHigh(pNode->lTree), GetHigh(pNode->rTree)) + 1;
	pK2->rTree = pNode;
	pK2->nHigh = Max(GetHigh(pK2->lTree), GetHigh(pK2->rTree)) + 1;
	pNode = pK2;
}

template<class T>
void CBalanceTree<T>::SigleRightRight(struct TreeNode<T>*& pNode)
{
	struct TreeNode<T>* pK2 = pNode->rTree;
	pNode->rTree = pK2->lTree;
	pNode->nHigh = Max(GetHigh(pNode->lTree), GetHigh(pNode->rTree)) + 1;
	pK2->lTree = pNode;
	pK2->nHigh = Max(GetHigh(pK2->lTree), GetHigh(pK2->rTree)) + 1;
	pNode = pK2;
}

template<class T>
void CBalanceTree<T>::DoubleLeftRight(struct TreeNode<T>*& pNode)
{
	SigleRightRight(pNode->lTree);
	SigleLeftLeft(pNode);
}

template<class T>
void CBalanceTree<T>::DoubleRightLeft(struct TreeNode<T>*& pNode)
{
	SigleLeftLeft(pNode->rTree);
	SigleRightRight(pNode);
}

template<class T>
void CBalanceTree<T>::InsertElement(T data)
{
	InsertElement(data, m_pRoot);
}

template<class T>
void CBalanceTree<T>::InsertElement(T data, struct TreeNode<T>*& pNode)
{
	if (!pNode)
	{
		pNode = new struct TreeNode<T>;
		pNode->data = data;
		return;
	}
	if (data < pNode->data)
	{
		InsertElement(data, pNode->lTree);
		if ((GetHigh(pNode->lTree) - GetHigh(pNode->rTree)) == 2) // �߶Ȳ�2 ��ƽ��
		{
			if (GetHigh(pNode->lTree->lTree) > GetHigh(pNode->lTree->rTree)) // ������� ����ת
				SigleLeftLeft(pNode);
			else
				DoubleLeftRight(pNode); // ������� ˫��ת

		}
	}
	else
	{
		InsertElement(data, pNode->rTree);
		if ((GetHigh(pNode->rTree) - GetHigh(pNode->lTree)) == 2)
		{
			if (GetHigh(pNode->rTree->lTree) > GetHigh(pNode->rTree->rTree)) // ������� ˫��ת
				DoubleRightLeft(pNode);
			else
				SigleRightRight(pNode); // ������� ����ת
		}
	}
	pNode->nHigh = Max(GetHigh(pNode->lTree), GetHigh(pNode->rTree)) + 1;
}

template<class T>
bool CBalanceTree<T>::DeleteElement(T data)
{
	return DeleteElement(data, m_pRoot);
}

template<class T>
bool CBalanceTree<T>::DeleteElement(T data, struct TreeNode<T>*& pNode)
{
	if (!pNode)
		return false;
	if (data < pNode->data)
	{
		DeleteElement(data, pNode->lTree);
		if ((GetHigh(pNode->rTree) - GetHigh(pNode->lTree)) == 2) // ɾ����������ƽ�����
		{
			if (GetHigh(pNode->lTree->lTree) > GetHigh(pNode->lTree->rTree))
				SigleLeftLeft(pNode);
			else
				DoubleLeftRight(pNode);
		}
	}
	else if (data > pNode->data)
	{
		DeleteElement(data, pNode->rTree);
		if ((GetHigh(pNode->lTree) - GetHigh(pNode->rTree)) == 2) // ɾ����������ƽ�����
		{
			if (GetHigh(pNode->rTree->lTree) > GetHigh(pNode->lTree->rTree))
				DoubleRightLeft(pNode);
			else
				SigleRightRight(pNode);
		}
	}
	else // ɾ���ýڵ�
	{
		if (pNode->lTree && pNode->rTree) // ����������������
		{
			// �ҵ�������������ڵ�
			struct TreeNode<T>* pNodeFind = pNode->rTree;
			while (pNodeFind->lTree)
				pNodeFind = pNodeFind->lTree;
			pNode->data = pNodeFind->data;
			DeleteElement(pNode->data, pNode->rTree);
			if ((GetHigh(pNode->lTree) - GetHigh(pNode->rTree)) == 2) // ����ƽ��
			{
				if (GetHigh(pNode->lTree->lTree) > GetHigh(pNode->lTree->rTree)) // ����
					SigleLeftLeft(pNode);
				else
					DoubleLeftRight(pNode); // ����
			}
		}
		else // ����һ���ڵ��û��
		{
			if (pNode->lTree)
			{
				pNode->data = pNode->lTree->data;
				delete pNode->lTree;
				pNode->lTree = nullptr;
			}
			else if (pNode->rTree)
			{
				pNode->data = pNode->rTree->data;
				delete pNode->rTree;
				pNode->rTree = nullptr;
			}
			else
			{
				delete pNode;
				pNode = nullptr;
				return true;
			}
		}
	}
	pNode->nHigh = Max(GetHigh(pNode->lTree), GetHigh(pNode->rTree)) + 1;
	return true;
}

template<class T>
bool CBalanceTree<T>::FindElement(T& data)
{
	return FindElement(data, m_pRoot);
}

template<class T>
void CBalanceTree<T>::DoSomthing(std::function<void(T&t)> func)
{
	DoSomthing(func, m_pRoot);
}

template<class T>
void CBalanceTree<T>::DoSomthing(std::function<void(T&t)>& func, struct TreeNode<T>* pNode)
{
	if (!pNode)
		return;
	func(pNode->data);
	DoSomthing(func, pNode->lTree);
	DoSomthing(func, pNode->rTree);
}

template<class T>
bool CBalanceTree<T>::FindElement(T& data, struct TreeNode<T>* pNode)
{
	if (!pNode)
		return false;
	if (data < pNode->data)
		return FindElement(data, pNode->lTree);
	else if (data > pNode->data)
		return FindElement(data, pNode->rTree);
	else // ��� ��ֵ
		data = pNode->data;
	return true;
}

template<class T>
void CBalanceTree<T>::Delete(struct TreeNode<T>*& pNode)
{
	if (!pNode)
		return;
	Delete(pNode->lTree);
	Delete(pNode->rTree);
	delete pNode;
	pNode = nullptr;
}

template<class T>
bool CBalanceTree<T>::Empty()
{
	return m_pRoot == nullptr;
}

template<class T>
void CBalanceTree<T>::Clear()
{
	Delete(m_pRoot);
}

#endif