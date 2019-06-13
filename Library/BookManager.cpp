#include "BookManager.h"
BookManager *BookManager::instance = nullptr;
BookManager * BookManager::GetInstance()
{
	if (instance == nullptr)
		instance = new BookManager();
	return instance;
}
BookMessage *BookManager::GetBookByName(const std::string &bookName)
{
	for (auto i = m_messages.begin(); i != m_messages.end(); ++i)
		if (i->second.name == bookName)
			return &i->second;
	return nullptr;
}
BookMessage *BookManager::GetBookByID(const std::string & bookID)
{
	auto fd = m_messages.find(bookID);
	if (fd != m_messages.end())
		return &fd->second;
	return nullptr;
}
void BookManager::AddBook(const BookMessage & book)
{
	auto fd = m_messages.find(book.ID);
	if (fd != m_messages.end())
		++fd->second.totalNum;
	else
		m_messages.insert(std::pair<std::string, BookMessage>(book.ID, book));
}
bool BookManager::AddBook(const std::string &bookID)
{
	auto fd = m_messages.find(bookID);
	if (fd != m_messages.end())
	{
		++fd->second.totalNum;
		return true;
	}
	return false;
}
bool BookManager::RemoveBookByID(const std::string &bookID)
{
	auto fd = m_messages.find(bookID);
	if (fd != m_messages.end())
	{
		m_messages.erase(bookID);
		return true;
	}
	return false;
}
bool BookManager::RemoveBook(const BookMessage &book)
{
	return RemoveBookByID(book.ID);
}
void BookManager::ForeachBooks(std::function<void(const BookMessage&)> Func) const
{
	for (auto i : m_messages)
		Func(i.second);
}
void BookManager::ForeachBooks(std::function<void(BookMessage&)> Func)
{
	for (auto &i : m_messages)
		Func(i.second);
}
bool BookManager::BorrowBook(User *user, const std::string & bookID, const TimeDate & shouldReturnTime)
{
	if (user == nullptr)
		return false;
	BookMessage *book = GetBookByID(bookID);
	if (book == nullptr|| book->borrowMsg.Size() == book->totalNum)
		return false;

	UserBorrowMessage userBMsg;
	userBMsg.bookID = bookID;
	userBMsg.borrowTime.GetTime();
	userBMsg.shouldReturnTime = shouldReturnTime;
	userBMsg.isDone = false;
	userBMsg.borrowID = user->GetUserName() + userBMsg.bookID + userBMsg.borrowTime.TimeStr();

	BookBorrowMessage bookBMsg;
	bookBMsg.borrowID = userBMsg.borrowID;
	bookBMsg.userName = user->GetUserName();

	user->PushBorrowMessage(userBMsg);
	book->borrowMsg.PushBack(bookBMsg);
	return false;
}
bool BookManager::ReturnBook(User *user, const std::string &borrowID)
{
	UserBorrowMessage *msg = user->GetBorrowMessage(borrowID);
	msg->isDone = true;
	msg->returnTime.GetTime();
	GetBookByID(msg->bookID)->borrowMsg.Remove([&](const BookBorrowMessage &msg)
	{
		return msg.borrowID == borrowID;
	});
	return msg->returnTime > msg->shouldReturnTime;
}
void BookManager::SaveBookData()
{
	std::ofstream out(BookSavePath);
	if (!out.is_open())
		return;

	for (auto i : m_messages)
	{
		BookMessage &msg = i.second;
		out << msg.ID << ' '
			<< msg.name << ' '
			<< msg.author << ' '
			<< msg.publisher << ' '
			<< msg.totalNum << ' '
			<< msg.borrowMsg.Size() << std::endl;
		msg.borrowMsg.Foreach([&](const BookBorrowMessage &msg)
		{
			out << msg.userName << ' ' << msg.borrowID << std::endl;
		});
	}
}
void BookManager::LoadBookData()
{
	std::ifstream in(BookSavePath);
	if (!in.is_open())
		return;
	
	int size;
	BookMessage msg;
	BookBorrowMessage bbmsg;
	while(in >> msg.ID
			>> msg.name
			>> msg.author
			>> msg.publisher
			>> msg.totalNum
			>> size)
	{
		for (int i = 0; i < size; ++i)
		{
			in >> bbmsg.userName >> bbmsg.borrowID;
			msg.borrowMsg.PushBack(bbmsg);
		}
		m_messages.insert(std::pair<std::string, BookMessage>(msg.ID, msg));
	}
}
BookManager::BookManager()
{
	LoadBookData();
}