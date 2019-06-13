#include "TimeDate.h"
TimeDate::TimeDate()
{
	GetTime();
}

TimeDate::TimeDate(long tm)
{
	tt = tm;
	GetTimeFromtt();
}

TimeDate::TimeDate(const TimeDate &tm)
{
	tt = tm.tt;
	GetTimeFromtt();
}

void TimeDate::GetTime()
{
	tt = time(nullptr);
	GetTimeFromtt();
}

double TimeDate::DiffTime(const TimeDate &tm, Fmt fmt)const
{
	double r = 0;
	r = difftime(tt, tm.tt);
	if (fmt == Min)
		r = r / 60;
	else if (fmt == Hour)
		r = r / 3600;
	else if (fmt == Day)
		r = r / (3600 * 24);
	return r;
}

void TimeDate::DiffTime(const TimeDate &tm, int &dday, int &dhour, int &dmin, int &dsec)const
{
	double r = 0;
	dday = dhour = dmin = dsec = 0;
	r = difftime(tt, tm.tt);
	if (r <= 0)
		r = -r;
	while (r >= 86400)
	{
		dday++;
		r -= 86400;
	}
	while (r >= 3600)
	{
		dhour++;
		r -= 3600;
	}
	while (r >= 60)
	{
		dmin++;
		r -= 60;
	}
	dsec = int(r);
}

void TimeDate::AddTime(int ad, int ah, int am, int as)
{
	long all = 0;
	all += ad * 86400 + ah * 3600 + am * 60 + as;
	tt += all;
	GetTimeFromtt();
}

std::string TimeDate::TimeStr()const
{
	char str[32];
	sprintf(str, "%d-%02d-%d#%02d:%02d:%02d", year, mon, day, hour, min, sec);
	return str;
}

void TimeDate::Parse(const std::string &timeStr)
{
	sscanf(timeStr.c_str(), "%d-%02d-%d#%02d:%02d:%02d", &year, &mon, &day, &hour, &min, &sec);
	MakeTime();
}

bool TimeDate::MakeTime()
{
	tm ttm;
	ttm.tm_year = year - 1900;
	ttm.tm_mon = mon - 1;
	ttm.tm_mday = day;
	ttm.tm_hour = hour;
	ttm.tm_min = min;
	ttm.tm_sec = sec;
	ttm.tm_isdst = -1;
	tt = mktime(&ttm);
	if (tt == -1)
		return false;
	GetTimeFromtt();
	return true;
}

TimeDate &TimeDate::operator=(const TimeDate &tm)
{
	tt = tm.tt;
	GetTimeFromtt();
	return *this;
}

TimeDate &TimeDate::operator=(long tm)
{
	tt = tm;
	GetTimeFromtt();
	return *this;
}

TimeDate TimeDate::operator+(long add)const
{
	TimeDate ins;
	ins.tt = tt + add;
	ins.GetTimeFromtt();
	return ins;
}

TimeDate &TimeDate::operator+=(long add)
{
	tt = tt + add;
	GetTimeFromtt();
	return *this;
}

double TimeDate::operator-(const TimeDate &tm)const
{
	return this->DiffTime(tm, Sec);
}

TimeDate &TimeDate::operator-=(long add)
{
	tt = tt - add;
	GetTimeFromtt();
	return *this;
}

bool TimeDate::operator==(const TimeDate &tm)const
{
	return tt == tm.tt;
}

bool TimeDate::operator!=(const TimeDate &tm)const
{
	return tt != tm.tt;
}

bool TimeDate::operator>(const TimeDate &tm)const
{
	return tt > tm.tt;
}

bool TimeDate::operator<(const TimeDate &tm)const
{
	return tt < tm.tt;
}

bool TimeDate::operator>=(const TimeDate &tm)const
{
	return tt >= tm.tt;
}

bool TimeDate::operator<=(const TimeDate &tm)const
{
	return tt <= tm.tt;
}

std::ostream& operator<<(std::ostream & out, const TimeDate & timeDate)
{
	out << timeDate.TimeStr();
	return out;
}

std::istream& operator>>(std::istream & in, TimeDate & timeDate)
{
	std::string str;
	in >> str;
	timeDate.Parse(str);
	return in;
}