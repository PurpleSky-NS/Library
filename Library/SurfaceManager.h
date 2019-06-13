#ifndef SURFACEMANAGER
#define SURFACEMANAGER

#include <unordered_map>
#include <string>
#include "UISurface.h"
#include "Timer.h"
#include "Time.h"



/*ʹ�õ�����ģʽ�������*/
class SurfaceManager
{
public:
	/*��ȡʵ��*/
	static SurfaceManager *GetInstance();

	/*ע�����*/
	void RegisterSurface(const std::string &name, UISurface *surface);

	/*��ȡ����*/
	UISurface *GetSurface(const std::string &name);

	inline UISurface *GetActiveSurface()const;

	/*��������*/
	void StartSurface(const std::string &name);

	/*��������*/
	void StartSurface(const std::string &name,void *data);

	/*���ٵ�ǰ�����*/
	void DestroyActiveSurface();

	/*���ٵ�ǰ����棬��������һ���µĻ����(��ʹ��ǰ�������ջ��)*/
	void RestartActiveSurface(const std::string& name);

	/*���´���ջ���Ľ��棬������false��û�л���棬�˳�����*/
	inline bool _OnUpdata();

	/*���¼�*/
	inline void _OnEvent(char input);

	/*ע����ɣ����ý����OnCreate*/
	inline void _Begin();

	/*������������ý����OnDestroy*/
	inline void _End();

private:
	static SurfaceManager *s_instance;

	SurfaceManager() = default;
	SurfaceManager(const SurfaceManager&) = delete;
	SurfaceManager(SurfaceManager&&) = delete;

	std::unordered_map<std::string, UISurface *> m_surfaces;//hash map�Ͳ�ʵ���ˣ�̫�鷳��
	ArrayList<UISurface *> m_activeSurfaces;//�����ջ��Щ���ܲ���ʵ��
	Timer m_updateTimer;

	void ResetTimer()
	{
		Time::SetDeltaTime(1.0f / GetActiveSurface()->GetFps());
		Time::s_beginDeltaTime = -1;
		m_updateTimer.Clear();
		m_updateTimer.SetUpadateTime(1.0f / GetActiveSurface()->GetFps());
		m_updateTimer.AddEvent([&]() 
		{
			Time::SetDeltaEndTime();
			Time::SetDeltaBeginTime();
			GetActiveSurface()->OnUpdate();
		});
		m_updateTimer.Run();
	}
};

UISurface *SurfaceManager::GetActiveSurface() const
{
	return m_activeSurfaces.GetLast();
}
bool SurfaceManager::_OnUpdata()
{
	if (m_activeSurfaces.IsEmpty())
		return false;
	m_updateTimer.Update();
	return true;
}
void SurfaceManager::_OnEvent(char input)
{
	if (!m_activeSurfaces.IsEmpty())
		m_activeSurfaces.GetLast()->OnEvent(input);
}
void SurfaceManager::_Begin()
{
	for (auto &i : m_surfaces)
		i.second->OnCreate();
}
void SurfaceManager::_End()
{
	for (auto &i : m_surfaces)
	{
		i.second->OnDestroy();
		delete i.second;
	}
}
#endif
