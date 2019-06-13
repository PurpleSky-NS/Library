#ifndef UIMAIN_H
#define UIMAIN_H

#include <cstdio>
#include <thread>
#include <conio.h>
#include "Time.h"
#include "SurfaceManager.h"
#include "EventPump.h"

class UIMain
{
public:
	
	static void SetUIMain(UIMain *main);

	static void Run();

protected:

	UIMain();

	virtual void OnRegisterSurface() {}

	/*ע�����*/
	void RegisterSurface(const std::string &name, UISurface *surface);

	/*��������*/
	void LaunchSurface(const std::string &name);

private:

	static UIMain *s_main;

	static SurfaceManager *m_surfaceManager;
};
#endif