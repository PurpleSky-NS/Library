#ifndef SPLASHSURFACE_H
#define SPLASHSURFACE_H

#include <cmath>
#include "Value.h"
#include "Time.h"
#include "Translate.h"
#include "SurfaceManager.h"
#include "UISurface.h"
#include "UIComponent.h"

class SplashSurface :public UISurface
{
	class Char
	{
	public:
		std::vector<Translate::IVector2<int>> &GetPoints()
		{
			return m_chars;
		}

		/*@Test*/
		void DrawTo(ViewRect &view)
		{
			for (auto i : m_chars)
				view(i.x, i.y) = '*';
		}

	protected:
		void AddPoint(int x, int y)
		{
			m_chars.push_back({ x,y});
		}
	private:
		std::vector<Translate::IVector2<int>> m_chars;
	};

	class Frame :public Char
	{
	public:
		Frame()
		{
			for (int i = 0; i < CONSOLE_WIDTH; ++i)
			{
				AddPoint(i, 0);
				AddPoint(i, CONSOLE_HEIGHT - 1);
			}
			for (int i = 0; i < CONSOLE_HEIGHT; ++i)
			{
				AddPoint(0, i);
				AddPoint(CONSOLE_WIDTH - 1, i);
			}
		}
	};
	
	class Heart :public Char
	{
		const int WIDTH = 40;
		const int HEIGHT = 20;
	public:
		Heart()
		{
			DrawHeart(WIDTH, HEIGHT);
		}

		/*解二阶方程，返回false则代表没有实数解*/
		bool CalcSOE(double a, double b, double c, double &x1, double &x2)
		{
			double delta = b * b - 4 * a*c;
			if (delta < 0.0)
				return false;

			x1 = (-b + sqrt(delta)) / 2 / a;
			x2 = (-b - sqrt(delta)) / 2 / a;
			return true;
		}

		/*放缩设置心的像素*/
		void DrawHeart(int w, int h)
		{
			/*心方程为：5x2-6|x|y+5y2=128
			关于y的标准形式为5y2-6|x|y+5x2-128,a=5,b=-6|x|,c=5x2-128
			定义域为[-6.5,6.5],将[0,W]->[-6.5,6.5]
			则映射函数为：x'=(x-W/2)*13/W
			值域为[-5,6.35]，将[-5,6.35]->[0,H]
			则映射函数为：y=H-(y'+5)*H/11.35
			*/

			int y1, y2;//映射的解
			double _y1, _y2;//原方程的解
			double _x;//映射出来的x
			const int offX = (CONSOLE_WIDTH - WIDTH) / 2 + 1;
			const int offY = (CONSOLE_HEIGHT - HEIGHT) / 2 + 1; 
			for (int x = 0; x < w; ++x)
			{
				_x = (x - w / 2.0)*13.0 / w;
				if (CalcSOE(5.0, -6.0*abs(_x), 5 * _x*_x - 128, _y1, _y2))
				{
					y1 = (int)((h - (_y1 + 5.0)*h / 11.35) + 0.5);//+0.5四舍五入
					y2 = (int)((h - (_y2 + 5.0)*h / 11.35) + 0.5);
					AddPoint(x + offX, y1 + offY);
					AddPoint(x + offX, y2 + offY);
				}
			}
		}

	};


public:
	SplashSurface() :
		UISurface(12, CONSOLE_WIDTH, CONSOLE_HEIGHT) {}

private:

	enum AnimationState
	{
		FrameToHeart,
		Stay,
		HeartBurst,
		Done
	};

	virtual void OnCreate()override
	{
		Char *heart = new Heart();
		Char *frame = new Frame();
		m_lines = Translate::CalcTranslate(frame->GetPoints(), heart->GetPoints(), frame->GetPoints());
		const int cenX = CONSOLE_WIDTH / 2, cenY = CONSOLE_HEIGHT / 2;
		float nX, nY;
		for (auto i : m_lines)
		{
			nX = (float)i.to.x - cenX;
			nY = (float)i.to.y - cenY;
			m_speed.push_back({ nX / sqrt(nX*nX + nY * nY)*burstSpeed,nY / sqrt(nX*nX + nY * nY)*burstSpeed });
		}
	}

	void UpdateState()
	{
		switch (m_animState)
		{
		case SplashSurface::FrameToHeart:
			if (m_passTime > animTime)
			{
				m_passTime -= animTime;
				m_animState = Stay;
			}
			break;
		case SplashSurface::Stay:
			if (m_passTime > stayTime)
			{
				m_passTime -= stayTime;
				m_animState = HeartBurst;
			}
			break;
		case SplashSurface::HeartBurst:
			if (m_passTime > burstTime)
			{
				m_passTime -= burstTime;
				m_animState = Done;
			}
			break;
		}
	}

	virtual void OnUpdate()override
	{
		ViewRect &view = *GetView();
		view.Reset();
		system("cls");
		switch (m_animState)
		{
		case SplashSurface::FrameToHeart:
			for (auto i : m_lines)
				view(int(i.from.x + (i.to.x - i.from.x)*m_passTime / animTime), int(i.from.y + (i.to.y - i.from.y)*m_passTime / animTime)) = '*';
			break;
		case SplashSurface::Stay:
			for (auto i : m_lines)
				view(i.to.x, i.to.y) = '*';
			break;
		case SplashSurface::HeartBurst:
			for (size_t i = 0; i < m_lines.size(); ++i)
			{
				int x = int(m_lines[i].to.x + m_speed[i].x*m_passTime);
				int y = int(m_lines[i].to.y + m_speed[i].y*m_passTime);
				if (x >= 0 && x < CONSOLE_WIDTH&&y >= 0 && y < CONSOLE_HEIGHT)
					view(x, y) = '*';
			}
			break;
		case SplashSurface::Done:
			SurfaceManager::GetInstance()->RestartActiveSurface("MainSurface");
			break;
		}
		puts(GetView()->GetViewData());
		m_passTime += Time::GetDeltaTime();
		UpdateState();
	}

	virtual void OnEvent(char input)override
	{
		if (input == ' ')
			m_animState = Done;
	}

private:
	std::vector<Translate::MoveLine<Translate::IVector2<int>>> m_lines;
	AnimationState m_animState = FrameToHeart;
	const float animTime = 3.0f;
	const float stayTime = 2.0f;
	const float burstTime = 2.5f;
	const float burstSpeed = 30;
	std::vector<Translate::IVector2<float>> m_speed;
	float m_passTime = 0;
};

#endif