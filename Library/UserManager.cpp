#include "UserManager.h"
UserManager *UserManager::instance = nullptr;
UserManager *UserManager::GetInstance()
{
	if (instance == nullptr)
		instance = new UserManager();
	return instance;
}

bool UserManager::HasUser(const std::string &userName)const
{
	return m_users.find(userName) != m_users.end();
}

bool UserManager::Register(const std::string &userName, const std::string &password, const std::string &passNum, bool isAdministrator)
{
	if (HasUser(userName))
		return false;
	m_users.insert(std::pair<std::string, IUser*>(userName, isAdministrator ? (IUser*)new Administrator(userName, password) : (IUser*)new User(userName, password, passNum)));
	return true;
}

IUser *UserManager::Login(const std::string &userName, const std::string &password)
{
	IUser *loginUser = GetUser(userName);
	if (loginUser == nullptr)
		return nullptr;
	if (loginUser->TryPass(password))
	{
		m_loginUser = loginUser;
		return loginUser;
	}
	return nullptr;
}
IUser * UserManager::GetLoginUser()
{
	return m_loginUser;
}
void UserManager::ForeachUsers(std::function<void(IUser *)> Func)
{
	for (auto &i : m_users)
		Func(i.second);
}

void UserManager::SaveUserData()
{
	std::ofstream out(UserSavePath);
	if (!out.is_open())
		return;

	for (auto i:m_users)
	{
		IUser *iuser = i.second;
		out << iuser->m_userName << ' ' << iuser->m_password << ' '
			<< (iuser->m_isAdministrator ? "true" : "false") << ' ' << std::endl;
		if (!iuser->IsAdministrator())
		{
			User *user = (User*)iuser;
			out << user->m_passNum << ' '
				<< user->m_borrowedMsgs.Size() << std::endl;
			user->m_borrowedMsgs.Foreach([&](const UserBorrowMessage &msg)
			{
				out << msg.borrowID << ' ' << msg.bookID << ' ' << msg.borrowTime << ' ' << msg.shouldReturnTime << ' ' << msg.isDone << ' ';
				if (msg.isDone)
					out << msg.returnTime;
				out << std::endl;
			});
		}
	}
}

void UserManager::LoadUserData()
{
	std::ifstream in(UserSavePath);
	if (!in.is_open())
		return;

	std::string userName, password, access;
	while (in >> userName >> password >> access)
	{
		IUser *iuser;
		if (access == "true")
			iuser = new Administrator(userName, password);
		else
		{
			int size;
			std::string passNum; 
			UserBorrowMessage msg;
			in >> passNum;
			User *user = new User(userName, password, passNum);
			iuser = user;
			in >> size;
			for (int i = 0; i < size; ++i)
			{
				in >> msg.borrowID >> msg.bookID >> msg.borrowTime >> msg.shouldReturnTime >> msg.isDone;
				if (msg.isDone)
					in >> msg.returnTime;
				user->m_borrowedMsgs.PushBack(msg);
			}
		}
		m_users.insert(std::pair<std::string, IUser*>(userName, iuser));
	}
}

UserManager::UserManager()
{
	LoadUserData();
}

IUser * UserManager::GetUser(const std::string & userName)
{
	auto user = m_users.find(userName);
	if (user != m_users.end())
		return user->second;
	return nullptr;
}