#ifndef FORWARDLINKEDLIST_H
#define FORWARDLINKEDLIST_H

#include <stdexcept>
#include "Ilist.h"

template<class T>
class ForwardLinkedList :public IList<T>
{
public:
	ForwardLinkedList() = default;
	ForwardLinkedList(const T *list, int count);
	ForwardLinkedList(const ForwardLinkedList<T> &);
	ForwardLinkedList(ForwardLinkedList<T> &&);
	ForwardLinkedList(const IList<T> &);

	/*高效*/
	virtual int Size()const;

	/*高效*/
	virtual void PushBack(const T &value);

	/*高效*/
	virtual void PushFront(const T &value);

	virtual void Insert(int index, const T &value);

	virtual void InsertRange(int begin, const T *list, int count);
	virtual void InsertRange(int begin, const IList<T> &list);

	virtual void Replace(const T *list, int count);
	virtual void Replace(const IList<T> &list);

	virtual T *Find(std::function<bool(const T&)> Test);
	virtual const T *Find(std::function<bool(const T&)> Test)const;

	virtual const T& GetTop()const;
	virtual T& GetTop();

	virtual const T& GetLast()const;
	virtual T& GetLast();

	virtual const T &At(int index)const;
	virtual const T &operator[](int index)const;

	virtual T &At(int index);
	virtual T &operator[](int index);

	virtual bool Remove(std::function<bool(const T&)> Test);

	/*高效*/
	virtual void RemoveFirst();

	virtual void RemoveLast();

	virtual void RemoveAt(int index);

	virtual void Clear();

	/*快速*/
	virtual bool IsEmpty()const;

	virtual bool Exist(std::function<bool(const T&)> Test)const;

	virtual void Foreach(std::function<void(T &)> Func);

	virtual void Foreach(std::function<void(const T &)> Func)const;

private:
	struct Node
	{
		T value;
		Node *next = nullptr;
	};

	int m_size = 0;
	Node *m_head = nullptr, *m_last = nullptr;

	Node *MallocNode(const T &value)
	{
		Node *node = new Node{ value };
		return node;
	}
};

template<class T>
inline ForwardLinkedList<T>::ForwardLinkedList(const T * list, int count)
{
	Replace(list, count);
}

template<class T>
inline ForwardLinkedList<T>::ForwardLinkedList(const ForwardLinkedList<T> &list)
{
	Replace(list);
}

template<class T>
inline ForwardLinkedList<T>::ForwardLinkedList(ForwardLinkedList<T> &&list)
{
	m_size = list.m_size;
	m_head = list.m_head;
	m_last = list.m_last;
}

template<class T>
inline ForwardLinkedList<T>::ForwardLinkedList(const IList<T> &list)
{
	Replace(list);
}

template<class T>
inline int ForwardLinkedList<T>::Size() const
{
	return m_size;
}

template<class T>
inline void ForwardLinkedList<T>::PushBack(const T & value)
{
	if (m_head == nullptr)
		m_head = m_last = MallocNode(value);
	else
	{
		m_last->next = MallocNode(value);
		m_last = m_last->next;
	}
	++m_size;
}

template<class T>
inline void ForwardLinkedList<T>::PushFront(const T & value)
{
	if (m_head == nullptr)
		m_head = m_last = MallocNode(value);
	else
	{
		Node *node= MallocNode(value);
		node->next = m_head;
		m_head = node;
	}
	++m_size;
}

template<class T>
inline void ForwardLinkedList<T>::Insert(int index, const T & value)
{
	if (index >= m_size)
		throw std::out_of_range("Linked List:In Insert variable[index] was out of range!");
	if (index == 0)
		PushFront(value);
	else
	{
		Node *parNode = m_head;
		Node *node = MallocNode(value);
		for (int i = 1; i < index; ++i)
			parNode = parNode->next;
		node->next = parNode->next;
		parNode->next = node;
	}
	++m_size;
}

