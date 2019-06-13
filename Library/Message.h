#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "TimeDate.h"
#include "ForwardLinkedList.h"

struct BookBorrowMessage
{
	std::string userName;//���������ĸ��û�
	std::string borrowID;//userName+bookID+borrowTime��Ϊ������������ͬһ�û��Ĳ�ͬ��¼
};

struct BookMessage
{
	std::string ID;//��ΪΨһ��ʶ��
	std::string name;
	std::string author;
	std::string publisher;
	int totalNum;
	ForwardLinkedList<BookBorrowMessage> borrowMsg;//���ȥ�Ľ���ID�б�
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