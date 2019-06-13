#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include <unordered_map>
#include "UserManager.h"
#include "User.h"
#include "Message.h"

class BookManager
{
public:
	static BookManager *GetInstance();

	/*����������ȡͼ�飬�������ظ��򷵻��ظ��ĵ�һ����*/
	BookMessage *GetBookByName(const std::string &bookName);

	/*����ID��ȡͼ��*/
	BookMessage *GetBookByID(const std::string &bookID);

	void AddBook(const BookMessage &book);//������ھ���������
	bool AddBook(const std::string &bookID);//��������

	bool RemoveBookByID(const std::string &bookID);

	bool RemoveBook(const BookMessage &book);

	/*����ͼ��*/
	void ForeachBooks(std::function<void(const BookMessage&)> Func)const;
	void ForeachBooks(std::function<void(BookMessage&)> Func);

	/*����*/
	bool BorrowBook(User *user, const std::string &bookID, const TimeDate &shouldBorrowTime);

	/*���飬�����Ƿ�ʱ*/
	bool ReturnBook(User *user, const std::string &borrowID);

	void SaveBookData();
	
	void LoadBookData();

private:

	static BookManager *instance;

	std::unordered_map<std::string,BookMessage> m_messages;

	BookManager();

};
#endif