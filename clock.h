typedef LRESULT (*FXN)(HWND , UINT , WPARAM , LPARAM);

typedef struct tagFuncList{
	UINT msg;
	FXN fun;
}FuncList;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return 0;
}

class LiteWnd
{
public:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	const static FuncList ProcMap[];
public:
	LiteWnd(void);
	~LiteWnd(void);
	void Create(void);
	void Show(void);
	void Update(void);
	MY_DECLARED_MESSAGE
	static LRESULT CALLBACK WinProc(HWND , UINT , WPARAM , LPARAM);
};

MY_MESSAGE_MAP

LRESULT CALLBACK LiteWnd::WinProc(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	for(int i=0 ; i<Dim(ProcMap) ; ++i)
	{
		if(ProcMap[i].msg == uMsg)
		{
			FXN f = ProcMap[i].fun;
			return f(hWnd , uMsg , wParam , lParam);
		}
	}
	return DefWindowProc(hWnd , uMsg , wParam , lParam);
}

LiteWnd::LiteWnd(void)
{
	m_hInstance = NULL;
	m_hWnd = NULL;
}


LiteWnd::~LiteWnd(void)
{
}

void LiteWnd::Create(void)
{
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL , IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL , IDI_APPLICATION);
	WndClass.hInstance = m_hInstance;
	WndClass.lpfnWndProc = WinProc;
	WndClass.lpszClassName = "LiteWindow";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	m_hWnd = CreateWindow(WndClass.lpszClassName ,\
		"MainMenu" , WS_POPUP , 200 , 100 , 800 , 600 ,\
		NULL , NULL , WndClass.hInstance , NULL);
}

void LiteWnd::Show(void)
{
	ShowWindow(m_hWnd , SW_SHOW);
}

void LiteWnd::Update(void)
{
	UpdateWindow(m_hWnd);
}

class LiteApp
{
public:
	LiteWnd * m_pLiteWnd;
public:
	LiteApp(void);
	~LiteApp(void);
	void InitInstance(void);
	void Run(void);
};

LiteApp::LiteApp(void)
{
	InitInstance();
	Run();
}


LiteApp::~LiteApp(void)
{
}

void LiteApp::InitInstance(void)
{
	m_pLiteWnd = new LiteWnd;
	m_pLiteWnd->Create();
	m_pLiteWnd->Show();
	m_pLiteWnd->Update();
}

void LiteApp::Run(void)
{
	MSG msg;
	while(GetMessage(&msg , NULL , 0 , 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
