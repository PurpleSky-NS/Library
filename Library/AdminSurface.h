#ifndef ADMINSURFACE_H
#define ADMINSURFACE_H

#include "Value.h"
#include "UISurface.h"
#include "TipsUI.h"
#include "Administrator.h"
#include "AdminSurface_Menu.h"
#include "Title.h"

class AdminSurface : public UISurface
{
public:

	class Welcome :public TipsUI
	{
	public:
		void Show(const std::string &userName)
		{
			CreateTips("登陆成功", { "欢迎回来，管理员" + userName + "!" });
			TipsUI::Show();
		}
	};

	AdminSurface() :
		UISurface(DEFAULT_FPS, CONSOLE_WIDTH, CONSOLE_HEIGHT)
	{}

	virtual void OnCreateUI()override
	{
		AddUIComponent(new Welcome());
		AddUIComponent(new AdminSurface_Menu());
		AddUIComponent(new OperationSurface_Title());
	}

	virtual void OnCreate()override
	{
		SetFocusUI(GetUIComponent<AdminSurface_Menu>());
	}

	virtual void OnEnter(void *data)override
	{
		UISurface::OnEnter();
		GetUIComponent<Welcome>()->Show(((User*)data)->GetUserName());
	}
};
#endif