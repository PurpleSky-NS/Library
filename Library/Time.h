#ifndef TIME_H
#define TIME_H

#include <ctime>

/*����Ϊ��λ*/
class Time
{
public:
	/*��ȡ����һ֡���þ�����ʱ��*/
	inline static float GetDeltaTime();

	/*��ȡ�������о���ʱ��*/
	inline static float GetRunningTime();

private:
	friend class SurfaceManager;

	static float s_deltaTime;
	static float s_beginDeltaTime;

	/*������һ֡�Ŀ�ʼʱ��*/
	inline static void SetDeltaTime(float delta);

	/*������һ֡�Ŀ�ʼʱ��*/
	inline static void SetDeltaBeginTime();

	/*������һ֡�Ľ���ʱ��*/
	inline static void SetDeltaEndTime();

};
inline void Time::SetDeltaTime(float delta)
{
	s_deltaTime = delta;
}
void Time::SetDeltaBeginTime()
{
	s_beginDeltaTime = clock() / (float)CLOCKS_PER_SEC;
}

void Time::SetDeltaEndTime()
{
	if (s_beginDeltaTime >= 0)
		s_deltaTime = clock() / (float)CLOCKS_PER_SEC - s_beginDeltaTime;
}

float Time::GetDeltaTime()
{
	return s_deltaTime;
}

float Time::GetRunningTime()
{
	return clock() / (float)CLOCKS_PER_SEC;
}

#endif