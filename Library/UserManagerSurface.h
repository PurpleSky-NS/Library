#ifndef USERMANAGERSURFACE_H
#define USERMANAGERSURFACE_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Value.h"
#include "UIComponent.h"
#include "UISurface.h"
#include "Title.h"
#include "MenuUI.h"
#include "TipsUI.h"
#include "EditWindow.h"
#include "UserManager.h"

class UserManageSurface : public UISurface
{
	class UserMenuList :public MenuUI
	{
	public:

		void Refresh()
		{
			std::vector<MenuItem> userItems;
			UserManager::GetInstance()->ForeachUsers([&](IUser *user)
			{
				MenuItem item;
				std::ostringstream oss;
				oss.setf(std::ios::left);
				oss << "用户名:" << std::setw(20) << user->GetUserName() << "密码:" << std::setw(20) << user->GetPassWord() << "管理员:" << (user->IsAdministrator() ? "是" : "否");
				item.name = oss.str();
				item.data = user;
				item.Event = [&](void *data)
				{
					GetContext()->GetUIComponent<AdminMenu>()-> Show((IUser*)data);
				};
				userItems.push_back(std::move(item));
			});
			CreateMenu(userItems, 40, 2, 0, CONSOLE_HEIGHT - 3, GravityMode::Left, true);
		}
		
		virtual void OnReset()override
		{
			Refresh();
		}

	};

	class SetUserWindow :public EditWindow
	{

		IUser *m_user;
	public:
		class Succeed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("修改成功", { "用户信息修改成功！" });
			}
		};

		void Show(IUser *msg)
		{
			m_user = msg;
			EditWindow::Show();
		}
	private:
		void OnCreate(const ViewRect &view)
		{
			EditWindow::OnCreate(view);
			CreateEditor(40, 5,
				{
					{"密码","重置密码",true,false,[](char input) {return true; }}
				});
		}

		virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
		{
			m_user->SetPassword(msgs[0].resualt);
			UserManager::GetInstance()->SaveUserData();
			GetContext()->GetUIComponent<Succeed>()->Show();
			EditWindow::Reset();
			GetContext()->GetUIComponent<UserMenuList>()->Refresh();
			return true;
		}
	};

	class AdminMenu :public MenuUI
	{
		IUser *m_user;
	public:
		class NotUser :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("查阅失败", { "该用户为管理员！" });
			}
		};
		void Show(IUser *user)
		{
			Enable();
			GetContext()->SetFocusUI(this);
			m_user = user;
		}
	protected:

		virtual void OnCreate(const ViewRect &srcView)override
		{
			MenuUI::OnCreate(srcView);
			CreateMenu(
				{
					{"修改用户信息",[&](void *) {GetContext()->GetUIComponent<SetUserWindow>()->Show(m_user); } },
					{"用户借阅记录",[&](void *) 
						{
							if (m_user->IsAdministrator())
								GetContext()->GetUIComponent<NotUser>()->Show();
							else
								SurfaceManager::GetInstance()->StartSurface("BorrowMessageSurface",(User*)m_user); 
						}}
				}, 2, 8, 1, GravityMode::Left);
			Disable();
		}

		virtual void OnDisable()override
		{
			MenuUI::OnDisable();
			GetContext()->SetFocusUI(GetContext()->GetUIComponent<UserMenuList>());
		}

		virtual bool OnEvent(char input)override
		{
			switch (input)
			{
			case 'q':
				Disable();
				GetContext()->Draw();
				break;
			default:
				MenuUI::OnEvent(input);
				break;
			}
			return false;
		}
	};

public:
	UserManageSurface() :
		UISurface(DEFAULT_FPS, CONSOLE_WIDTH, CONSOLE_HEIGHT)
	{}

	virtual void OnCreateUI()override
	{
		AddUIComponent(new ManagerSurfaceTitle("管理用户信息"));
		AddUIComponent(new AdminMenu());
		AddUIComponent(new SetUserWindow());
		AddUIComponent(new UserMenuList());
		AddUIComponent(new AdminMenu::NotUser());
		AddUIComponent(new SetUserWindow::Succeed());
	}

	virtual void OnCreate()override
	{
		SetFocusUI(GetUIComponent<UserMenuList>());
	}

};
#endif