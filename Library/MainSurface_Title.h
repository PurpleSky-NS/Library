#ifndef MAINSURFACE_TITLE_H
#define MAINSURFACE_TITLE_H

#include "UIComponent.h"
#include "Time.h"

class MainSurface_Title :public UIComponent
{
public:
	const int rectW = 60;
	const int rectH = 7;

	virtual void OnCreate(const	ViewRect &srcView)override
	{
		m_titleView.SetViewRect(srcView, { (CONSOLE_WIDTH - rectW) / 2,1,rectW,rectH });
		m_addrView.SetViewRect(srcView, { 0,CONSOLE_HEIGHT - 1,CONSOLE_WIDTH,1 });
	}

	virtual void OnDraw()override
	{
		std::string title = "ͼ��ݹ���ϵͳ";
		std::string addr = "PurpleUI��ܲ��͵�ַ:https://blog.csdn.net/qq_34106090/article/details/90942877";
		m_titleView.SetText(GetTextCenterLayoutBeginPos(title, rectW), 2, title);
		title = "Made With PurpleUI���";
		m_titleView.SetText(GetTextRightLayoutBeginPos(title, rectW) - 2, 4, title);
		title = "By �Ͽ�";
		m_titleView.SetText(GetTextRightLayoutBeginPos(title, rectW) - 2, 5, title);
		DrawFrame(m_titleView, '*');
		m_addrView.SetText(GetTextRightLayoutBeginPos(addr, CONSOLE_WIDTH), 0, addr);
	}

private:
	ViewRect m_titleView;
	ViewRect m_addrView;
};
#endif