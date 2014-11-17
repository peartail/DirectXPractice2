#include "SystemClass.h"


SystemClass::SystemClass()
{
	m_Input = NULL;
	m_Graphics = NULL;

	_fps = NULL;
	_cpu = NULL;
	_timer = NULL;
	_position = NULL;

	_sound = NULL; 
}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"C n i t i o ", L"E", MB_OK);
		return false;
	}

	m_Graphics = new GraphicClass;
	if (!m_Graphics)
	{
		return false;
	}

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	_sound = new SoundClass;
	if (!_sound)
	{
		return false;
	}

	result = _sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"CN Sound", L"Err", MB_OK);
		return false;
	}

	_fps = new FpsClass;
	if (!_fps)
	{
		return false;
	}

	_fps->Initialize();

	_cpu = new CpuClass;
	if (!_cpu)
	{
		return false;
	}

	_cpu->Initialize();

	_timer = new TimerClass;
	if (!_timer)
	{
		return false;
	}

	result = _timer->Initialize();

	if (!result)
	{
		MessageBox(m_hwnd, L"N timer", L"ER", MB_OK);
		return false;
	}

	_position = new PositionClass;
	if (!_position)
	{
		return false;
	}

	return true;
}

void SystemClass::ShutDown()
{
	if (_position)
	{
		delete _position;
		_position = NULL;
	}


	if (_timer)
	{
		delete _timer;
		_timer = NULL;
	}

	if (_cpu)
	{
		_cpu->Shutdown();
		delete _cpu;
		_cpu = NULL;
	}

	if (_fps)
	{
		delete _fps;
		_fps = NULL;
	}

	if (_sound)
	{
		_sound->Shutdown();
		delete _sound;
		_sound = NULL;
	}

	if (m_Graphics)
	{
		m_Graphics->ShutDown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

		if (m_Input->IsEscapePressed())
		{
			done = true;
		}

	}
}

bool SystemClass::Frame()
{
	bool keydown,result;
	float rotationY;
	int mouseX, mouseY;

	_timer->Frame();
	_fps->Frame();
	_cpu->Frame();

	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	D3DXVECTOR3 rot(0.0f, 0.0f, 0.0f);

	_position->SetFrameTime(_timer->GetTime());

	keydown = m_Input->IsLeftArrowPressed();
	_position->TurnLeft(keydown);

	keydown = m_Input->IsRightArrowPressed();
	_position->TurnRight(keydown);

	_position->GetRotation(rot.y);

	keydown = m_Input->IsFronArrowPressed();
	_position->TurnFront(keydown);

	keydown = m_Input->IsBackendArrowPressed();
	_position->TurnBack(keydown);

	_position->GetPositionZ(rot.z);
	_position->GetPositionX(rot.x);

	m_Input->GetMouseLocation(mouseX, mouseY);

	//result = m_Graphics->Frame(rot, mouseX, mouseY, _fps->GetFps(), _cpu->GetCpuPercentage(), _timer->GetTime());
	result = m_Graphics->Frame(rot);
	//result = m_Graphics->Frame(_timer->GetTime());
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MEssageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posx, posy;

	//외부 포인터 설정
	ApplicationHandle = this;

	//어플 인스턴스
	m_hinstance = GetModuleHandle(NULL);

	//어플 이름
	m_applicationName = L"TEST";

	//윈도우 클래스 설정
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//윈도우 클래스 등록
	RegisterClassEx(&wc);

	//모니터 화면 해상도
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//풀스크린 설정
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posx = posy = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posx = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posx, posy, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
					   return 0;
	}
	case WM_CLOSE:
	{
					 PostQuitMessage(0);
					 return 0;
	}
	default:
	{
			   return ApplicationHandle->MEssageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}