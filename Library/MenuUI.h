#ifndef MENUUI_H
#define MENUUI_H

#include <vector>
#include <functional>
#include "SurfaceManager.h"
#include "Timer.h"
#include "UIComponent.h"

class MenuUI :public UIComponent
{
protected:
	struct MenuItem
	{
		std::string name;
		std::function<void(void *)> Event;
		void *data;
	};

	enum GravityMode
	{
		Left,
		Center
	};
	
	void OnCreate(const ViewRect &view)override
	{
		m_animationTimer.SetUpadateTime(0.3f);
		m_animationTimer.AddEvent([&]()
		{
			m_animationStep = ++m_animationStep % 3;
			DrawChooseItem(m_choose, m_animationStep);
		});
		m_srcView.SetViewRect(view);
	}

	/*����һ���˵���ViewRect�����menuItemName����󳤶Ⱦ���
	@menuItems:�˵�����
	@x,y:�˵����Ͻǵĳ�ʼλ��
	@lineOffset:�˵�ÿ��֮��ľ���
	@pageCount:һҳ�����ʾ����Ԫ��[>0]
	@mode:�˵�Ԫ�صĶ����ʽ
	@drawBottomTips:�Ƿ������·����Ʋ˵���ʾ��,��׷��һ��*/
	void CreateMenu(const std::vector<MenuItem> &menuItems,
		int x, int y, int lineOffset, int pageCount, GravityMode mode, bool hasBottomTips)
	{
		m_menuItems = menuItems;
		m_lineOffset = lineOffset;
		m_pageCount = pageCount;
		m_pageNum = menuItems.size() / pageCount + 1;
		m_pageLine = (pageCount - 1)*(lineOffset + 1) + 1;
		m_mode = mode;
		m_hasBottomTips = hasBottomTips;
		m_choose = 0;
		int maxWidth = 0;
		for (auto i : m_menuItems)
			if (i.name.size() > (size_t)maxWidth)
				maxWidth = i.name.size();
		maxWidth += (mode == GravityMode::Left ? 4 : 10);//���ϼ�ͷ���
		if (hasBottomTips)
		{
			//��ʾ�����
			int tipWidth = std::string("��/ҳ ��/��").size() + 2 * std::to_string(m_menuItems.size()).size() + 2 * std::to_string(m_menuItems.size() / pageCount).size();
			if (tipWidth > maxWidth)
				maxWidth = tipWidth;
		}
		m_menuView.SetViewRect(m_srcView, { x,y, maxWidth,m_pageLine + hasBottomTips });
	}

	/*����һ���˵���ViewRect�����menuItemName����󳤶Ⱦ���
	@menuItems:�˵�����
	@x,y:�˵����Ͻǵĳ�ʼλ��
	@lineOffset:�˵�ÿ��֮��ľ���
	@mode:�˵�Ԫ�صĶ����ʽ*/
	void CreateMenu(const std::vector<MenuItem> &menuItems,
		int x, int y, int lineOffset, GravityMode mode)
	{
		m_menuItems = menuItems;
		m_lineOffset = lineOffset;
		m_pageCount = menuItems.size();
		m_pageNum = 1;
		m_pageLine = (menuItems.size() - 1)*(lineOffset + 1) + 1;
		m_mode = mode;
		m_hasBottomTips = false;
		m_choose = 0;
		int maxWidth = 0;
		for (auto i : m_menuItems)
			if (i.name.size() > (size_t)maxWidth)
				maxWidth = i.name.size();
		maxWidth += (mode == GravityMode::Left ? 4 : 10);//���ϼ�ͷ���
		m_menuView.SetViewRect(m_srcView, { x,y, maxWidth,m_pageLine });
	}

	void UpChoose()
	{
		m_choose -= 1;
		if (m_choose < 0)
		{
			m_choose = m_menuItems.size() - 1;
			DrawPage(m_choose);
		}
		else if ((m_choose + 1) % m_pageCount == 0)//������һҳ��
			DrawPage(m_choose);
		else
		{
			DrawNormalItem(m_choose + 1);
			DrawChooseItem(m_choose, m_animationStep = 0);
		}
		if (m_hasBottomTips)
			DrawTips();
		m_animationTimer.Run();
	}

	void DownChoose()
	{
		m_choose += 1;
		if (m_choose == m_menuItems.size())
		{
			m_choose = 0;
			DrawPage(m_choose);
		}
		else if (m_choose%m_pageCount == 0)
			DrawPage(m_choose);
		else
		{
			DrawNormalItem(m_choose - 1);
			DrawChooseItem(m_choose, m_animationStep = 0);
		}
		if (m_hasBottomTips)
			DrawTips();
		m_animationTimer.Run();
	}

	void UpPage()
	{
		m_choose -= m_pageCount;
		m_choose = (m_choose < 0 ? m_choose + m_pageCount : m_choose);
		DrawPage(m_choose);
		if (m_hasBottomTips)
			DrawTips();
		m_animationTimer.Run();
	}

	void DownPage()
	{
		m_choose += m_pageCount;
		m_choose = (m_choose >= (int)m_menuItems.size() ? m_choose % m_pageCount : m_choose);
		DrawPage(m_choose);
		if (m_hasBottomTips)
			DrawTips();
		m_animationTimer.Run();
	}

