#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <vector>
#include <algorithm>
#include <exception>

/*实现初始点的集合移动到目标点集合的移动变换功能的工具类
 并使每个点和总移动路程<尽量>小，计算出每个点的移动路径*/
class Translate
{
public:

	/*传入点位置不足，无法计算*/
	class SrcNumIsNotEnough :public std::runtime_error
	{
	public:SrcNumIsNotEnough(const char *msg) :runtime_error(msg) {}
	};

	/* 可以继承在其中添加标签但添加的字段过多
	   会导致内存占用较大，模板也尽量只写基础的数据类型(int，float) */
	template<class T>
	class IVector1
	{
	public:
		T x;
	};
	template<class T>
	class IVector2
	{
	public:
		T x, y;
	};
	template<class T>
	class IVector3
	{
	public:
		T x, y, z;
	};	

	template<class T>
	struct MoveLine
	{
		T from, to;
	};

	/* 开始执行计算，会改变初始和目标点集合 */
	/* 初始坐标集合，目标位置集合和补偿点位置集合
	   注：补偿点：当初始坐标集合数目不够时会在补偿点位置生成一个 */
	template<class T>
	static std::vector<MoveLine<IVector1<T>>> CalcTranslate(std::vector<IVector1<T>> &src,
		std::vector<IVector1<T>> &dst,
		std::vector<IVector1<T>> extra = {});

	template<class T>
	static std::vector<MoveLine<IVector2<T>>> CalcTranslate(std::vector<IVector2<T>> &src,
		std::vector<IVector2<T>> &dst,
		std::vector<IVector2<T>> extra = {});

	template<class T>
	static std::vector<MoveLine<IVector3<T>>> CalcTranslate(std::vector<IVector3<T>> &src,
		std::vector<IVector3<T>> &dst,
		std::vector<IVector3<T>> extra = {});

private:

	//向上取整
	inline static size_t Ceil(float t);

};
template<class T>
std::vector<Translate::MoveLine<Translate::IVector1<T>>> Translate::CalcTranslate(std::vector<Translate::IVector1<T>> &src,
	std::vector<Translate::IVector1<T>> &dst,
	std::vector<Translate::IVector1<T>> extra)
{
	//检查初始点的位置数量
	if (src.size() < dst.size() && extra.size() == 0)
		throw SrcNumIsNotEnough("The source position count is less than destination position count and there isn't any extra position!");
	//初始点的数量不足，使用补偿点
	for (size_t i = 0; src.size() < dst.size(); ++i)
		src.push_back(extra[i%extra.size()]);

	std::vector<MoveLine<IVector1<T>>> line(src.size());//编译器有优化，不用右值引用
	auto Cmp = [](const IVector1<T> &a, const IVector1<T> &b)
	{
		return a.x < b.x;
	};
	std::sort(src.begin(), src.end(), Cmp);//按照距离原点顺序排序
	std::sort(dst.begin(), dst.end(), Cmp);
	//使src对应dst
	for (size_t srcIdx = 0, dstIdx = 0, match = Ceil((float)src.size() / dst.size()); srcIdx < src.size(); ++dstIdx)
		for (size_t i = 0; i < match&&srcIdx < src.size(); ++i, ++srcIdx)
		{
			line[srcIdx].from = src[srcIdx];
			line[srcIdx].to = dst[dstIdx];
			if (match != 1 && src.size() - srcIdx == dst.size() - dstIdx)
				match = 1;
		}
	return line;
}

template<class T>
std::vector<Translate::MoveLine<Translate::IVector2<T>>> Translate::CalcTranslate(std::vector<Translate::IVector2<T>> &src,
	std::vector<Translate::IVector2<T>> &dst,
	std::vector<Translate::IVector2<T>> extra)
{
	//检查初始点的位置数量
	if (src.size() < dst.size() && extra.size() == 0)
		throw SrcNumIsNotEnough("The source position count is less than destination position count and there isn't any extra position!");
	//初始点的数量不足，使用补偿点
	for (size_t i = 0; src.size() < dst.size(); ++i)
		src.push_back(extra[i%extra.size()]);

	std::vector<MoveLine<IVector2<T>>> line(src.size());//编译器有优化，不用右值引用
	auto Cmp = [](const IVector2<T> &a, const IVector2<T> &b)
	{
		if (a.y == b.y)
			return a.x < b.x;
		return a.y < b.y;
	};
	sort(src.begin(), src.end(), Cmp);//按照距离原点顺序排序
	sort(dst.begin(), dst.end(), Cmp);
	//使src对应dst
	for (size_t srcIdx = 0, dstIdx = 0, match = Ceil((float)src.size() / dst.size()); srcIdx < src.size(); ++dstIdx)
		for (size_t i = 0; i < match&&srcIdx < src.size(); ++i, ++srcIdx)
		{
			line[srcIdx].from = src[srcIdx];
			line[srcIdx].to = dst[dstIdx];
			if (match != 1 && src.size() - srcIdx == dst.size() - dstIdx)
				match = 1;
		}
	return line;
}

template<class T>
std::vector<Translate::MoveLine<Translate::IVector3<T>>> Translate::CalcTranslate(std::vector<Translate::IVector3<T>> &src,
	std::vector<Translate::IVector3<T>> &dst,
	std::vector<Translate::IVector3<T>> extra)
{
	//检查初始点的位置数量
	if (src.size() < dst.size() && extra.size() == 0)
		throw SrcNumIsNotEnough("The source position count is less than destination position count and there isn't any extra position!");
	//初始点的数量不足，使用补偿点
	for (size_t i = 0; src.size() < dst.size(); ++i)
		src.push_back(extra[i%extra.size()]);

	std::vector<MoveLine<IVector3<T>>> line(src.size());//编译器有优化，不用右值引用
	auto Cmp = [](const IVector3<T> &a, const IVector3<T> &b)
	{
		if (a.z == b.z)
		{
			if (a.y == b.y)
				return a.x < b.x;
			return a.y < b.y;
		}
		return a.z < b.z;
	};
	sort(src.begin(), src.end(), Cmp);//按照距离原点顺序排序
	sort(dst.begin(), dst.end(), Cmp);
	//使src对应dst
	for (size_t srcIdx = 0, dstIdx = 0, match = Ceil((float)src.size() / dst.size()); srcIdx < src.size(); ++dstIdx)
		for (size_t i = 0; i < match&&srcIdx < src.size(); ++i, ++srcIdx)
		{
			line[srcIdx].from = src[srcIdx];
			line[srcIdx].to = dst[dstIdx];
			if (match != 1 && src.size() - srcIdx == dst.size() - dstIdx)
				match = 1;
		}
	return line;
}

size_t Translate::Ceil(float t)
{
	return (t > (size_t)t ? size_t(t + 1) : size_t(t));
}
#endif