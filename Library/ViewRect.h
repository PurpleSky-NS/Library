#ifndef VIEWRECT_H
#define VIEWRECT_H

#include <memory>
#include <string>

struct Rect
{
	int x, y, w, h;

	bool IsCollisionWith(const Rect &rect)const
	{
		return x < rect.x + rect.w &&
			x + w > rect.x &&
			y < rect.y + rect.h &&
			h + y > rect.y;
	}

};

/*�ַ���ͼ����[һ���������]*/
class ViewRect
{
public:
	ViewRect() = default;
	~ViewRect();

	/*��ָ����߹���һ��ViewRect,����ch���*/
	void SetViewRect(int w, int h, char ch = ' ');
	/*������һ��ViewRect*/
	void SetViewRect(const ViewRect &src);
	/*������һ��ViewRect��ĳһ������*/
	void SetViewRect(const ViewRect &src, const Rect &cut);
	/*�ƶ�ViewRect��Դ����,���ԭ�����ڴ���ͷ�Ȩ*/
	void SetViewRect(ViewRect &&src);

	/*ͨ��view(x,y)���̻�ȡ��λ�ö�Ӧ�ַ�������,�±��0��ʼ*/
	inline char &operator()(int x, int y);
	inline const char &operator()(int x, int y)const;

	/*�����ı������ı���С���ڿ�ȣ�����ʾʡ�Ժ�*/
	inline void SetText(int x, int y, const std::string &src);

	/*���һ��*/
	inline void FillLine(int xOffset, int y, char ch);
	inline void FillLine(int xOffset, int xLast, int y, char ch);

	/*��ȡ��������*/
	inline const Rect &GetCutRect()const;

	/*��ȡ�ַ���ͼ����Դ����*/
	inline char *GetViewData()const;

	/*���ַ���ͼ��ch����*/
	inline void Reset(char ch = ' ');

	inline void Clear();

private:
	char *m_viewData = nullptr;
	bool m_isSrcData = true;
	Rect m_srcRect = {};
	Rect m_cutRect = {};
};

char &ViewRect::operator()(int x, int y)
{
	return m_viewData[(m_cutRect.y + y)*(m_srcRect.w + 1) + m_cutRect.x + x];
}

const char &ViewRect::operator()(int x, int y)const
{
	return m_viewData[(m_cutRect.y + y)*(m_srcRect.w + 1) + m_cutRect.x + x];
}

void ViewRect::SetText(int x, int y, const std::string &src)
{
	if (x >= m_cutRect.w || y >= m_cutRect.h)
		return;
	if (x + src.size() <= (size_t)m_cutRect.w)
		memcpy(&(*this)(x, y), src.data(), src.size());
	else
		memcpy(&(*this)(x, y), (src.substr(0, (m_cutRect.w - x - 3 > 0 ? m_cutRect.w - x - 3 : 0)) + "...").data(), m_cutRect.w);
}

inline void ViewRect::FillLine(int xOffset, int y, char ch)
{
	memset(&(*this)(xOffset, y), ch, m_cutRect.w - xOffset);
}

inline void ViewRect::FillLine(int xOffset, int xLast, int y, char ch)
{
	memset(&(*this)(xOffset, y), ch, m_cutRect.w - xOffset - xLast);
}

const Rect &ViewRect::GetCutRect() const
{
	return m_cutRect;
}

char *ViewRect::GetViewData() const
{
	return m_viewData;
}

void ViewRect::Reset(char ch)
{
	const int &w = GetCutRect().w;
	const int &h = GetCutRect().h;
	memset(m_viewData, ch, sizeof(char)*(w + 1)*h);
	//��任�м�������
	for (int i = 0; i < h; ++i)
		m_viewData[i*(w + 1) + w] = '\n';
	m_viewData[h*(w + 1) - 1] = 0;
}

void ViewRect::Clear()
{
	if (m_viewData != nullptr&&m_isSrcData)
		delete m_viewData;
	m_viewData = nullptr;
	m_isSrcData = false;
}
#endif