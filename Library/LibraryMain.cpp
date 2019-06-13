#include "LibraryMain.h"
void LibraryMain::OnRegisterSurface()
{
	RegisterSurface("SplashSurface", new SplashSurface());
	RegisterSurface("MainSurface", new MainSurface());
	RegisterSurface("UserSurface", new UserSurface());
	RegisterSurface("AdminSurface", new AdminSurface());
	RegisterSurface("BookLookSurface", new BookManageSurface(true));
	RegisterSurface("BookManageSurface", new BookManageSurface(false));
	RegisterSurface("UserManageSurface", new UserManageSurface());
	RegisterSurface("BorrowMessageSurface", new BorrowMessageSurface());
	LaunchSurface("SplashSurface");
}
int main()
{
	UIMain::SetUIMain(new LibraryMain());
	UIMain::Run();
}