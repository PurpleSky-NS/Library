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
			CreateTips("������ʾ(Tips)",
				{
					"�ٰ���Tab�����л���һ������",
					"�ڰ��»س������������",
					"�۰���\'<\'�����˳��༭����"
				});
		}
	};

	virtual void OnCreate(const ViewRect &srcView)override
	{
		MenuUI::OnCreate(srcView);
		CreateMenu(
			{
				{"��½",[&](void *) {m_loginCom->Show(); m_opTips->Show(); }},
				{"ע��",[&](void *) {m_registerCom->Show(); m_opTips->Show(); }},
				{"�˳�",[](void *) {SurfaceManager::GetInstance()->DestroyActiveSurface(); }}
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