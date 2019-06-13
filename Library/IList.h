#ifndef ILIST_H
#define ILIST_H

#include <functional>

/*线性数据结构的公共接口*/
template<class T>
class IList
{
public:
	/*实现该接口应该快速高效*/
	virtual int Size()const = 0;

	virtual void PushBack(const T &value) = 0;

	virtual void PushFront(const T &value) = 0;	

	virtual void Insert(int index, const T &value) = 0;

	virtual void InsertRange(int begin, const T *list, int count) = 0;
	virtual void InsertRange(int begin, const IList<T> &list) = 0;

	virtual void Replace(const T *list, int count) = 0;
	virtual void Replace(const IList<T> &list) = 0;

	virtual T *Find(std::function<bool(const T&)> Test) = 0;
	virtual const T *Find(std::function<bool(const T&)> Test)const = 0;

	virtual const T& GetTop()const = 0;
	virtual T& GetTop() = 0;

	virtual const T& GetLast()const = 0;
	virtual T& GetLast() = 0;

	virtual const T &At(int index)const = 0;
	virtual const T &operator[](int index)const = 0;

	virtual T &At(int index) = 0;
	virtual T &operator[](int index) = 0;

	virtual bool Remove(std::function<bool(const T&)> Test) = 0;

	virtual void RemoveFirst() = 0;

	virtual void RemoveLast() = 0;

	virtual void RemoveAt(int index) = 0;

	virtual void Clear() = 0;

	virtual bool IsEmpty()const = 0;

	virtual bool Exist(std::function<bool(const T&)> Test)const = 0;

	virtual void Foreach(std::function<void(T &)> Func) = 0;

	virtual void Foreach(std::function<void(const T &)> Func)const = 0;
};
#endif