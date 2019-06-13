#ifndef TIMEDATE_H
#define TIMEDATE_H

#include <ctime>
#include <string>
#include <cstdio>

struct TimeDate
{
public:
	enum Fmt { Sec, Min, Hour, Day };//返回时间差格式，分别为：秒，分，时，日
	int year = 0;//年
	int mon = 0;//月
	int day = 0;//日
	int hour = 0;//小时
	int min = 0;//分钟
	int sec = 0;//秒

	void GetTime();//获取当前时间

	/*返回时间差，若是负数则表示本时间比要比较的时间早
	第二个参数为返回格式，Sec为秒，Min为分，Hour为时，Day为天
	ps：格式为enum类型*/
	double DiffTime(const TimeDate&, Fmt)const;

	/*时间差的绝对值，细分到几天，小时，分钟，秒钟（类似于闹钟）
	第二个参数是相差的天数，第三个是小时，第四个是分钟，第五个是秒钟*/
	void DiffTime(const TimeDate&, int&, int&, int&, int&)const;

	/*第一个参数为天数，第二个是小时，第三个是分钟，第四个是秒钟*/
	void AddTime(int, int, int, int);//时间向后推移

	/*获取时间字符串*/
	std::string TimeStr()const;

	/*根据上面那个函数的格式转化*/
	void Parse(const std::string &timeStr);

	/*将年，月，日，时，分，秒设置完成后，使用本函数进行设置，返回是否设置成功
	上限为2038年1月18日19时14分07秒
	下限为1902年1月1日0时0分0秒
	ps：不同系统可能会不同
	*/
	bool MakeTime();

	TimeDate &operator=(const TimeDate&);//重载=
	TimeDate &operator=(long);//支持库头文件的time_t（单位：秒）
	TimeDate operator+(long)const;//可以将时间后移（单位：秒）
	TimeDate &operator+=(long);//可以将时间后移（单位：秒）
	double operator-(const TimeDate&)const;//返回时间差，类似于DiffTimeDate（单位：秒）
	TimeDate &operator-=(long);//可以将时间前移（单位：秒）

	//重载比较运算符
	bool operator ==(const TimeDate&)const;
	bool operator !=(const TimeDate&)const;
	bool operator >(const TimeDate&)const;
	bool operator <(const TimeDate&)const;
	bool operator >=(const TimeDate&)const;
	bool operator <=(const TimeDate&)const;

	//构造
	TimeDate();//默认
	TimeDate(long);//支持库头文件的time_t
	TimeDate(const TimeDate&);//复制构造

//以下内容请自动忽略
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