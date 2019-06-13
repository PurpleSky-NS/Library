#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <fstream>
#include <unordered_map>
#include "ForwardLinkedList.h"
#include "Value.h"
#include "Message.h"
#include "Administrator.h"
#include "User.h"

class UserManager
{
public:
	struct UserMessage
	{
		std::string *password;
		std::string *passNum;
		bool *isAdministrator;
		ForwardLinkedList<UserBorrowMessage> *borrowMsgs;
	};

	static UserManager *GetInstance();

	bool HasUser(const std::string &userName)const;

	IUser *GetUser(const std::string &userName);

	/*无法注册用户名相同的用户，管理员的话passNum不用填*/
	bool Register(const std::string &userName, const std::string &password, const std::string &passNum, bool isAdministrator = false);

	/*登陆成功返回这个用户*/
	IUser *Login(const std::string &userName, const std::string &password);

	IUser *GetLoginUser();

	void ForeachUsers(std::function<void(IUser *)> Func);

	/*记得操作完即时保存*/
	/*载入用户数据*/
	void LoadUserData();

	/*保存用户数据*/
	void SaveUserData();

private:

	static UserManager *instance;

	std::unordered_map<std::string,IUser*> m_users;
	IUser *m_loginUser = nullptr;

	UserManager();
};
#endif
