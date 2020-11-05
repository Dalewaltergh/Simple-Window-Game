#include "utility.cpp"
#include <windows.h>

static bool running = true;

struct RenderState {
	int height, width;
	void* memory;

	BITMAPINFO bitmapInfo;
};

RenderState renderState;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		} break;
		
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderState.width = rect.right - rect.left;
			renderState.height = rect.bottom - rect.top;

			int size = renderState.width * renderState.height * sizeof(unsigned int);

			if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
			renderState.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
			renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
			renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
			renderState.bitmapInfo.bmiHeader.biPlanes = 1;
			renderState.bitmapInfo.bmiHeader.biBitCount = 32;
			renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;

		} break;

		default: 
		{
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	ShowCursor(false);

	// CREATE WINDOW CLASS
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = "Game Window Class";
	windowClass.lpfnWndProc = windowCallback;

	// REGISTER CLASS
	RegisterClass(&windowClass);

	// CREATE WINDOW
	HWND window = CreateWindow(
		windowClass.lpszClassName, 
		"Pong Game", 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		1280, 720, 0, 0,
		hInstance, 0);

	{
		//FULLSCREEN
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & -WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi)};
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(
			window, HWND_TOP, mi.rcMonitor.left, 
			mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}

	HDC hdc = GetDC(window);

	Input input = {};

	float deltaTime = 0.016666f;
	LARGE_INTEGER frameBeginTime;
	QueryPerformanceCounter(&frameBeginTime);

	float performanceFrequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}

	while (running)
	{
		// INPUT
		MSG message;

		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].changed - false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN: 
				{
					unsigned int keyCode = (unsigned int)message.wParam;
					bool keyDown = ((message.lParam & (1 << 31)) == 0);

#define processButton(b, vk)\
case vk: {\
input.buttons[b].changed = keyDown != input.buttons[b].keyDown;\
input.buttons[b].keyDown = keyDown;\
} break;
					switch (keyCode)
					{
						processButton(BUTTON_UP, VK_UP);
						processButton(BUTTON_DOWN, VK_DOWN);
						processButton(BUTTON_W, 'W');
						processButton(BUTTON_S, 'S');
						processButton(BUTTON_LEFT, VK_LEFT);
						processButton(BUTTON_RIGHT, VK_RIGHT);
						processButton(BUTTON_ENTER, VK_RETURN);
					}
				} break;

				default: 
				{
					TranslateMessage(&message);
					DispatchMessage(&message);	
				}
			}
		}

		// SIMULATE
		SimulateGame(&input, deltaTime);

		// RENDER
		StretchDIBits(
			hdc, 0, 0, renderState.width, renderState.height, 0, 0,
			renderState.width, renderState.height, renderState.memory,
			&renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		deltaTime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFrequency;
		frameBeginTime = frameEndTime;
	}
}