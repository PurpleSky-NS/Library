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
			CreateTips("注册成功", { "恭喜，注册成功！" });
		}
	};
	class UserNameRepeated :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("注册失败", { "用户名已存在！" });
		}
	};
	class PasswordRepeatedFailed :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("注册失败", { "两次密码不一致！" });
		}
	};
	class Empty :public TipsUI
	{
	public:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			TipsUI::OnCreate(srcView);
			CreateTips("注册失败", { "用户名，密码，证件号码不能为空！" });
		}
	};
protected:
	void OnCreate(const ViewRect &view)
	{
		EditWindow::OnCreate(view);
		CreateEditor(42, 9,
			{
				{"用户名","只能为英文,数字,下划线",true,true,[](char input) {return input >= 0 && (isalnum(input) || input == '_'); }},
				{"密码","只能为英文,数字,下划线",true,false,[](char input) {return input >= 0 && isalnum(input) || input == '_'; }},
				{"重复密码","必须与密码相同",true,false,[](char input) {return input >= 0 && isalnum(input) || input == '_'; }},
				{"证件号码","只能为英文,数字",true,true,[](char input) {return input >= 0 && isalnum(input); }},
				{"管理员Key","如果无，则不输入",true,true,[](char input) {return true; }}
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