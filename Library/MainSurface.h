#ifndef MAINSURFACE_H
#define MAINSURFACE_H

#include "Value.h"
#include "UISurface.h"
#include "MainSurface_Title.h"
#include "MainSurface_Menu.h"
#include "MainSurface_Login.h"
#include "MainSurface_Register.h"

class MainSurface : public UISurface
{
public:
	class MainTips :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &view)override
		{
			TipsUI::OnCreate(view);
			CreateTips("操作提示(Tips)",
				{
					"①按下w和s可以上下切换",
					"②按下空格和Space可以选择当前项",
					"③按下q可以退出当前目录",
					"④以后的操作提示都会在这里！"
				});
		}
	};

	MainSurface() :
		UISurface(DEFAULT_FPS, CONSOLE_WIDTH, CONSOLE_HEIGHT)
	{}

	virtual void OnCreateUI()override
	{
		AddUIComponent(new MainSurface_Title());
		AddUIComponent(new MainSurface_Menu());
		AddUIComponent(new MainTips());
		AddUIComponent(new MainSurface_Login());
		AddUIComponent(new MainSurface_Login::Failed());
		AddUIComponent(new MainSurface_Register());
		AddUIComponent(new MainSurface_Menu::OperatorTips());
		AddUIComponent(new MainSurface_Register::Succeed());
		AddUIComponent(new MainSurface_Register::UserNameRepeated());
		AddUIComponent(new MainSurface_Register::PasswordRepeatedFailed());
		AddUIComponent(new MainSurface_Register::Empty());
	}

	virtual void OnCreate()override
	{
		GetUIComponent<MainSurface_Login>()->Disable();
		GetUIComponent<MainSurface_Register>()->Disable();
		SetFocusUI(GetUIComponent<MainSurface_Menu>());
	}

	virtual void OnEnter()override
	{
		UISurface::OnEnter();
		GetUIComponent<MainTips>()->Show();
	}
};
#endif