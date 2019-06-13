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

	/*生成一个菜单，ViewRect宽度由menuItemName的最大长度决定
	@menuItems:菜单子项
	@x,y:菜单左上角的初始位置
	@lineOffset:菜单每行之间的距离
	@pageCount:一页最多显示多少元素[>0]
	@mode:菜单元素的对齐格式
	@drawBottomTips:是否在最下方绘制菜单提示栏,会追加一行*/
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
		maxWidth += (mode == GravityMode::Left ? 4 : 10);//加上箭头宽度
		if (hasBottomTips)
		{
			//提示栏宽度
			int tipWidth = std::string("第/页 第/个").size() + 2 * std::to_string(m_menuItems.size()).size() + 2 * std::to_string(m_menuItems.size() / pageCount).size();
			if (tipWidth > maxWidth)
				maxWidth = tipWidth;
		}
		m_menuView.SetViewRect(m_srcView, { x,y, maxWidth,m_pageLine + hasBottomTips });
	}

	/*生成一个菜单，ViewRect宽度由menuItemName的最大长度决定
	@menuItems:菜单子项
	@x,y:菜单左上角的初始位置
	@lineOffset:菜单每行之间的距离
	@mode:菜单元素的对齐格式*/
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
		maxWidth += (mode == GravityMode::Left ? 4 : 10);//加上箭头宽度
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
		else if ((m_choose + 1) % m_pageCount == 0)//翻到上一页了
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
	int m_lineOffset;//每项空几行
	int m_pageLine;//一页多少行
	int m_pageCount;//一页多少个
	int m_pageNum;//多少页
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

	/*刷新pos所在页*/
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
		sprintf(tips, "第%2d/%2d项 第%2d/%2d页", m_choose + 1, m_menuItems.size(), m_choose / m_pageCount + 1, m_pageNum);
		m_menuView.SetText(GetTextRightLayoutBeginPos(tips, m_menuView.GetCutRect().w), m_pageLine, tips);
	}
	
	//该元素所在页，元素的索引在该页的哪一行
	int GetLineInPage(int pos)
	{
		return (pos % m_pageCount) * (m_lineOffset + 1);
	}

	//该元素所在页第一个元素的索引
	int GetFirstInPage(int pos)
	{
		return (pos / m_pageCount)*m_pageCount;
	}

	//该元素所在页最后一个元素的索引
	int GetLastInPage(int pos)
	{
		if (pos / m_pageCount + 1 == m_pageNum)
			return m_menuItems.size() - 1;
		return (pos / m_pageCount + 1)*m_pageCount - 1;
	}
};
#endif