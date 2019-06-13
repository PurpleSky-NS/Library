#ifndef BORROWMESSAGESURFACE_H
#define BORROWMESSAGESURFACE_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Value.h"
#include "Title.h"
#include "MenuUI.h"
#include "TipsUI.h"
#include "EditWindow.h"
#include "UserManager.h"
#include "BookManager.h"

class BorrowMessageSurface : public UISurface
{
	class BorrowMenuList :public MenuUI
	{
		User *m_user;
	public:

		void SetUser(User *user)
		{
			m_user = user;
		}

		void Refresh()
		{
			std::vector<MenuItem> userItems;
			m_user->GetBorrowMessages().Foreach([&](UserBorrowMessage &msg)
			{
				MenuItem item;
				BookMessage *book = BookManager::GetInstance()->GetBookByID(msg.bookID);
				std::ostringstream oss;
				oss.setf(std::ios::left);
				oss <<"�����鼮:" << std::setw(10) << book->name  << msg.borrowTime.TimeStr() << "  -> " << msg.shouldReturnTime.TimeStr() << "  ״̬:" << (msg.isDone ? "�ѻ�" : "δ��");
				item.name = oss.str();
				item.data = &msg;
				item.Event = [&](void *data)
				{
					GetContext()->GetUIComponent<UserMenu>()->Show((UserBorrowMessage*)data,m_user);
				};
				userItems.push_back(std::move(item));
			});
			CreateMenu(userItems, 40, 2, 0, CONSOLE_HEIGHT - 3, GravityMode::Left, true);
		}

		virtual void OnReset()override
		{
			Refresh();
		}
	};

	class ReturnWindow :public EditWindow
	{
		User *m_user;
		UserBorrowMessage *m_msg;
	public:
		class Succeed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("�黹�ɹ�", { "����ʱ���ڹ黹�ɹ���" });
			}
		};
		class Failed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("�黹ʧ��", { "֤��������֤ʧ�ܣ�" });
			}
		};
		class OutTime :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("�黹�ɹ������ĳ�ʱ", { "��ȥ���������ɷ��ã�" });
			}
		};

		void Show(UserBorrowMessage *msg, User *user)
		{
			m_msg = msg;
			m_user = user;
			EditWindow::Show();
		}
	private:
		void OnCreate(const ViewRect &srcView)
		{
			EditWindow::OnCreate(srcView);
			CreateEditor(40, 5,
				{
					{"֤������","������֤������ȷ�Ϲ黹ͼ��",true,true,[](char input) {return true; }},
				});
		}

		virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
		{
			if (msgs[0].resualt != m_user->GetPassNum())
			{
				GetContext()->GetUIComponent<Failed>()->Show();
				return false;
			}
			if (BookManager::GetInstance()->ReturnBook(m_user, m_msg->borrowID))
			{
				GetContext()->GetUIComponent<OutTime>()->Show();
				return false;
			}
			BookManager::GetInstance()->SaveBookData();
			UserManager::GetInstance()->SaveUserData();
			GetContext()->GetUIComponent<Succeed>()->Show();
			EditWindow::Reset();
			GetContext()->GetUIComponent<BorrowMenuList>()->Refresh();
			return true;
		}
	};

	class UserMenu :public MenuUI
	{
		UserBorrowMessage *m_msg;
		User *m_user;

	public:
		class HaveReturned :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("����黹", { "��ͼ���Ѿ��黹~" });
			}
		};

		void Show(UserBorrowMessage *msg, User *user)
		{
			GetContext()->SetFocusUI(this);
			m_msg = msg;
			m_user = user;
			Enable();
		}
	protected:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			MenuUI::OnCreate(srcView);
			CreateMenu(
				{
					{"�黹",[&](void *)
					{
						if (m_msg->isDone)
						{
							GetContext()->GetUIComponent<HaveReturned>()->Show();
							return;
						}
						GetContext()->GetUIComponent<ReturnWindow>()->Show(m_msg,m_user); } }
				}, 2, 8, 1, GravityMode::Left);
			Disable();
		}

		virtual void OnDisable()override
		{
			MenuUI::OnDisable();
			GetContext()->SetFocusUI(GetContext()->GetUIComponent<BorrowMenuList>());
			GetContext()->Draw();
		}


		virtual bool OnEvent(char input)override
		{
			switch (input)
			{
			case 'q':
				Disable();
				break;
			default:
				MenuUI::OnEvent(input);
				break;
			}
			return false;
		}
	};

public:
	BorrowMessageSurface() :
		UISurface(DEFAULT_FPS, CONSOLE_WIDTH, CONSOLE_HEIGHT)
	{}

	virtual void OnCreateUI()override
	{
		AddUIComponent(new ManagerSurfaceTitle("������Ϣ"));
		AddUIComponent(new BorrowMenuList());
		AddUIComponent(new UserMenu());
		AddUIComponent(new ReturnWindow());
		AddUIComponent(new UserMenu::HaveReturned());
		AddUIComponent(new ReturnWindow::Succeed());
		AddUIComponent(new ReturnWindow::Failed());
		AddUIComponent(new ReturnWindow::OutTime());
	}

	virtual void OnCreate()override
	{
		SetFocusUI(GetUIComponent<BorrowMenuList>());
	}

	virtual void OnEnter(void *user)override
	{
		GetUIComponent<BorrowMenuList>()->SetUser((User*)user);
		GetUIComponent<BorrowMenuList>()->Refresh();
		UISurface::OnEnter();
	}
};
#endif