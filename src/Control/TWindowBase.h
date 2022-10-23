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
��Ϣ�ַ�ԭ��

��̬����StaticProcΪ��Ϣ����ڣ���GetWndClassEx����wc���á�
����ClassName���Ǳ����֣�static, button�ȣ����򴰿���Ϣ������ָ��StaticProc��

����Create�󣬴�����CreateWindowEx�����ɹ����������ַͨ�����һ������lParam���롣
��StaticProc�����н��յ�WM_NCCREATE��Ϣ����ʱlParam�п�ȡ��lpCreateParams���õ����ַ��
ʹ��SetWindowLongϵ�к�������HWND��ӦGWL_USERDATA������Ϊ���ַ��

��������Ϣ��ͨ��GetWindowLongȡ����ת����WndProc��
*/

class TWindowBase
{
private:
	bool m_bDeleted;
	HICON m_hTitleIcon;//ͼ��
	bool m_bDoubleBuffer;//˫����
	bool m_bDrawSelf;

	//�����Զ�����
	std::unordered_map<TWindowBase *, bool> childs;

	//�����¼������ص�
	//����ֵ��0������Ĭ�ϴ��� LRESULT(*)(TWindowBase* pWin,WPARAM wParam, LPARAM lParam)
	using PMsgDealer = std::function<LRESULT(TWindowBase *, UINT, WPARAM, LPARAM)>;

	//ע���Ӧ�����Ĵ����࣬ע��ʧ���׳��쳣
	void MyRegisterClass(LPCTSTR lpszClass);

	//��̬��Ϣ������
	static LRESULT CALLBACK StaticProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND m_hWnd;
	TWindowBase *m_pParent;
	HINSTANCE m_hInst;
	HACCEL m_hAccelTable;//��ݼ�
	HFONT m_hFont;
	RECT ClientRect;//��WM_PAINT���Զ�����
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

	//����Ϣ�ַ�
	virtual LRESULT WndProc(WNDPROC defProc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnCreate(CREATESTRUCT *pCS) {	}
	virtual bool OnDraw(HDC hdc) { assert(m_bDrawSelf); return true; }

	virtual void OnSetFocus() {}
	virtual void OnKillFocus() {}

	virtual bool OnClose() { return true; }//����true��˳���˳�������false����˳���Ϊ
	virtual void OnSize(WPARAM wParam, LPARAM lParam) {	}

public:
	TWindowBase(const TWindowBase &other) = delete;
	void operator=(const TWindowBase &other) = delete;
	virtual ~TWindowBase();

	//��������
	virtual void Create(LPCTSTR lpszName, LPCTSTR lpszClass, DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		TWindowBase *pParent = nullptr, HMENU hMenu = 0);


	void LoadTitleIcon(UINT id);//���ڴ�������ͼ��
	void SetAccel(UINT id);//�����ݼ���Դ

	void SetDoubleBuffer(bool bDoubleBuffer = true);
	void SetDrawSelf(bool bDrawSelf = true);

	//ע���Զ�����Ϣ����
	void RegisterMessage(UINT uMsg, PMsgDealer pFun);

	HWND GetHWND();

	void Invalidate();//�ػ洰��
	bool ShowWindow(int nCmdShow = SW_SHOW) const;

	bool UpdateWindow() const;

	RECT GetClientRect();
	RECT GetWindowRect();

	//��������ڸ����ڵ�λ��
	POINT GetPosition(); 

	//����λ��
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
	int GetLength() const;//��ȡ�ַ�������	

	void SetVisible(bool bShow);//���ÿɼ���
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
//����OpenGL API
HGLRC gl_create(HDC hdc);
//��ȡ������OpenGL���Ƶ�DC
//https://blog.csdn.net/hhhhhhhhhhkkkkkkkkkk/article/details/103600876#comments_13890857
HDC win32_get_gl_dc(HWND hwnd);
#endif