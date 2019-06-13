#include "User.h"
void User::PushBorrowMessage(const UserBorrowMessage & msg)
{
	m_borrowedMsgs.PushBack(msg);
}
UserBorrowMessage *User::GetBorrowMessage(const std::string & borrowID)
{
	return m_borrowedMsgs.Find([&](const UserBorrowMessage &msg)
	{
		return msg.borrowID == borrowID;
	});
	
}
ForwardLinkedList<UserBorrowMessage>& User::GetBorrowMessages()
{
	return m_borrowedMsgs;
}
bool User::RemoveBorrowMessage(const std::string &borrowID)
{
	return m_borrowedMsgs.Remove([&](const UserBorrowMessage &msg)
	{
		return msg.borrowID == borrowID;
	});
}
User::User(const std::string &userName, const std::string &password, const std::string &passNum)
{
	SetUserName(userName);
	SetPassword(password);
	SetPassNum(passNum);
	SetAsUser();
}
