#include "UISurface.h"
UISurface::~UISurface()
{
	ClearUIComponents();
}
void UISurface::OnEnter()
{
	for (int i = 0; i < m_view.GetCutRect().h; ++i)
		m_view.FillLine(0, i, ' ');
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		msg.ui->OnReset();
		if (msg.ui->IsEnable())
			msg.ui->OnDraw();
	});
}
void UISurface::OnEnter(void * data)
{
	OnEnter();
}
void UISurface::OnExit()
{
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		msg.ui->OnPause();
	});
}
void UISurface::OnBack()
{
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		msg.ui->OnResume();
	});
}
void UISurface::OnUpdate()
{
	system("cls");
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		if (msg.ui->IsEnable())
			msg.ui->OnUpdate();
	});
	puts(m_view.GetViewData());
}
void UISurface::OnDestroy()
{
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		msg.ui->OnDestroy();
	});
	ClearUIComponents();
}
void UISurface::OnEvent(char input)
{
	if (m_focusUi != nullptr)
		if (!m_focusUi->OnEvent(input))
			return;
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		if (msg.ui != m_focusUi && msg.ui->IsEnable())
			msg.ui->OnEvent(input);
	});
}
void UISurface::AddUIComponent(UIComponent *newUI, bool isMemoryDeposit)
{
	UIComponentMsg msg = UIComponentMsg(newUI, isMemoryDeposit);
	newUI->m_context = this;
	m_uiComs.PushBack(msg);
}
void UISurface::ClearUIComponents()
{
	m_uiComs.Foreach([](const UIComponentMsg &uiCom)
	{
		if (uiCom.isMemoryDeposit)
			delete uiCom.ui;
	});
	m_uiComs.Clear();
}
void UISurface::SetFocusUI(UIComponent *focusUi)
{
	if (m_focusUi != nullptr)
	{
		m_focusUi->m_isFocus = false;
		m_focusUi->OnLoseFocus();
	}
	m_focusUi = focusUi;
	m_focusUi->m_isFocus = true;
	focusUi->OnGetFocus();
}
UIComponent *UISurface::GetFocusUI() const
{
	return m_focusUi;
}
void UISurface::Draw()
{
	m_view.Reset();
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		if (msg.ui->IsEnable())
			msg.ui->OnDraw();
	});
}
void UISurface::Draw(UIComponent *withoutUI)
{
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		if (msg.ui != withoutUI && msg.ui->IsEnable())
			msg.ui->OnDraw();
	});
}
unsigned char UISurface::GetFps() const
{
	return m_fps;
}

void UISurface::OnCreatedUI()
{
	m_uiComs.Foreach([&](const UIComponentMsg &msg)
	{
		msg.ui->OnCreate(m_view);
	});
}
