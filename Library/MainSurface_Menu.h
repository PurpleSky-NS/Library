#ifndef MAINSURFACE_MENU_H
#define MAINSURFACE_MENU_H

#include "Value.h"
#include "MenuUI.h"
#include "MainSurface_Login.h"
#include "MainSurface_Register.h"

class MainSurface_Menu :public MenuUI
{
public:

	class OperatorTips:public TipsUI
	{
		virtual void OnCreate(const ViewRect &view)override
		{
			TipsUI::OnCreate(view);
			CreateTips("操作提示(Tips)",
				{
					"①按下Tab可以切换下一项内容",
					"②按下回车可以完成数据",
					"③按下\'<\'可以退出编辑窗口"
				});
		}
	};

	virtual void OnCreate(const ViewRect &srcView)override
	{
		MenuUI::OnCreate(srcView);
		CreateMenu(
			{
				{"登陆",[&](void *) {m_loginCom->Show(); m_opTips->Show(); }},
				{"注册",[&](void *) {m_registerCom->Show(); m_opTips->Show(); }},
				{"退出",[](void *) {SurfaceManager::GetInstance()->DestroyActiveSurface(); }}
			}, (CONSOLE_WIDTH - 16) / 2, 12, 1, GravityMode::Center);
		m_loginCom = GetContext()->GetUIComponent<MainSurface_Login>();
		m_registerCom = GetContext()->GetUIComponent<MainSurface_Register>();
		m_opTips = GetContext()->GetUIComponent<OperatorTips>();
	}

private:
	MainSurface_Login *m_loginCom;
	MainSurface_Register *m_registerCom;
	OperatorTips *m_opTips;
};
#endif