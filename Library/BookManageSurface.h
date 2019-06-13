#ifndef BOOKMANAGERSURFACE_H
#define BOOKMANAGERSURFACE_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Value.h"
#include "UIComponent.h"
#include "UISurface.h"
#include "MenuUI.h"
#include "TipsUI.h"
#include "Title.h"
#include "EditWindow.h"
#include "BookManager.h"

class BookManageSurface : public UISurface
{
	public:
	class ListMenuTips :public TipsUI
		{
		public:
			virtual void OnCreate(const ViewRect &view)override
			{
				TipsUI::OnCreate(view);
				CreateTips("操作提示(Tips)",
					{
						"除了其他操作外，按下e可以切换到功能目录",
					});
			}
		};

	class BookMenuList :public MenuUI
	{
		bool m_isUser;
	public:
		void Refresh()
		{
			std::vector<MenuItem> bookItems;
			BookManager::GetInstance()->ForeachBooks([&](BookMessage &msg)
			{
				MenuItem item;
				std::ostringstream oss;
				oss.setf(std::ios::left);
				oss << "ID:" << std::setw(10) << msg.ID << std::setw(15) << msg.name << "作者:" << std::setw(12) << msg.author << "出版商:" << std::setw(15) << msg.publisher;
				oss.setf(std::ios::right);
				oss << std::setw(4) << msg.totalNum - msg.borrowMsg.Size() << '/' << msg.totalNum;
				item.name = oss.str();
				item.data = &msg;
				item.Event = [&](void *data)
				{
					if(!m_isUser)
						GetContext()->GetUIComponent<AdminMenu>()->Show((BookMessage*)data);
					else
						GetContext()->GetUIComponent<UserMenu>()->Show((BookMessage*)data);
				};
				bookItems.push_back(std::move(item));
			});
			CreateMenu(bookItems, 40, 2, 0, CONSOLE_HEIGHT - 3, GravityMode::Left, true);
		}

		BookMenuList(bool isUser)
		{
			m_isUser = isUser;
		}

		virtual void OnReset()override
		{
			Refresh();
		}

		virtual bool OnEvent(char input)override
		{
			switch (input)
			{
			case 'e':
				if (!m_isUser)
					GetContext()->GetUIComponent<AuxiliaryAdminMenu>()->Show();
				break;
			default:
				MenuUI::OnEvent(input);
				break;
			}
			return false;
		}
	};

