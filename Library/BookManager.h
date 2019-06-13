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

	/*根据书名获取图书，若存在重复则返回重复的第一本书*/
	BookMessage *GetBookByName(const std::string &bookName);

	/*根据ID获取图书*/
	BookMessage *GetBookByID(const std::string &bookID);

	void AddBook(const BookMessage &book);//如果存在就增加数量
	bool AddBook(const std::string &bookID);//增加数量

	bool RemoveBookByID(const std::string &bookID);

	bool RemoveBook(const BookMessage &book);

	/*遍历图书*/
	void ForeachBooks(std::function<void(const BookMessage&)> Func)const;
	void ForeachBooks(std::function<void(BookMessage&)> Func);

	/*借书*/
	bool BorrowBook(User *user, const std::string &bookID, const TimeDate &shouldBorrowTime);

	/*还书，返回是否超时*/
	bool ReturnBook(User *user, const std::string &borrowID);

	void SaveBookData();
	
	void LoadBookData();

private:

	static BookManager *instance;

	std::unordered_map<std::string,BookMessage> m_messages;

	BookManager();

};
#endif