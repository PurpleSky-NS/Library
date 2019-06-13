#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <memory>
#include <stdexcept>
#include "Ilist.h"
/*ArrayList中为了效率，会将数组预留几个空位避免多次申请内存，Remove之后变量也不会真正的移除
所以，若是自定义类，应重载=运算符，并且完善复制构造机制,和vector一样预留2倍容量*/
template<class T>
class ArrayList :public IList<T>
{
public:
	ArrayList() = default;
	/*根据容量创建*/
	ArrayList(int capacity);
	/*填充count个value*/
	ArrayList(int count, const T &value);
	/*复制另一个IList*/
	ArrayList(const IList<T> &list);
	/*使用原生数组构造*/
	ArrayList(T *list, int size);//直接使用原数组数据
	ArrayList(const T *list, int size);//复制原数组数据
	/*复制另一个ArrayList*/
	ArrayList(const ArrayList<T> &list);
	/*移动另一个ArrayList*/
	ArrayList(ArrayList<T> &&list);

	~ArrayList();

	/*高效*/
	inline virtual int Size()const;

	/*高效*/
	inline int Capacity()const;

	/*将容量收束/扩张至capacity*/
	inline void ClampCapacityTo(int capacity);

	/*高效*/
	inline virtual void PushBack(const T &value);

	/*低效效*/
	virtual void PushFront(const T &value);

	virtual void Insert(int index, const T &value);

	virtual void InsertRange(int begin, const T *list, int count);
	virtual void InsertRange(int begin, const IList<T> &list);

	void Replace(const ArrayList<T> &list);
	void Replace(ArrayList<T> &&list);

	virtual void Replace(T *list, int count);//复制原数组数据
	virtual void Replace(const T *list, int count);//复制原数组数据
	virtual void Replace(const IList<T> &list);
	
	ArrayList<T> &operator=(const ArrayList<T> &list);
	ArrayList<T> &operator=(ArrayList<T> &&list);

	inline virtual T *Find(std::function<bool(const T&)> Test);
	inline virtual const T *Find(std::function<bool(const T&)> Test)const;

	inline virtual const T& GetTop()const;
	inline virtual T& GetTop();

	inline virtual const T& GetLast()const;
	inline virtual T& GetLast();

	inline virtual const T &At(int index)const;
	inline virtual const T &operator[](int index)const;

	inline virtual T &At(int index);
	inline virtual T &operator[](int index);

	virtual bool Remove(std::function<bool(const T&)> Test);

	virtual void RemoveFirst();

	/*高效*/
	inline virtual void RemoveLast();

	virtual void RemoveAt(int index);
	void RemoveRange(int from, int count);

	virtual bool IsEmpty()const;

	virtual void Clear();

	virtual bool Exist(std::function<bool(const T&)> Test)const;

	virtual void Foreach(std::function<void(T &)> Func);

	virtual void Foreach(std::function<void(const T &)> Func)const;

private:
	T *m_data = nullptr;
	int m_capacity = 0;
	int m_size = 0;

	void MallocAndMoveData(int capacity)
	{
		T *newData = new T[m_capacity = capacity];
		if (m_data != nullptr)
		{
			memcpy(newData, m_data, sizeof(T)*(m_size > m_capacity ? m_capacity : m_size));
			delete[] m_data;
		}
		m_data = newData;
	}

	void Realloc()
	{
		MallocAndMoveData((m_capacity <= 0 ? 1 : m_capacity) * 2);
	}

	/*将数组中的一段数据范围向后移动一个偏移量(要保证容量范围够大)*/
	void MoveRangeBack(int begin, int count, int offset)
	{
		if (begin + count > m_size)
			return;
		for (int end = begin + count - 1; end >= begin; --end)
			m_data[end + offset] = m_data[end];
	}

	/*将数组中的一段数据范围向前移动一个偏移量(要保证begin<=offset)*/
	void MoveRangeFront(int begin, int count, int offset)
	{
		if (begin < offset)
			return;
		for (int i = 0; i < count; ++i)
			m_data[begin + i - offset] = m_data[begin + i];
	}
};

template<class T>
ArrayList<T>::ArrayList(int capacity)
{
	MallocAndMoveData(capacity);
}

