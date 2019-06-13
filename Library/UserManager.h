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

	/*�޷�ע���û�����ͬ���û�������Ա�Ļ�passNum������*/
	bool Register(const std::string &userName, const std::string &password, const std::string &passNum, bool isAdministrator = false);

	/*��½�ɹ���������û�*/
	IUser *Login(const std::string &userName, const std::string &password);

	IUser *GetLoginUser();

	void ForeachUsers(std::function<void(IUser *)> Func);

	/*�ǵò����꼴ʱ����*/
	/*�����û�����*/
	void LoadUserData();

	/*�����û�����*/
	void SaveUserData();

private:

	static UserManager *instance;

	std::unordered_map<std::string,IUser*> m_users;
	IUser *m_loginUser = nullptr;

	UserManager();
};
#endif