	virtual void OnDraw()override
	{
		if (!m_menuItems.empty())
			DrawPage(m_choose);
	}
	
	virtual void OnUpdate()override
	{
		if (IsFocus() && !m_menuItems.empty())
		{
			m_animationTimer.Update();
			DrawChooseItem(m_choose, m_animationStep);
			if (m_hasBottomTips)
				DrawTips();
		}
	}

	virtual void OnLoseFocus()override
	{
		m_animationTimer.Pause();
	}

	virtual void OnGetFocus()override
	{
		m_animationTimer.Resume();
	}

	virtual void OnReset()override
	{
		m_animationStep = 0;
		m_animationTimer.Run();
	}

	virtual void OnPause()override
	{
		m_animationTimer.Pause();
	}

	virtual void OnResume()override
	{
		m_animationTimer.Resume();
	}

	virtual void OnEnable()override
	{
		m_animationTimer.Resume();
	}

	virtual void OnDisable()override
	{
		m_animationTimer.Pause();
	}

	virtual bool OnEvent(char input)override
	{
		if (input == 'q')
		{
			SurfaceManager::GetInstance()->DestroyActiveSurface();
			return false;
		}
		if (m_menuItems.empty())
			return true;
		switch (input)
		{
		case 'w':
			UpChoose();
			break;
		case 's':
			DownChoose();
			break;
		case ' ':
		case '\r':
			m_menuItems[m_choose].Event(m_menuItems[m_choose].data);
			break;
		case 'a':
			if (m_pageCount > 1)
				UpPage();
			break;
		case 'd':
			if (m_pageCount > 1)
				DownPage();
			break;
		}	
		return false;
	}
	
private:

	ViewRect m_srcView, m_menuView;
	std::vector<MenuItem> m_menuItems;
	int m_lineOffset;//ÿ��ռ���
	int m_pageLine;//һҳ������
	int m_pageCount;//һҳ���ٸ�
	int m_pageNum;//����ҳ
	int m_choose = 0;
	Timer m_animationTimer;
	char m_animationStep = 0;
	GravityMode m_mode;
	bool m_hasBottomTips;
	bool m_isLoseFocus = false;

	void DrawChooseItem(int pos, int step)
	{
		int line = GetLineInPage(pos);
		m_menuView.FillLine(0, line, ' ');
		if (m_menuItems.empty())
			return;
		switch (m_mode)
		{
		case MenuUI::Left:
			m_menuView.SetText(0, line, ("->" + m_menuItems[pos].name + std::string(step, '.')));
			break;
		case MenuUI::Center:
			std::string space(3 - step, ' ');
			std::string chooseText = "->" + space + m_menuItems[pos].name + space + "<-";
			m_menuView.SetText(GetTextCenterLayoutBeginPos(chooseText, m_menuView.GetCutRect().w), line, chooseText);
			break;
		}
	}

	void DrawNormalItem(int pos)
	{
		int line = GetLineInPage(pos);
		m_menuView.FillLine(0, line, ' ');
		if (m_menuItems.empty())
			return;
		switch (m_mode)
		{
		case MenuUI::Left:
			m_menuView.SetText(0, line, m_menuItems[pos].name);
			break;
		case MenuUI::Center:
			m_menuView.SetText(GetTextCenterLayoutBeginPos(m_menuItems[pos].name, m_menuView.GetCutRect().w), line, m_menuItems[pos].name);
			break;
		}
	}

	/*ˢ��pos����ҳ*/
	void DrawPage(int pos)
	{
		for (int i = 0; i < m_menuView.GetCutRect().h; ++i)
			m_menuView.FillLine(0, i, ' ');
		int pagePos = GetFirstInPage(pos);
		int pageLast = GetLastInPage(pos);
		for (int line; pagePos <= pageLast; ++pagePos)
		{
			line = GetLineInPage(pagePos);
			if (pagePos == pos)
				DrawChooseItem(pagePos, m_animationStep);
			else
				DrawNormalItem(pagePos);
		}
		if (m_hasBottomTips)
			DrawTips();
	}

	void DrawTips()
	{
		char tips[32];
		sprintf(tips, "��%2d/%2d�� ��%2d/%2dҳ", m_choose + 1, m_menuItems.size(), m_choose / m_pageCount + 1, m_pageNum);
		m_menuView.SetText(GetTextRightLayoutBeginPos(tips, m_menuView.GetCutRect().w), m_pageLine, tips);
	}
	
	//��Ԫ������ҳ��Ԫ�ص������ڸ�ҳ����һ��
	int GetLineInPage(int pos)
	{
		return (pos % m_pageCount) * (m_lineOffset + 1);
	}

	//��Ԫ������ҳ��һ��Ԫ�ص�����
	int GetFirstInPage(int pos)
	{
		return (pos / m_pageCount)*m_pageCount;
	}

	//��Ԫ������ҳ���һ��Ԫ�ص�����
	int GetLastInPage(int pos)
	{
		if (pos / m_pageCount + 1 == m_pageNum)
			return m_menuItems.size() - 1;
		return (pos / m_pageCount + 1)*m_pageCount - 1;
	}
};
#endif