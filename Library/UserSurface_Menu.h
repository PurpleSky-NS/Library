#ifndef USERSURFACE_MENU_H
#define USERSURFACE_MENU_H

#include "Value.h"
#include "MenuUI.h"

class UserSurface_Menu :public MenuUI
{
	virtual void OnCreate(const ViewRect &srcView)override
	{
		MenuUI::OnCreate(srcView);
		CreateMenu(
			{
				{"查看图书",[&](void *) {SurfaceManager::GetInstance()->StartSurface("BookLookSurface"); }},
				{"借阅记录",[&](void *) {SurfaceManager::GetInstance()->StartSurface("BorrowMessageSurface",UserManager::GetInstance()->GetLoginUser()); }}
			}, 46, 9, 2, GravityMode::Left);
	}
};
#endif