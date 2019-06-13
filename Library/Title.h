#ifndef TITLE_H
#define TITLE_H

#include "Value.h"
#include "UIComponent.h"
#include "Time.h"

/*��������ı���*/
class OperationSurface_Title :public UIComponent
{
public:

	virtual void OnCreate(const	ViewRect &srcView)override
	{
		m_view.SetViewRect(srcView, { 1,5,CONSOLE_WIDTH,1 });
	}

	virtual void OnDraw()override
	{
		m_view.SetText(GetTextCenterLayoutBeginPos(title, CONSOLE_WIDTH), 0, title);
	}

private:
	const std::string title = "������ʲô......";
	ViewRect m_view;
};
/*�������ı���*/
class ManagerSurfaceTitle :public UIComponent
{
	std::string m_title;
public:

	ManagerSurfaceTitle(const std::string &title)
	{
		m_title = title;
	}

	virtual void OnCreate(const	ViewRect &srcView)override
	{
		m_view.SetViewRect(srcView, { 0,0,CONSOLE_WIDTH,1 });
	}

	virtual void OnDraw()override
	{
		m_view.SetText(GetTextCenterLayoutBeginPos(m_title, CONSOLE_WIDTH), 0, m_title);
	}

private:
	ViewRect m_view;
};

#endif