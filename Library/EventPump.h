#ifndef EVENTPUMB_H
#define EVENTPUMB_H

#include <queue>
#include <conio.h>

class EventPump
{
public:
	/*�����¼����øú����¿�һ���߳�*/
	static void UpdateEventThread();

	/*�ر��¼��߳�*/
	static void StopPumbThread();

	/*�������뷵��0*/
	static char GetInput();

	/*�Ƿ�������*/
	static bool HasInput();

private:
	static std::queue<char> m_eventQueue;
	static bool m_isRunning;
};
#endif