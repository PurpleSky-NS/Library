#ifndef TIMEDATE_H
#define TIMEDATE_H

#include <ctime>
#include <string>
#include <cstdio>

struct TimeDate
{
public:
	enum Fmt { Sec, Min, Hour, Day };//����ʱ����ʽ���ֱ�Ϊ���룬�֣�ʱ����
	int year = 0;//��
	int mon = 0;//��
	int day = 0;//��
	int hour = 0;//Сʱ
	int min = 0;//����
	int sec = 0;//��

	void GetTime();//��ȡ��ǰʱ��

	/*����ʱ�����Ǹ������ʾ��ʱ���Ҫ�Ƚϵ�ʱ����
	�ڶ�������Ϊ���ظ�ʽ��SecΪ�룬MinΪ�֣�HourΪʱ��DayΪ��
	ps����ʽΪenum����*/
	double DiffTime(const TimeDate&, Fmt)const;

	/*ʱ���ľ���ֵ��ϸ�ֵ����죬Сʱ�����ӣ����ӣ����������ӣ�
	�ڶ���������������������������Сʱ�����ĸ��Ƿ��ӣ������������*/
	void DiffTime(const TimeDate&, int&, int&, int&, int&)const;

	/*��һ������Ϊ�������ڶ�����Сʱ���������Ƿ��ӣ����ĸ�������*/
	void AddTime(int, int, int, int);//ʱ���������

	/*��ȡʱ���ַ���*/
	std::string TimeStr()const;

	/*���������Ǹ������ĸ�ʽת��*/
	void Parse(const std::string &timeStr);

	/*���꣬�£��գ�ʱ���֣���������ɺ�ʹ�ñ������������ã������Ƿ����óɹ�
	����Ϊ2038��1��18��19ʱ14��07��
	����Ϊ1902��1��1��0ʱ0��0��
	ps����ͬϵͳ���ܻ᲻ͬ
	*/
	bool MakeTime();

	TimeDate &operator=(const TimeDate&);//����=
	TimeDate &operator=(long);//֧�ֿ�ͷ�ļ���time_t����λ���룩
	TimeDate operator+(long)const;//���Խ�ʱ����ƣ���λ���룩
	TimeDate &operator+=(long);//���Խ�ʱ����ƣ���λ���룩
	double operator-(const TimeDate&)const;//����ʱ��������DiffTimeDate����λ���룩
	TimeDate &operator-=(long);//���Խ�ʱ��ǰ�ƣ���λ���룩

	//���رȽ������
	bool operator ==(const TimeDate&)const;
	bool operator !=(const TimeDate&)const;
	bool operator >(const TimeDate&)const;
	bool operator <(const TimeDate&)const;
	bool operator >=(const TimeDate&)const;
	bool operator <=(const TimeDate&)const;

	//����
	TimeDate();//Ĭ��
	TimeDate(long);//֧�ֿ�ͷ�ļ���time_t
	TimeDate(const TimeDate&);//���ƹ���

//�����������Զ�����
private:
	std::tm t;
	std::time_t tt;
	void GetTimeFromtt()
	{
		t = *localtime(&tt);
		sec = t.tm_sec;
		min = t.tm_min;
		hour = t.tm_hour;
		day = t.tm_mday;
		mon = t.tm_mon;
		mon += 1;
		year = t.tm_year;
		year += 1900;
	}
};
std::ostream &operator<<(std::ostream &out, const TimeDate &timeDate);
std::istream &operator>>(std::istream &in, TimeDate &timeDate);
#endif