template<class T>
ArrayList<T>::ArrayList(int count, const T &value)
{
	MallocAndMoveData(m_size = count);
	for (int i = 0; i < count; ++i)
		m_data[i] = value;
}

template<class T>
ArrayList<T>::ArrayList(const IList<T>& list)
{
	Replace(list);
}

template<class T>
ArrayList<T>::ArrayList(T *list, int size)
{
	Replace(list, size);
}

template<class T>
ArrayList<T>::ArrayList(const T *list, int size)
{
	Replace(list, size);
}

template<class T>
ArrayList<T>::ArrayList(const ArrayList<T>& list)
{
	Replace(list);
}

template<class T>
ArrayList<T>::ArrayList(ArrayList<T>&& list)
{
	Replace(list);
}

template<class T>
ArrayList<T>::~ArrayList()
{
	Clear();
}

template<class T>
int ArrayList<T>::Size() const
{
	return m_size;
}

template<class T>
int ArrayList<T>::Capacity() const
{
	return m_capacity;
}

template<class T>
void ArrayList<T>::ClampCapacityTo(int capacity)
{
	MallocAndMoveData(capacity);
}

template<class T>
void ArrayList<T>::PushBack(const T & value)
{
	if (m_capacity == m_size)
		Realloc();
	m_data[m_size++] = value;
}

template<class T>
void ArrayList<T>::PushFront(const T & value)
{
	if (m_capacity == m_size)
		Realloc();
	MoveRangeBack(0, m_size, 1);
	m_data[0] = value;
	++m_size;
}

template<class T>
void ArrayList<T>::Insert(int index, const T & value)
{
	if (index >= m_size)
		throw std::out_of_range("Array List:In Insert variable[index] was out of range!");
	if (m_capacity == m_size)
		Realloc();
	MoveRangeBack(index, m_size - index, 1);
	m_data[index] = value;
	++m_size;
}

template<class T>
void ArrayList<T>::InsertRange(int begin, const T *list, int count)
{
	if (begin >= m_size)
		throw std::out_of_range("Array List:In InsertRange variable[begin] was out of range!");
	if (m_capacity < m_size + count)
		MallocAndMoveData(m_size + count);
	MoveRangeBack(begin, m_size - begin, count);
	int idx = 0;
	for(int i=0;i<count;++i)
		m_data[begin + idx++] = list[i];
	m_size += count;
}

template<class T>
void ArrayList<T>::InsertRange(int begin, const IList<T>& list)
{
	int otherSize = list.Size();
	if (begin >= m_size)
		throw std::out_of_range("Array List:In InsertRange variable[begin] was out of range!");
	if (m_capacity < m_size + otherSize)
		MallocAndMoveData(m_size + otherSize);
	MoveRangeBack(begin, m_size - begin, otherSize);
	int idx = 0;
	list.Foreach([&](const T &value)
	{
		m_data[begin + idx++] = value;
	});
	m_size+=otherSize;
}

template<class T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& list)
{
	Replace(list);
	return *this;
}

template<class T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& list)
{
	Replace(list);
	return *this;
}

template<class T>
inline T * ArrayList<T>::Find(std::function<bool(const T&)> Test)
{
	for (int i = 0; i < m_size; ++i)
		if (Test(m_data[i]))
			return &m_data[i];
	return nullptr;
}

template<class T>
inline const T * ArrayList<T>::Find(std::function<bool(const T&)> Test) const
{
	for (int i = 0; i < m_size; ++i)
		if (Test(m_data[i]))
			return &m_data[i];
	return nullptr;
}

template<class T>
void ArrayList<T>::Replace(const ArrayList<T>& list)
{
	Clear();
	m_size = list.m_size;
	MallocAndMoveData(m_size);
	memcpy(m_data, list.m_data, sizeof(T)*m_size);
}

template<class T>
void ArrayList<T>::Replace(ArrayList<T>&& list)
{
	Clear();
	m_data = list.m_data;
	m_capacity = list.m_capacity;
	m_size = list.m_size;
	list.m_data = nullptr;
	list.m_capacity = 0;
	list.m_size = 0;
}