template<class T>
inline void ForwardLinkedList<T>::InsertRange(int begin, const T *list, int count)
{
	if (count == 0)
		return;
	if (begin >= m_size)
		throw std::out_of_range("Linked List:In InsertRange variable[begin] was out of range!");

	if(begin==0)
	{
		//构造一条新链表，把原来的接到后面
		//暂存数据
		Node *head = m_head, *last = m_last;
		int srcSize = m_size;

		m_head = m_last = nullptr;
		m_size = 0;

		for (int i = 0; i < count; ++i)
			PushBack(list[i]);
		m_last->next = head;
		m_last = last;
		m_size += srcSize;
		return;
	}

	Node *last = m_last;
	Node *parNode = m_head;
	for (int i = 1; i < begin; ++i)
		parNode = parNode->next;
	Node *srcNode = parNode->next;
	m_last = parNode;

	for (int i = 0; i < count; ++i)
		PushBack(list[i]);
	m_last->next = srcNode;
	m_last = last;
}

template<class T>
inline void ForwardLinkedList<T>::InsertRange(int begin, const IList<T>& list)
{
	if (list.Size() == 0)
		return;
	if (begin >= m_size)
		throw std::out_of_range("Linked List:In InsertRange variable[begin] was out of range!");

	if(begin==0)
	{
		//构造一条新链表，把原来的接到后面
		//暂存数据
		Node *head = m_head, *last = m_last;
		int srcSize = m_size;

		m_head = m_last = nullptr;
		m_size = 0;

		list.Foreach([&](const T &value)
		{
			PushBack(value);
		});
		m_last->next = m_head;
		m_last = last;
		m_size+=srcSize;
		return;
	}

	Node *last = m_last;
	Node *parNode = m_head;
	for (int i = 1; i < begin; ++i)
		parNode = parNode->next;
	Node *srcNode = parNode->next;
	m_last = parNode;

	list.Foreach([&](const T &value)
	{
		PushBack(value);
	});
	m_last->next = srcNode;
	m_last = last;
}

template<class T>
inline void ForwardLinkedList<T>::Replace(const T * list, int count)
{
	Clear();
	for (int i = 0; i < count; ++i)
		PushBack(list[i]);
	m_size = count;
}

template<class T>
inline void ForwardLinkedList<T>::Replace(const IList<T>& list)
{
	Clear();
	list.Foreach([&](const T &value)
	{
		PushBack(value);
	});
	m_size = list.Size();
}

template<class T>
inline T *ForwardLinkedList<T>::Find(std::function<bool(const T&)> Test)
{
	for (Node *node = m_head; node != nullptr; node = node->next)
		if (Test(node->value))
			return &node->value;
	return nullptr;
}

template<class T>
inline const T * ForwardLinkedList<T>::Find(std::function<bool(const T&)> Test) const
{
	for (Node *node = m_head; node != nullptr; node = node->next)
		if (Test(node->value))
			return &node->value;
	return nullptr;
}

template<class T>
inline const T & ForwardLinkedList<T>::GetTop() const
{
	if (m_size <= 0)
		throw std::out_of_range("Linked List:GetTop was out of range!");
	return m_head->value;
}

template<class T>
inline T & ForwardLinkedList<T>::GetTop()
{
	if (m_size <= 0)
		throw std::out_of_range("Linked List:GetTop was out of range!");
	return m_head->value;
}

template<class T>
inline const T & ForwardLinkedList<T>::GetLast() const
{
	if (m_size <= 0)
		throw std::out_of_range("Linked List:GetLast was out of range!");
	return m_last->value;
}

template<class T>
inline T & ForwardLinkedList<T>::GetLast()
{
	if (m_size <= 0)
		throw std::out_of_range("Linked List:GetLast was out of range!");
	return m_last->value;
}

template<class T>
inline const T & ForwardLinkedList<T>::At(int index) const
{
	if(index>=m_size)
		throw std::out_of_range("Linked List:At variable[index] was out of range!");
	Node *node = m_head;
	for (int i = 0; i < index; ++i)
		node = node->next;
	return node->value;
}

