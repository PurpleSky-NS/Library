#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <vector>
#include <string>
#include <cctype>
#include <functional>
#include "SurfaceManager.h"
#include "UIComponent.h"

/*�༭����UI*/
class EditWindow :public UIComponent
{
public:

	void Reset()
	{
		for (auto &i : m_msgs)
			i.resualt.clear();
	}

	void Show()
	{
		Enable(); 
		m_backFocus = GetContext()->GetFocusUI();
		GetContext()->SetFocusUI(this);
	}

protected:

	struct EditDataMsg
	{
		std::string name;
		std::string tips;
		bool hasTips;
		bool isVisitable;
		std::function<bool(char)> TestInput;
		std::string resualt;
	};

	/*�༭��ɣ������Ƿ��˳���Ӧ��д*/
	virtual bool OnEditFinish(const std::vector<EditDataMsg> &msgs) { return true; };

	void OnCreate(const ViewRect &srcView)override
	{
		m_markChangeTimer.SetUpadateTime(m_markChangeTime);
		m_markChangeTimer.AddEvent([&]()
		{
			m_mark = (m_mark == '_' ? ' ' : '_');
		});
		m_markChangeTimer.Run();
		m_srcView.SetViewRect(srcView);
	}

	/*Oncreate���ã������������*/
	void CreateEditor(int x, int y,const std::vector<EditDataMsg> &msgs)
	{
		int maxW = 0;
		for (auto i : msgs)
		{
			int tipsLineSize = i.tips.size() + 5;//frame:4+'#'
			int dataLineSize = i.name.size() + i.resualt.size() + 14;//" : "+mark+frame+6byte�հ�
			if (tipsLineSize > maxW)
				maxW = tipsLineSize;
			if (dataLineSize > maxW)
				maxW = dataLineSize;
		}	
		m_view.SetViewRect(m_srcView, { x,y,maxW,((int)msgs.size() + 1) * 3 });
		m_msgs = msgs;
		Disable();
	}

	virtual void OnDraw()
	{
		if (m_msgs.empty())
			return;
		for (int i = 1; i < m_view.GetCutRect().h - 1; ++i)
			m_view.FillLine(1, 1, i, ' ');
		for (size_t i = 0; i < m_msgs.size(); ++i)
			if (i == m_edit)
				DrawEditData(i);
			else
				DrawNormalData(i);
		DrawFrame(m_view, '*');
	}

	virtual void OnEnable()override
	{
		m_edit = 0;
		m_markChangeTimer.Run();
	}

	virtual void OnDisable()override
	{
		if (m_backFocus != nullptr)
		{
			GetContext()->SetFocusUI(m_backFocus);
			GetContext()->Draw();
		}
	}

	virtual void OnUpdate()
	{
		m_markChangeTimer.Update();
		DrawEditData(m_edit);
	}

	virtual bool OnEvent(char input)
	{
		if (m_msgs.empty())
			return true;
		std::string &editStr = m_msgs[m_edit].resualt;
		switch (input)
		{
		case '<':
			Disable();
			break;
		case '\r':
			if (OnEditFinish(m_msgs))
				Disable();
			break;
		case '\b':
			if (editStr.size() > 0)
				if (editStr[editStr.size() - 1] > 0)
					editStr.erase(editStr.size() - 1, 1);
				else
					editStr.erase(editStr.size() - 2, 2);
			break;
		case '\t':
			DrawNormalData(m_edit);
			m_edit = (m_edit + 1) % m_msgs.size();
			break;
		default:
			if (m_msgs[m_edit].TestInput(input))
				m_msgs[m_edit].resualt += input;
			break;
		}
		return false;
	}
	
private:
	ViewRect m_srcView, m_view;
	std::vector<EditDataMsg> m_msgs;
	int m_edit = 0;
	char m_mark = '_';
	const float m_markChangeTime = 0.8f;
	Timer m_markChangeTimer;
	UIComponent *m_backFocus = nullptr;

	void DrawEditData(int index)
	{
		size_t showStrSize = m_view.GetCutRect().w - m_msgs[index].name.size() - 8;
		std::string data;
		if (m_msgs[index].isVisitable)
			data = m_msgs[index].name + " : " +
			m_msgs[index].resualt.substr((m_msgs[index].resualt.size() > showStrSize ? m_msgs[index].resualt.size() - showStrSize : 0), (m_msgs[index].resualt.size() > showStrSize ? showStrSize : m_msgs[index].resualt.size()))
			+ m_mark;
		else
			data = m_msgs[index].name + " : " + (m_msgs[index].resualt.size() > showStrSize ? std::string(showStrSize, '*') : std::string(m_msgs[index].resualt.size(), '*')) + m_mark;
		m_view.FillLine(1, 1, index * 3 + 2, ' ');
		m_view.SetText(2, index * 3 + 2, data);
		if (m_msgs[index].hasTips)
		{
			m_view.FillLine(1, 1, index * 3 + 3, ' ');
			m_view.SetText(2, index * 3 + 3, "#" + m_msgs[index].tips);
		}
		m_view.FillLine(1, 1, index * 3 + 4, ' ');
	}

	void DrawNormalData(int index)
	{
		size_t showStrSize = m_view.GetCutRect().w - m_msgs[index].name.size() - 7;//" : "+frame
		std::string data;
		if (m_msgs[index].isVisitable)
			data = m_msgs[index].name + " : " +
			m_msgs[index].resualt.substr((m_msgs[index].resualt.size() > showStrSize ? m_msgs[index].resualt.size() - showStrSize : 0), (m_msgs[index].resualt.size() > showStrSize ? showStrSize : m_msgs[index].resualt.size()));
		else
			data = m_msgs[index].name + " : " + (m_msgs[index].resualt.size() > showStrSize ? std::string(showStrSize, '*') : std::string(m_msgs[index].resualt.size(), '*'));
		m_view.FillLine(1, 1, index * 3 + 2, ' ');
		m_view.SetText(2, index * 3 + 2, data);
		if (m_msgs[index].hasTips)
		{
			m_view.FillLine(1, 1, index * 3 + 3, ' ');
			m_view.SetText(2, index * 3 + 3, "#" + m_msgs[index].tips);
		}
		m_view.FillLine(1, 1, index * 3 + 4, ' ');
	}
};
#endif