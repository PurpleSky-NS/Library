#ifndef ADMINSURFACE_MENU_H
#define ADMINSURFACE_MENU_H

#include "Value.h"
#include "MenuUI.h"

class AdminSurface_Menu :public MenuUI
{
	virtual void OnCreate(const ViewRect &srcView)override
	{
		MenuUI::OnCreate(srcView);
		CreateMenu(
			{
				{"管理图书",[&](void *) {SurfaceManager::GetInstance()->StartSurface("BookManageSurface"); }},
				{"管理用户",[&](void *) {SurfaceManager::GetInstance()->StartSurface("UserManageSurface"); }}
			}, 46, 9, 2, GravityMode::Left);
	}
};
#endif