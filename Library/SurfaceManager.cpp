#include "SurfaceManager.h"
SurfaceManager *SurfaceManager::s_instance = nullptr;
SurfaceManager * SurfaceManager::GetInstance()
{
	if (s_instance == nullptr)
		s_instance = new SurfaceManager();
	return s_instance;
}

void SurfaceManager::RegisterSurface(const std::string &name, UISurface * surface)
{
	m_surfaces.insert(std::pair<std::string, UISurface *>(name, surface));
	surface->OnCreateUI();
	surface->OnCreate();
	surface->OnCreatedUI();
}

UISurface * SurfaceManager::GetSurface(const std::string & name)
{
	return m_surfaces[name];
}

void SurfaceManager::StartSurface(const std::string &name)
{
	if(!m_activeSurfaces.IsEmpty())
		m_activeSurfaces.GetLast()->OnExit();
	UISurface *surface = m_surfaces[name];
	m_activeSurfaces.PushBack(surface);
	surface->OnEnter();
	ResetTimer();
}

void SurfaceManager::StartSurface(const std::string & name, void *data)
{
	if (!m_activeSurfaces.IsEmpty())
		m_activeSurfaces.GetLast()->OnExit();
	UISurface *surface = m_surfaces[name];
	m_activeSurfaces.PushBack(surface);
	surface->OnEnter(data);
	ResetTimer();
}

void SurfaceManager::DestroyActiveSurface()
{
	if (m_activeSurfaces.IsEmpty())
		return;
	m_activeSurfaces.GetLast()->OnQuit();
	m_activeSurfaces.RemoveLast();
	if (!m_activeSurfaces.IsEmpty())
	{
		m_activeSurfaces.GetLast()->OnBack();
		ResetTimer();
	}
}

void SurfaceManager::RestartActiveSurface(const std::string &name)
{
	DestroyActiveSurface();
	StartSurface(name);
}
