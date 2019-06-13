#ifndef MAINSURFACE_REGISTER_H
#define MAINSURFACE_REGISTER_H

#include "UserManager.h"
#include "TipsUI.h"
#include "EditWindow.h"

class MainSurface_Register :public EditWindow
{
public:
	class Succeed :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("ע��ɹ�", { "��ϲ��ע��ɹ���" });
		}
	};
	class UserNameRepeated :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("ע��ʧ��", { "�û����Ѵ��ڣ�" });
		}
	};
	class PasswordRepeatedFailed :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("ע��ʧ��", { "�������벻һ�£�" });
		}
	};
	class Empty :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("ע��ʧ��", { "�û��������룬֤�����벻��Ϊ�գ�" });
		}
	};
protected:
	void OnCreate(const ViewRect &view)
	{
		EditWindow::OnCreate(view);
		CreateEditor(42, 9,
			{
				{"�û���","ֻ��ΪӢ��,����,�»���",true,true,[](char input) {return input >= 0 && (isalnum(input) || input == '_'); }},
				{"����","ֻ��ΪӢ��,����,�»���",true,false,[](char input) {return input >= 0 && isalnum(input) || input == '_'; }},
				{"�ظ�����","������������ͬ",true,false,[](char input) {return input >= 0 && isalnum(input) || input == '_'; }},
				{"֤������","ֻ��ΪӢ��,����",true,true,[](char input) {return input >= 0 && isalnum(input); }},
				{"����ԱKey","����ޣ�������",true,true,[](char input) {return true; }}
			});
	}

	virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
	{
		if (msgs[1].resualt != msgs[2].resualt)
		{
			GetContext()->GetUIComponent<PasswordRepeatedFailed>()->Show();
			return false;
		}
		if (msgs[0].resualt.empty() || msgs[1].resualt.empty() || msgs[2].resualt.empty())
		{
			GetContext()->GetUIComponent<Empty>()->Show();
			return false;
		}
		if (!UserManager::GetInstance()->Register(msgs[0].resualt, msgs[1].resualt,msgs[3].resualt, msgs[4].resualt=="000219"))
		{
			GetContext()->GetUIComponent<UserNameRepeated>()->Show();
			return false;
		}
		GetContext()->GetUIComponent<Succeed>()->Show();
		UserManager::GetInstance()->SaveUserData();
		return true;
	}
};
#endif