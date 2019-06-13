#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "IUser.h"

class Administrator :public IUser
{
private:

	friend class UserManager;

	Administrator(const std::string &userName, const std::string &password);
};

#endif