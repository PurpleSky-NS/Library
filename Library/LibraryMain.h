#ifndef LIBRARYMAIN_H
#define LIBRARYMAIN_H

#include "UIMain.h"
#include "SplashSurface.h"
#include "MainSurface.h"
#include "UserSurface.h"
#include "AdminSurface.h"
#include "BookManageSurface.h"
#include "UserManagerSurface.h"
#include "BorrowMessageSurface.h"

class LibraryMain : public UIMain
{
protected:

	virtual void OnRegisterSurface()override;

};
#endif