	/*Admin*/
	class AddBookWindow :public EditWindow
	{
	public:
		class Succeed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("添加成功", { "书籍录入完成！" });
			}
		};
		class RepeatedNum :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("添加失败", { "已经存在相同编号的书籍！" });
			}
		};
		class Empty :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("添加失败", { "某项信息为空！" });
			}
		};
	private:

		void OnCreate(const ViewRect &view)
		{
			EditWindow::OnCreate(view);
			CreateEditor(45, 5,
				{
					{"ID","只能为英文,数字,下划线，并且不能与现有图书重复",true,true,[](char input) {return input >= 0 && (isalnum(input) || input == '_'); }},
					{"图书名称","",false,true,[](char input) {return true; }},
					{"作者","",false,true,[](char input) {return true; }},
					{"出版商","",false,true,[](char input) {return true; }},
					{"书籍数量","",false,true,[](char input) {return input >= 0 && isdigit(input); }}
				});
		}

		virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
		{
			if (BookManager::GetInstance()->GetBookByID(msgs[0].resualt))
			{
				GetContext()->GetUIComponent<RepeatedNum>()->Show();
				return false;
			}
			if (msgs[0].resualt.empty()||msgs[1].resualt.empty()||msgs[2].resualt.empty()||msgs[3].resualt.empty()||msgs[4].resualt.empty())
			{
				GetContext()->GetUIComponent<Empty>()->Show();
				return false;
			}
			BookManager::GetInstance()->AddBook({ msgs[0].resualt,msgs[1].resualt,msgs[2].resualt,msgs[3].resualt,std::stoi(msgs[4].resualt) });
			BookManager::GetInstance()->SaveBookData();
			GetContext()->GetUIComponent<Succeed>()->Show();
			EditWindow::Reset();
			GetContext()->GetUIComponent<BookMenuList>()->Refresh();
			return true;
		}
	};

	class DelBookWindow :public EditWindow
	{
		BookMessage *m_msg;
	public:
		class Succeed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("移除成功", { "书籍删除成功！" });
			}
		};
		class Failed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("移除失败", { "验证码输入错误！" });
			}
		};

		void Show(BookMessage *msg)
		{
			m_msg = msg;
			EditWindow::Show();
			m_isDelSucceed = false;
		}
	private:
		bool m_isDelSucceed = false;
		void OnCreate(const ViewRect &view)
		{
			EditWindow::OnCreate(view);
			CreateEditor(45, 5,
				{
					{"验证码","请输入\"Del\"来删除该图书",true,true,[](char input) {return true; }}
				});
		}

		virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
		{
			if (msgs[0].resualt!="Del")
			{
				GetContext()->GetUIComponent<Failed>()->Show();
				return false;
			}
			m_isDelSucceed = true;
			BookManager::GetInstance()->RemoveBookByID(m_msg->ID);
			BookManager::GetInstance()->SaveBookData();
			GetContext()->GetUIComponent<Succeed>()->Show();
			GetContext()->GetUIComponent<AdminMenu>()->Disable();
			GetContext()->GetUIComponent<BookMenuList>()->Refresh();
			return true;
		}

		virtual void OnDisable()override
		{
			if (!m_isDelSucceed)
				EditWindow::OnDisable();
			else
				GetContext()->Draw();
		}
	};
	class SetBookWindow :public EditWindow
	{
		BookMessage *m_msg;
	public:
		class Succeed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("修改成功", { "书籍信息修改成功！" });
			}
		};
		class Empty :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("修改失败", { "某项信息为空！" });
			}
		};
		class TooLowReapeted :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("修改失败", { "设定数量小于已借阅数量！" });
			}
		};

		void Show(BookMessage *msg)
		{
			m_msg = msg;
			EditWindow::Show();
		}
	private:
		void OnCreate(const ViewRect &view)
		{
			EditWindow::OnCreate(view);
			CreateEditor(45, 5,
				{
					{"图书名称","名称",true,true,[](char input) {return true; }},
					{"作者","作者",true,true,[](char input) {return true; }},
					{"出版商","出版商",true,true,[](char input) {return true; }},
					{"书籍数量","数量",true,true,[](char input) {return input >= 0 && isdigit(input); }}
				});
		}

		virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
		{
			int totalNum = (msgs[3].resualt.empty() ? m_msg->totalNum : std::stoi(msgs[3].resualt));
			if (totalNum < m_msg->borrowMsg.Size())
			{
				GetContext()->GetUIComponent<TooLowReapeted>()->Show();
				return false;
			}
			m_msg->name = msgs[0].resualt;
			m_msg->author = msgs[1].resualt;
			m_msg->publisher = msgs[2].resualt;
			m_msg->totalNum = totalNum;
			BookManager::GetInstance()->SaveBookData();
			GetContext()->GetUIComponent<Succeed>()->Show();
			EditWindow::Reset();
			GetContext()->GetUIComponent<BookMenuList>()->Refresh();
			return true;
		}
	};

	class AuxiliaryAdminMenu :public MenuUI
	{
	public:
		void Show()
		{
			GetContext()->SetFocusUI(this);
			Enable();
		}
	protected:

		virtual void OnCreate(const ViewRect &srcView)override
		{
			MenuUI::OnCreate(srcView);
			CreateMenu(
				{
					{"添加图书",[&](void *) {GetContext()->GetUIComponent<AddBookWindow>()->Show(); } }
				}, 2, 2, 1, GravityMode::Left);
			Disable();
		}

		virtual void OnDisable()override
		{
			MenuUI::OnDisable();
			GetContext()->SetFocusUI(GetContext()->GetUIComponent<BookMenuList>());
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
    class AdminMenu :public MenuUI
	{
		BookMessage *m_msg;
	public:
	void Show(BookMessage *msg)
	{
		m_msg = msg;
		GetContext()->SetFocusUI(this);
		Enable();
	}
	protected:

		virtual void OnCreate(const ViewRect &srcView)override
		{
			MenuUI::OnCreate(srcView);
			CreateMenu(
				{
					{"删除图书",[&](void *) {GetContext()->GetUIComponent<DelBookWindow>()->Show(m_msg); } },
					{"修改信息",[&](void *) {GetContext()->GetUIComponent<SetBookWindow>()->Show(m_msg); } }
				}, 2, 15, 1, GravityMode::Left);
			Disable();
		}

		virtual void OnDisable()override
		{
			MenuUI::OnDisable();
			GetContext()->SetFocusUI(GetContext()->GetUIComponent<BookMenuList>());
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

	/*User*/
	class BorrowBookWindow :public EditWindow
	{
		BookMessage *m_msg;
	public:
		class Succeed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("借阅成功", { "恭喜你，书籍借阅成功！" });
			}
		};
		class Failed :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("借阅失败", { "证件号码输入错误！" });
			}
		};
		class DataError :public TipsUI
		{
			virtual void OnCreate(const ViewRect &srcView)override
			{
				TipsUI::OnCreate(srcView);
				CreateTips("借阅失败", { "日期输入异常！" });
			}
		};

		void Show(BookMessage *msg)
		{
			m_msg = msg;
			EditWindow::Show();
		}
	protected:
		virtual void OnCreate(const ViewRect &view)override
		{
			EditWindow::OnCreate(view);
			CreateEditor(45, 5,
				{
					{"证件号码","请输入证件号码",true,true,[](char input) {return true; }},
					{"归还天数","请输入需要借阅多少天",true,true,[](char input) {return input >= 0 && isdigit(input); },"30"}
				});
		}

		virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs)override
		{
			if (msgs[0].resualt != ((User*)UserManager::GetInstance()->GetLoginUser())->GetPassNum())
			{
				GetContext()->GetUIComponent<Failed>()->Show();
				return false;
			}
			int days = 30;
			if (!msgs[1].resualt.empty())
				days = std::stoi(msgs[1].resualt);
			if (days <= 0)
			{
				GetContext()->GetUIComponent<DataError>()->Show();
				return false;
			}
			TimeDate tm;
			tm.AddTime(30, 0, 0, days);
			BookManager::GetInstance()->BorrowBook((User*)UserManager::GetInstance()->GetLoginUser(), m_msg->ID, tm);
			BookManager::GetInstance()->SaveBookData();
			GetContext()->GetUIComponent<Succeed>()->Show();
			EditWindow::Reset();
			GetContext()->GetUIComponent<BookMenuList>()->Refresh();
			return true;
		}
	};
	class UserMenu :public MenuUI
	{
		BookMessage *m_msg;
	public:
		void Show(BookMessage *msg)
		{
			m_msg = msg;
			GetContext()->SetFocusUI(this);
			Enable();
		}
	protected:
		virtual void OnCreate(const ViewRect &srcView)override
		{
			MenuUI::OnCreate(srcView);
			CreateMenu(
				{
					{"借阅图书",[&](void *) {GetContext()->GetUIComponent<BorrowBookWindow>()->Show(m_msg); } },
				}, 2, 8, 1, GravityMode::Left);
			Disable();
		}

		virtual void OnDisable()override
		{
			MenuUI::OnDisable();
			GetContext()->SetFocusUI(GetContext()->GetUIComponent<BookMenuList>());
		}

		virtual bool OnEvent(char input)override
		{
			switch (input)
			{
			case 'q':
				Disable();
				GetContext()->Draw();
				break;
			default:
				MenuUI::OnEvent(input);
				break;
			}
			return false;
		}
	};

