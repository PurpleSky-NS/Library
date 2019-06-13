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
				{"����ͼ��",[&](void *) {SurfaceManager::GetInstance()->StartSurface("BookManageSurface"); }},
				{"�����û�",[&](void *) {SurfaceManager::GetInstance()->StartSurface("UserManageSurface"); }}
			}, 46, 9, 2, GravityMode::Left);
	}
};
#endif