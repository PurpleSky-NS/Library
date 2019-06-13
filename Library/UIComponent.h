#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "ViewRect.h"

class UISurface;
class UIComponent
{
public:

	inline void Enable();

	inline void Disable();

	virtual void OnEnable() {}

	virtual void OnDisable() {}
	
	//����disable��������������@OnDraw,@OnUpdate��@OnEvent[���Ǹ����ΪfocusUI]
	bool IsEnable()const { return m_isEnable; }

	/*������ʱ�����,ֻ�����һ��*/
	virtual void OnCreate(const ViewRect &srcView) {}

	/*����ֵ�����Ƿ��������*/
	virtual bool OnEvent(char input) { return true; }

	virtual void OnDraw() {}

	//���������ʱ����
	virtual void OnReset() {}

	//�뿪�����ʱ����
	virtual void OnPause() {}

	//���������ʱ����
	virtual void OnResume() {}

	virtual void OnUpdate() {}

	virtual void OnDestroy() {}

	virtual void OnGetFocus() {}

	virtual void OnLoseFocus() {}

	inline virtual bool IsFocus()const;

	inline UISurface *GetContext()const;

	/*���Ʊ߿�*/
	inline void DrawFrame(ViewRect &rect, char ch);

	/*��ȡ�ַ��������Ų�ʱ�����ַ�������ʼ���꣬str.size()Ӧ��<=w��wΪһ�еĿ��*/
	inline static int GetTextCenterLayoutBeginPos(const std::string &str, int w);

	/*��ȡ�ַ��������Ų�ʱ�����ַ�������ʼ���꣬str.size()Ӧ��<=w��wΪһ�еĿ��*/
	inline static int GetTextRightLayoutBeginPos(const std::string &str, int w);


private:
	friend class UISurface;
	UISurface *m_context;
	bool m_isEnable = true;
	bool m_isFocus = false;
};
inline void UIComponent::Enable()
{
	if (m_isEnable)
		return;
	m_isEnable = true;
	OnEnable();
	OnDraw();
}
inline void UIComponent::Disable()
{
	if (!m_isEnable)
		return;
	m_isEnable = false;
	OnDisable();
}
int UIComponent::GetTextCenterLayoutBeginPos(const std::string &str, int w)
{
	return (w - str.size()) / 2;
}
int UIComponent::GetTextRightLayoutBeginPos(const std::string & str, int w)
{
	return w - str.size();
}
bool UIComponent::IsFocus() const
{
	return m_isFocus;
}
UISurface *UIComponent::GetContext() const
{
	return m_context;
}
void UIComponent::DrawFrame(ViewRect &view, char ch)
{
	Rect box = view.GetCutRect();
	std::string line(box.w, ch);
	view.SetText(0, 0, line);
	view.SetText(0, box.h - 1, line);
	for (int i = 1, limH = box.h - 1, limW = box.w - 1; i < limH; ++i)
		view(0, i) = view(limW, i) = ch;
}
#endif