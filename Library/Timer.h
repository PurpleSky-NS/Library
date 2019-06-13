#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <cmath>
#include <vector>
#include "Time.h"

class Timer
{
public:
	typedef std::function<void()> EventHandler;

	Timer() = default;

	// ����ÿһ֡���ø��£���һ֡���¼����ֻ�ᴥ��һ�Σ���Ҫ������������StayUpdate
	void Update()
	{
		if (!m_isRunning)
			return;
		float present = (float)clock() / (float)CLOCKS_PER_SEC;
		m_passTime += present - m_begin;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
		if (m_passTime >= m_updateTime)
		{
			m_passTime -= m_updateTime;
			Invoke();
		}
	}

	// ����ÿһ֡���ø��£���һ֡���¼��ᴥ����Σ����ܻῨ֡������Ҫ����ÿֻ֡�ܴ���һ�������Update
	void StayUpdate()
	{
		if (!m_isRunning)
			return;
		float present = (float)clock() / CLOCKS_PER_SEC;
		m_passTime += present-m_begin;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
		while (m_passTime >= m_updateTime)
		{
			m_passTime -= m_updateTime;
			Invoke();
		}
	}

	//���ö�ʱ��ʱ��
	void SetUpadateTime(float updateTime)
	{
		m_updateTime = updateTime;
	}

	//����¼�
	void AddEvent(const EventHandler &Event)
	{
		EventHandle.push_back(Event);
	}

	// ��ʼ����,���ü�ʱ��
	void Run()
	{
		m_isRunning = true;
		m_passTime = 0;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
	}

	//��ͣ��ʱ��
	void Pause()
	{
		if (!m_isRunning)
			return;
		m_isRunning = false;
		m_passTime += (float)clock() / CLOCKS_PER_SEC - m_begin;
	}

	void Resume()
	{
		if (m_isRunning)
			return;
		m_isRunning = true;
		m_begin = (float)clock() / CLOCKS_PER_SEC;
	}

	// ֹͣ��ʱ��������״̬
	void Stop()
	{
		m_isRunning = false;
		m_passTime = 0;
		m_begin = 0;
	}

	// ��ն�ʱ��Ϊ��ʼ״̬
	void Clear()
	{
		m_isRunning = false;
		m_updateTime = INFINITY;
		m_passTime = 0;
		m_begin = 0;
		EventHandle.clear();
	}

private:
	std::vector<EventHandler> EventHandle;

	bool m_isRunning = false;
	float m_updateTime;
	float m_passTime;
	float m_begin;

	void Invoke()
	{
		for(auto &i:EventHandle)
			i();
	}
};
#endif