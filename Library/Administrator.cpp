#include "Administrator.h"
Administrator::Administrator(const std::string &userName, const std::string &password)
{
	SetUserName(userName);
	SetPassword(password);
	SetPassNum("Admin");
	SetAsAdministrator();
}