template<class T>
inline const T & ForwardLinkedList<T>::operator[](int index) const
{
	if (index >= m_size)
		throw std::out_of_range("Linked List:operator[] variable[index] was out of range!");
	Node *node = m_head;
	for (int i = 0; i < index; ++i)
		node = node->next;
	return node->value;
}

template<class T>
inline T & ForwardLinkedList<T>::At(int index)
{
	if (index >= m_size)
		throw std::out_of_range("Linked List:At variable[index] was out of range!");
	Node *node = m_head;
	for (int i = 0; i < index; ++i)
		node = node->next;
	return node->value;
}

template<class T>
inline T & ForwardLinkedList<T>::operator[](int index)
{
	if (index >= m_size)
		throw std::out_of_range("Linked List:operator[] variable[index] was out of range!");
	Node *node = m_head;
	for (int i = 0; i < index; ++i)
		node = node->next;
	return node->value;
}

template<class T>
inline bool ForwardLinkedList<T>::Remove(std::function<bool(const T&)> Test)
{
	if (m_size <= 0)
		return false;
	if (Test(m_head->value))
	{
		RemoveFirst();
		return true;
	}
	
	for (Node *parNode = m_head; parNode->next != nullptr; parNode = parNode->next)
	{
		if (Test(parNode->next->value))
		{
			Node *del = parNode->next;
			parNode->next = del->next;
			if (del == m_last)
				m_last = parNode;
			delete del;
			--m_size;
			return true;
		}
	}
	return false;
}

template<class T>
inline void ForwardLinkedList<T>::RemoveFirst()
{
	if (m_size == 1)
	{
		delete m_head;
		m_head = m_last = nullptr;
		--m_size;
		return;
	}
	Node *node = m_head->next;
	delete m_head;
	m_head = node;
	--m_size;
}

template<class T>
inline void ForwardLinkedList<T>::RemoveLast()
{
	if (m_size <= 0)
		return;
	if (m_size == 1)
	{
		delete m_head;
		m_head = m_last = nullptr;
		--m_size;
		return;
	}

	Node *parNode = m_head;
	for (; parNode->next != m_last; parNode = parNode->next);
	delete m_last;
	parNode->next = nullptr;
	m_last = parNode;
	--m_size;
}

template<class T>
inline void ForwardLinkedList<T>::RemoveAt(int index)
{
	if (index >= m_size)
		throw std::out_of_range("Linked List:At variable[index] was out of range!");
	if (index == 0)
	{
		RemoveFirst();
		return;
	}
	else if (index == m_size - 1)
	{
		RemoveLast();
		return;
	}
	Node *parNode = m_head;
	for (int i = 1; i < index; ++i)
		parNode = parNode->next;
	Node *del = parNode->next;
	parNode->next = del->next;
	delete del;
	--m_size;
}

template<class T>
inline void ForwardLinkedList<T>::Clear()
{
	if (m_size != 0)
	{
		for (Node *del = m_head, *next = m_head->next; next != nullptr; del = next, next = next->next)
			delete del;
		delete m_last;
	}
	m_head = m_last = nullptr;
	m_size = 0;
}

template<class T>
inline bool ForwardLinkedList<T>::IsEmpty() const
{
	return m_size == 0;
}

template<class T>
inline bool ForwardLinkedList<T>::Exist(std::function<bool(const T&)> Test) const
{
	for (Node *node = m_head; node != nullptr; node = node->next)
		if (Test(node->value))
			return true;
	return false;
}

template<class T>
inline void ForwardLinkedList<T>::Foreach(std::function<void(T&)> Func)
{
	for (Node *node = m_head; node != nullptr; node = node->next)
		Func(node->value);
}

template<class T>
inline void ForwardLinkedList<T>::Foreach(std::function<void(const T&)> Func)const
{
	for (Node *node = m_head; node != nullptr; node = node->next)
		Func(node->value);
}

#endif
