#ifndef UISURFACE_H
#define UISURFACE_H

#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include "UIComponent.h"
#include "ArrayList.h"

/*һ�����棬Ӧ������̳в����*/
class UISurface
{	
public:

	virtual ~UISurface();

	/*�����ﴴ�������UI���@AddUIComponent*/
	virtual void OnCreateUI() {}

	/*�ڽ����һ�δ�����ʱ�����,ֻ�����һ��,�������FocusUI�����¼�*/
	virtual void OnCreate() {}

	/*������������*/
	/*�ڽ�������ʱ�����*/
	virtual void OnEnter();

	/*������������*/
	/*����һ�������İ汾*/
	virtual void OnEnter(void *data);

	/*�ڽ���֮���л�����һ�������ʱ�����*/
	virtual void OnExit();

	/*�ڴ���һ�����淵�ؽ���ʱ����*/
	virtual void OnBack();

	/*�ڽ����˳�ʱ����*/
	virtual void OnQuit() {}

	/*������������*/
	/*�ڽ�����Ƶ�ʱ�����*/
	virtual void OnUpdate();

	/*������������*/
	/*�ڽ������ٵ�ʱ�����*/
	virtual void OnDestroy();

	/*�����¼�����*/
	virtual void OnEvent(char input);

	/*��ȡ��ͼ*/
	inline ViewRect *GetView();

	/*��ȡUI���*/
	template<class T>
	T *GetUIComponent();

	/*��ȡUI���*/
	template<class T>
	ArrayList<T*>GetUIComponents();

	/*���ý���UI����UI����������¼���Ϣ��������Ȩ�����Ƿ��¼�����*/
	void SetFocusUI(UIComponent *focusUi);

	/*��ȡ����UI*/
	UIComponent * GetFocusUI()const;

	/*ǿ�ƽ����ػ�*/
	void Draw();

	/*ǿ�ƽ����ػ棬���ػ�withoutUI*/
	void Draw(UIComponent *withoutUI);

	/*��ȡ֡��*/
	unsigned char GetFps()const;

protected:

	UISurface(unsigned char fps,int w,int h)
		: m_fps(fps)
	{
		m_view.SetViewRect(w, h);
	}

	/*���UI������ͷ��йܵ���Դ*/
	void ClearUIComponents();

	/*���UI������Ƿ�����ڴ��й�[�����˳�ʱ�Զ��ͷ���Դ]*/
	void AddUIComponent(UIComponent *newUI, bool isMemoryDeposit = true);

private:
	friend class SurfaceManager;

	struct UIComponentMsg
	{
		UIComponentMsg() = default;
		UIComponentMsg(UIComponent *ui, bool isMemoryDeposited = true)
		{
			this->ui = ui;
			this->isMemoryDeposit = isMemoryDeposited;
		}

		UIComponent *ui;
		bool isMemoryDeposit;
	};

	unsigned char m_fps;//֡��
	ViewRect m_view;
	ArrayList<UIComponentMsg> m_uiComs;
	UIComponent *m_focusUi = nullptr;

	/*��������UIʱ����*/
	void OnCreatedUI();
};
ViewRect *UISurface::GetView()
{
	return &m_view;
}
template<class T>
T *UISurface::GetUIComponent()
{
	return (T*)(m_uiComs.Find([&](const UIComponentMsg &uiMsg)
	{
		return typeid(*(uiMsg.ui)) == typeid(T);
	})->ui);
}
template<class T>
ArrayList<T*> UISurface::GetUIComponents()
{
	ArrayList<T*> uis;
	m_uiComs.Foreach([&](const UIComponentMsg &uiMsg)
	{
		if (typeid(*(uiMsg.ui)) == typeid(T))
			uis.PushBack(uiMsg.ui);
	});
	return uis;
}
#endif