template<class T>
void ArrayList<T>::Replace(T * list, int count)
{
	m_capacity = m_size = count;
	m_data = list;
}

template<class T>
void ArrayList<T>::Replace(const T * list, int count)
{
	m_capacity = m_size = count;
	MallocAndMoveData(count);
	memcpy(m_data, list, sizeof(T)*count);
}

template<class T>
void ArrayList<T>::Replace(const IList<T>& list)
{
	Clear();
	MallocAndMoveData(m_size = list.Size());
	int idx = 0;
	list.Foreach([&](const T &value)
	{
		m_data[idx++] = value;
	});
}

template<class T>
const T & ArrayList<T>::GetTop() const
{
	if(m_size<=0)
		throw std::out_of_range("Array List:GetTop was out of range!");
	return m_data[0];
}

template<class T>
T & ArrayList<T>::GetTop()
{
	if (m_size <= 0)
		throw std::out_of_range("Array List:GetTop was out of range!");
	return m_data[0];
}

template<class T>
const T & ArrayList<T>::GetLast() const
{
	if (m_size <= 0)
		throw std::out_of_range("Array List:GetLast was out of range!");
	return m_data[m_size - 1];
}

template<class T>
T & ArrayList<T>::GetLast()
{
	if (m_size <= 0)
		throw std::out_of_range("Array List:GetLast was out of range!");
	return m_data[m_size - 1];
}

template<class T>
const T & ArrayList<T>::At(int index) const
{
	if (index >= m_size)
		throw std::out_of_range("Array List:In At variable[index] was out of range!");
	return m_data[index];
}

template<class T>
const T & ArrayList<T>::operator[](int index) const
{
	if (index >= m_size)
		throw std::out_of_range("Array List:In operator[] variable[index] was out of range!");
	return m_data[index];
}

template<class T>
T & ArrayList<T>::At(int index)
{
	if (index >= m_size)
		throw std::out_of_range("Array List:In At variable[index] was out of range!");
	return m_data[index];
}

template<class T>
T & ArrayList<T>::operator[](int index)
{
	if (index >= m_size)
		throw std::out_of_range("Array List:In operator[] variable[index] was out of range!");
	return m_data[index];
}

template<class T>
void ArrayList<T>::RemoveFirst()
{
	if (m_size == 0)
		return;
	MoveRangeFront(1, m_size - 1, 1);
	--m_size;
}

template<class T>
void ArrayList<T>::RemoveLast()
{
	if (m_size > 0)
		--m_size;
}

template<class T>
void ArrayList<T>::RemoveAt(int index)
{
	if (index >= m_size)
		throw std::out_of_range("Array List:In RemoveAt variable[index] was out of range!");
	MoveRangeFront(index + 1, m_size - index - 1, 1);
	--m_size;
}

template<class T>
void ArrayList<T>::RemoveRange(int from, int count)
{
	if (from + count > m_size)
		throw std::out_of_range("Array List:In RemoveRange variable[from] was out of range!");
	MoveRangeFront(from + count, m_size - from - count, count);
	m_size -= count;
}

template<class T>
bool ArrayList<T>::Remove(std::function<bool(const T&)> Test)
{
	for (int i = 0; i < m_size; ++i)
		if (Test(m_data[i]))
		{
			RemoveAt(i);
			return true;
		}
	return false;
}

template<class T>
bool ArrayList<T>::IsEmpty() const
{
	return m_size == 0;
}

template<class T>
void ArrayList<T>::Clear()
{
	if (m_data != nullptr)
		delete[] m_data;
	m_data = nullptr;
	m_size = m_capacity = 0;
}

template<class T>
bool ArrayList<T>::Exist(std::function<bool(const T&)> Test)const
{
	for (int i = 0; i < m_size; ++i)
		if (Test(m_data[i]))
			return true;
	return false;
}

template<class T>
void ArrayList<T>::Foreach(std::function<void(T&)> Func)
{
	for (int i = 0; i < m_size; ++i)
		Func(m_data[i]);
}

template<class T>
void ArrayList<T>::Foreach(std::function<void(const T&)> Func)const
{
	for (int i = 0; i < m_size; ++i)
		Func(m_data[i]);
}
#endif