public:
	BookManageSurface(bool isUser) :
		UISurface(DEFAULT_FPS, CONSOLE_WIDTH, CONSOLE_HEIGHT), m_isUser(isUser)
	{}

	virtual void OnCreateUI()override
	{
		if (m_isUser)
		{
			AddUIComponent(new ManagerSurfaceTitle("阅览图书信息"));
			AddUIComponent(new BookMenuList(true));
			AddUIComponent(new UserMenu());
			AddUIComponent(new BorrowBookWindow());
			AddUIComponent(new BorrowBookWindow::Succeed());
			AddUIComponent(new BorrowBookWindow::Failed());
			AddUIComponent(new BorrowBookWindow::DataError());
		}
		else
		{
			AddUIComponent(new ManagerSurfaceTitle("管理图书信息"));
			AddUIComponent(new BookMenuList(false));
			AddUIComponent(new AuxiliaryAdminMenu());
			AddUIComponent(new AdminMenu());
			AddUIComponent(new AddBookWindow());
			AddUIComponent(new DelBookWindow());
			AddUIComponent(new SetBookWindow());
			AddUIComponent(new AddBookWindow::Succeed());
			AddUIComponent(new AddBookWindow::RepeatedNum());
			AddUIComponent(new AddBookWindow::Empty());
			AddUIComponent(new DelBookWindow::Succeed());
			AddUIComponent(new DelBookWindow::Failed());
			AddUIComponent(new SetBookWindow::Succeed());
			AddUIComponent(new SetBookWindow::TooLowReapeted());
			AddUIComponent(new ListMenuTips());
		}
	}

	virtual void OnCreate()override
	{
		SetFocusUI(GetUIComponent<BookMenuList>());
	}

	virtual void OnEnter()override
	{
		UISurface::OnEnter();
		if(!m_isUser)
			GetUIComponent<ListMenuTips>()->Show();
	}

private:
	bool m_isUser;
};
#endif