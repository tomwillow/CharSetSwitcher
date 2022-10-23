#pragma once
#include "ControlStyle.h"
#include <Windows.h>
#include "tstring.h"
#include <assert.h>
#include <functional>
#include <unordered_map>
#include <list>

#ifdef _OPENGL
#include<GL/GL.h>
#pragma comment(lib,"opengl32.lib")
#endif

/*
消息分发原理：

静态函数StaticProc为消息总入口，由GetWndClassEx中向wc设置。
窗口ClassName若非保留字（static, button等），则窗口消息处理函数指向StaticProc。

调用Create后，窗口由CreateWindowEx创建成功，并将类地址通过最后一个参数lParam传入。
在StaticProc函数中接收到WM_NCCREATE消息，此时lParam中可取出lpCreateParams，得到类地址。
使用SetWindowLong系列函数，将HWND对应GWL_USERDATA参数绑定为类地址。

在其他消息中通过GetWindowLong取出，转发至WndProc。
*/

class TWindowBase
{
private:
	bool m_bDeleted;
	HICON m_hTitleIcon;//图标
	bool m_bDoubleBuffer;//双缓冲
	bool m_bDrawSelf;

	//用于自动销毁
	std::unordered_map<TWindowBase *, bool> childs;

	//用于事件监听回调
	//返回值：0不启用默认处理 LRESULT(*)(TWindowBase* pWin,WPARAM wParam, LPARAM lParam)
	using PMsgDealer = std::function<LRESULT(TWindowBase *, UINT, WPARAM, LPARAM)>;

	//注册对应类名的窗口类，注册失败抛出异常
	void MyRegisterClass(LPCTSTR lpszClass);

	//静态消息处理函数
	static LRESULT CALLBACK StaticProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND m_hWnd;
	TWindowBase *m_pParent;
	HINSTANCE m_hInst;
	HACCEL m_hAccelTable;//快捷键
	HFONT m_hFont;
	RECT ClientRect;//在WM_PAINT中自动更新
	RECT WindowRect;

	std::unordered_map<UINT, PMsgDealer > msgDealer;//key:uMsg; value:PMsgDealer

#ifdef _OPENGL
	HDC glhdc;
	HGLRC hglrc;
	static float fps;
	bool isFocus;
	virtual void Render() {}
#endif

	TWindowBase(HINSTANCE hInst, TWindowBase *_parent = nullptr) :m_hInst(hInst), m_pParent(_parent), m_hWnd(0), m_bDoubleBuffer(false),
		m_hTitleIcon(0), m_hAccelTable(0), ClientRect(), WindowRect(), m_hFont(0), m_bDrawSelf(false), m_bDeleted(false)
#ifdef _OPENGL
		, isFocus(true), glhdc(0), hglrc(0)
#endif
	{
		if (_parent)
			_parent->childs[this] = true;
	}

	TWindowBase(TWindowBase *_parent = nullptr) :TWindowBase(GetModuleHandle(0), _parent)
	{
		if (_parent)
			_parent->childs[this] = true;
	}

	//主消息分发
	virtual LRESULT WndProc(WNDPROC defProc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnCreate(CREATESTRUCT *pCS) {	}
	virtual bool OnDraw(HDC hdc) { assert(m_bDrawSelf); return true; }

	virtual void OnSetFocus() {}
	virtual void OnKillFocus() {}

	virtual bool OnClose() { return true; }//返回true则顺利退出，返回false阻断退出行为
	virtual void OnSize(WPARAM wParam, LPARAM lParam) {	}

public:
	TWindowBase(const TWindowBase &other) = delete;
	void operator=(const TWindowBase &other) = delete;
	virtual ~TWindowBase();

	//创建窗口
	virtual void Create(LPCTSTR lpszName, LPCTSTR lpszClass, DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		TWindowBase *pParent = nullptr, HMENU hMenu = 0);


	void LoadTitleIcon(UINT id);//先于窗口载入图标
	void SetAccel(UINT id);//载入快捷键资源

	void SetDoubleBuffer(bool bDoubleBuffer = true);
	void SetDrawSelf(bool bDrawSelf = true);

	//注册自定义消息函数
	void RegisterMessage(UINT uMsg, PMsgDealer pFun);

	HWND GetHWND();

	void Invalidate();//重绘窗口
	bool ShowWindow(int nCmdShow = SW_SHOW) const;

	bool UpdateWindow() const;

	RECT GetClientRect();
	RECT GetWindowRect();

	//返回相对于父窗口的位置
	POINT GetPosition(); 

	//设置位置
	void SetPosition(int x, int y);

	POINT GetSize();
	void SetSize(POINT sz);
	void SetHeight(int height);
	void SetWidth(int width);

	void SetFont(HFONT hFont);
	HFONT GetFont();
	void SetDefaultGuiFont();
	void SetFont(TCHAR FontName[], int FontSize);

	void SetText(const std::tstring &s);
	void GetText(TCHAR text[]);
	std::tstring GetText() const;
	int GetLength() const;//获取字符串长度	

	void SetVisible(bool bShow);//设置可见性
	bool GetVisible();

	virtual void SetEnable(bool bEnable);
	virtual bool GetEnable();

	void SetFocus() const;

	bool HasStyleMark(LONG style) const;
	void ClearStyleMark(LONG style) const;
	void SetStyleMark(LONG style) const;

	void SetDouble(double d);

	friend class TControl;
};


int GetFontHeight(HFONT hFont);

#ifdef _OPENGL
//创建OpenGL API
HGLRC gl_create(HDC hdc);
//获取可用于OpenGL绘制的DC
//https://blog.csdn.net/hhhhhhhhhhkkkkkkkkkk/article/details/103600876#comments_13890857
HDC win32_get_gl_dc(HWND hwnd);
#endif