#ifndef USERSURFACE_H
#define USERSURFACE_H

#include "Value.h"
#include "User.h"
#include "UISurface.h"
#include "Title.h"
#include "UserSurface_Menu.h"

class UserSurface : public UISurface
{
	class Welcome :public TipsUI
	{
	public:
		void Show(const std::string &userName)
		{
			CreateTips("��½�ɹ�", { "��ӭ�������û�" + userName + "!" });
			TipsUI::Show();
		}
	};
public:

	UserSurface():
		UISurface(DEFAULT_FPS, CONSOLE_WIDTH, CONSOLE_HEIGHT)
	{}

	virtual void OnCreateUI()override
	{
		AddUIComponent(new Welcome());
		AddUIComponent(new OperationSurface_Title());
		AddUIComponent(new UserSurface_Menu());
	}

	virtual void OnCreate()override
	{
		SetFocusUI(GetUIComponent<UserSurface_Menu>());
	}

	virtual void OnEnter(void *data)override
	{
		UISurface::OnEnter();
		GetUIComponent<Welcome>()->Show(((User*)data)->GetUserName());
	}
};
#endif