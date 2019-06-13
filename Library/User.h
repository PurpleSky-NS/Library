#ifndef USER_H
#define USER_H

#include "Message.h"
#include "ForwardLinkedList.h"
#include "IUser.h"

class User :public IUser
{
public:

	void PushBorrowMessage(const UserBorrowMessage &msg);

	UserBorrowMessage *GetBorrowMessage(const std::string &borrowID);

	ForwardLinkedList<UserBorrowMessage> &GetBorrowMessages();

	bool RemoveBorrowMessage(const std::string &borrowID);

private:

	friend class UserManager;
	friend class BookManager;

	ForwardLinkedList<UserBorrowMessage> m_borrowedMsgs;

	User(const std::string &userName, const std::string &password, const std::string &passNum);

};

#endif
