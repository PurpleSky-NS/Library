#ifndef TIPSUI_H
#define TIPSUI_H

#include <queue>
#include <vector>
#include <string>
#include "SurfaceManager.h"
#include "Timer.h"
#include "UIComponent.h"

/*在右下角提示的提示框UI*/
class TipsUI :public UIComponent
{
	static TipsUI *activeTips;//同一时间只能有一个tips
public:

	/*创建提示*/
	void CreateTips(const std::string &title, const std::vector<std::string> &content);

	/*调用显示*/
	inline void Show();

protected:

	/*子类需要先调用该方法*/
	virtual void OnCreate(const ViewRect &srcView)override;

	inline virtual void OnEnable()override;

	inline virtual void OnDisable()override;

	inline virtual void OnUpdate()override;

private:
	ViewRect m_srcView, m_cutView;
	float m_passTime;
	const float m_fadeTime = 1.0f;
	const float m_stayTime = 3.0f;
	std::vector<std::string> m_datas;
	Timer *m_updateTimer;
	Timer m_beginTimer;
	Timer m_stayTimer;
	Timer m_fadeTimer;
	std::function<void()> *Update;
	std::function<void()> Begin;
	std::function<void()> Stay;
	std::function<void()> Fade;

	void Reset();

	void Draw(float percent);
};
void TipsUI::Show()
{
	if (activeTips != nullptr)
		activeTips->Disable();
	activeTips = this;
	Enable();//加上添加条件
}
void TipsUI::OnEnable()
{
	Reset();
}
void TipsUI::OnDisable()
{
	GetContext()->Draw();
}
void TipsUI::OnUpdate()
{
	(*Update)();
	m_updateTimer->Update();
	m_passTime += Time::GetDeltaTime();
}
#endif