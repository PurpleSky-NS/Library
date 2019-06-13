#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <vector>
#include <algorithm>
#include <exception>

/*ʵ�ֳ�ʼ��ļ����ƶ���Ŀ��㼯�ϵ��ƶ��任���ܵĹ�����
 ��ʹÿ��������ƶ�·��<����>С�������ÿ������ƶ�·��*/
class Translate
{
public:

	/*�����λ�ò��㣬�޷�����*/
	class SrcNumIsNotEnough :public std::runtime_error
	{
	public:SrcNumIsNotEnough(const char *msg) :runtime_error(msg) {}
	};

	/* ���Լ̳���������ӱ�ǩ����ӵ��ֶι���
	   �ᵼ���ڴ�ռ�ýϴ�ģ��Ҳ����ֻд��������������(int��float) */
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

	/* ��ʼִ�м��㣬��ı��ʼ��Ŀ��㼯�� */
	/* ��ʼ���꼯�ϣ�Ŀ��λ�ü��ϺͲ�����λ�ü���
	   ע�������㣺����ʼ���꼯����Ŀ����ʱ���ڲ�����λ������һ�� */
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

	//����ȡ��
	inline static size_t Ceil(float t);

};
template<class T>
std::vector<Translate::MoveLine<Translate::IVector1<T>>> Translate::CalcTranslate(std::vector<Translate::IVector1<T>> &src,
	std::vector<Translate::IVector1<T>> &dst,
	std::vector<Translate::IVector1<T>> extra)
{
	//����ʼ���λ������
	if (src.size() < dst.size() && extra.size() == 0)
		throw SrcNumIsNotEnough("The source position count is less than destination position count and there isn't any extra position!");
	//��ʼ����������㣬ʹ�ò�����
	for (size_t i = 0; src.size() < dst.size(); ++i)
		src.push_back(extra[i%extra.size()]);

	std::vector<MoveLine<IVector1<T>>> line(src.size());//���������Ż���������ֵ����
	auto Cmp = [](const IVector1<T> &a, const IVector1<T> &b)
	{
		return a.x < b.x;
	};
	std::sort(src.begin(), src.end(), Cmp);//���վ���ԭ��˳������
	std::sort(dst.begin(), dst.end(), Cmp);
	//ʹsrc��Ӧdst
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
	//����ʼ���λ������
	if (src.size() < dst.size() && extra.size() == 0)
		throw SrcNumIsNotEnough("The source position count is less than destination position count and there isn't any extra position!");
	//��ʼ����������㣬ʹ�ò�����
	for (size_t i = 0; src.size() < dst.size(); ++i)
		src.push_back(extra[i%extra.size()]);

	std::vector<MoveLine<IVector2<T>>> line(src.size());//���������Ż���������ֵ����
	auto Cmp = [](const IVector2<T> &a, const IVector2<T> &b)
	{
		if (a.y == b.y)
			return a.x < b.x;
		return a.y < b.y;
	};
	sort(src.begin(), src.end(), Cmp);//���վ���ԭ��˳������
	sort(dst.begin(), dst.end(), Cmp);
	//ʹsrc��Ӧdst
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
	//����ʼ���λ������
	if (src.size() < dst.size() && extra.size() == 0)
		throw SrcNumIsNotEnough("The source position count is less than destination position count and there isn't any extra position!");
	//��ʼ����������㣬ʹ�ò�����
	for (size_t i = 0; src.size() < dst.size(); ++i)
		src.push_back(extra[i%extra.size()]);

	std::vector<MoveLine<IVector3<T>>> line(src.size());//���������Ż���������ֵ����
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
	sort(src.begin(), src.end(), Cmp);//���վ���ԭ��˳������
	sort(dst.begin(), dst.end(), Cmp);
	//ʹsrc��Ӧdst
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