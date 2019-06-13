#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "TimeDate.h"
#include "ForwardLinkedList.h"

struct BookBorrowMessage
{
	std::string userName;//用于区分哪个用户
	std::string borrowID;//userName+bookID+borrowTime作为键，用于区分同一用户的不同记录
};

struct BookMessage
{
	std::string ID;//作为唯一标识符
	std::string name;
	std::string author;
	std::string publisher;
	int totalNum;
	ForwardLinkedList<BookBorrowMessage> borrowMsg;//借出去的借阅ID列表
};

struct UserBorrowMessage
{
	std::string borrowID;
	std::string bookID;
	TimeDate borrowTime;
	TimeDate shouldReturnTime;
	TimeDate returnTime;
	bool isDone;
};

#endif