#ifndef MAINSURFACE_LOGIN_H
#define MAINSURFACE_LOGIN_H

#include "TipsUI.h"
#include "EditWindow.h"
#include "UserManager.h"
#include "UserSurface.h"

class MainSurface_Login :public EditWindow
{
public:
	class Failed :public TipsUI
	{
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("��½ʧ��", { "�û��������������" });
		}
	};
protected:

	void OnCreate(const ViewRect &view)
	{
		EditWindow::OnCreate(view);
		CreateEditor(42, 9,
			{
				{"�û���","ֻ��ΪӢ��,����,�»���",true,true,[](char input) {return input>=0&&(isalnum(input) || input == '_'); }},
				{"����","ֻ��ΪӢ��,����,�»���",true,false,[](char input) {return input >= 0 && isalnum(input) || input == '_'; }}
			});
	}

	virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
	{
		IUser *user = UserManager::GetInstance()->Login(msgs[0].resualt, msgs[1].resualt);
		if (user == nullptr)
		{
			GetContext()->GetUIComponent<Failed>()->Show();
			return false;
		}
		if (user->IsAdministrator())
			SurfaceManager::GetInstance()->StartSurface("AdminSurface", user);
		else
			SurfaceManager::GetInstance()->StartSurface("UserSurface", user);
		return true;
	}
};
#endif