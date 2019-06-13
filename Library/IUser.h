#ifndef IUSER_H
#define IUSER_H

#include <string>

class IUser
{
public:

	inline std::string GetUserName()const;

	inline std::string GetPassWord()const;

	inline std::string GetPassNum()const;

	inline bool TryPass(const std::string &password);

	inline bool TryPassNum(const std::string &passNum);

	inline bool IsAdministrator()const;

	inline void SetUserName(const std::string &userName);

	inline void SetPassword(const std::string &password);

	inline void SetPassNum(const std::string &passNum);

protected:

	inline void SetAsUser();

	inline void SetAsAdministrator();

private:
	friend class UserManager;
	bool m_isAdministrator;
	std::string m_userName;
	std::string m_password;
	std::string m_passNum;//证件号码，身份证
};

std::string IUser::GetUserName()const
{
	return m_userName;
}
std::string IUser::GetPassWord() const
{
	return m_password;
}
std::string IUser::GetPassNum() const
{
	return m_passNum;
}
bool IUser::TryPass(const std::string &password)
{
	return password == m_password;
}
inline bool IUser::TryPassNum(const std::string & passNum)
{
	return m_passNum == passNum;
}
bool IUser::IsAdministrator()const
{
	return m_isAdministrator;
}
void IUser::SetUserName(const std::string &userName)
{
	m_userName = userName;
}
void IUser::SetPassword(const std::string &password)
{
	m_password = password;
}
void IUser::SetPassNum(const std::string &passNum)
{
	m_passNum = passNum;
}
void IUser::SetAsUser()
{
	m_isAdministrator = false;
}
void IUser::SetAsAdministrator()
{
	m_isAdministrator = true;
}
#endif