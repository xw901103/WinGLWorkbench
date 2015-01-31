#include "Window.h"
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include "Application.h"

#include <iostream>
#include <map>

LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static std::map<HWND, Window*> windowMap;
	static unsigned int openedWindowCount = 0;
	LONG result = 1;
	Window* window = 0;
	LPCREATESTRUCT createStruct;
	WindowEvent event;
	RECT rect;
	PAINTSTRUCT ps;

	GetClientRect(hWnd, &rect);
	std::map<HWND, Window*>::iterator iter = windowMap.find(hWnd);
	event.hWnd = hWnd;
	event.width = rect.right;
	event.height = rect.bottom;

	if (iter != windowMap.end())
	{
		window = windowMap.find(hWnd)->second;
	}
	switch (uMsg)
	{
	case WM_CREATE:
		createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		if (createStruct)
		{
			window = reinterpret_cast<Window*>(createStruct->lpCreateParams);
			windowMap.insert(std::pair<HWND, Window*>(hWnd, window));
			event.event = WindowEvent::WINDOW_CREATE;
			window->notifyEvent(&event);
			++openedWindowCount;
		}
		break;
	case WM_PAINT:
		event.event = WindowEvent::WINDOW_RENDER;
		window->notifyEvent(&event);
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		event.event = WindowEvent::WINDOW_RESIZE;
		window->notifyEvent(&event);
		break;
	case WM_CLOSE:
		event.event = WindowEvent::WINDOW_CLOSE;
		window->notifyEvent(&event);
		if (--openedWindowCount == 0) {
			Application::getInstance()->quit();
		}
		break;
	case WM_DESTROY:
		event.event = WindowEvent::WINDOW_DESTROY;
		window->notifyEvent(&event);
		windowMap.erase(hWnd);
		break;
	case WM_KEYDOWN:
		event.event = WindowEvent::WINDOW_KEYDOWN;
		window->notifyEvent(&event);
		break;
	default:
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return result;
}

Window::Window(Window* initParent) : parent(initParent), visibility(false)
{
}

Window::~Window()
{
#ifdef _MSC_VER
	DestroyWindow(hWnd);
#endif
}

Window* Window::getParent() const
{
	return this->parent;
}

void Window::setParent(Window* newParent)
{
	this->parent = newParent;
}

void Window::show()
{
#ifdef _MSC_VER
	static WNDCLASS wndclass;
	static bool isWindowClassRegistered = false;

	if (this->hWnd == NULL)
	{
		if (!isWindowClassRegistered)
		{
			wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
			wndclass.cbClsExtra = 0;
			wndclass.cbWndExtra = 0;
			wndclass.hInstance = Application::getInstance()->hInstance;
			wndclass.hIcon = LoadIcon(Application::getInstance()->hInstance, TEXT("WORKBENCH"));
			wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wndclass.lpszMenuName = TEXT("WORKBENCH");
			wndclass.lpszClassName = TEXT("WORKBENCH");

			isWindowClassRegistered = static_cast<bool>(RegisterClass(&wndclass));
		}

		this->hWnd = CreateWindow(
			TEXT("WORKBENCH"),
			TEXT("Window"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			Application::getInstance()->hInstance,
			reinterpret_cast<LPVOID>(this));
		if (this->hWnd == NULL)
		{
			DWORD dw = GetLastError();
			std::cerr << "ERR " << dw << std::endl;
		}
	}
	this->visibility = static_cast<bool>(ShowWindow(this->hWnd, SW_SHOW));
#endif
}

void Window::hide()
{
#ifdef _MSC_VER
	this->visibility = !static_cast<bool>(ShowWindow(this->hWnd, SW_HIDE));
#endif
}

bool Window::isVisible() const
{
	return this->visibility;
}

void Window::notifyEvent(WindowEvent* event) {
	this->processEvent(event);
}

void Window::processEvent(WindowEvent* event)
{
	if (event)
	{
		switch (event->event)
		{
		case WindowEvent::WINDOW_CREATE:
			this->width = event->width;
			this->height = event->height;
			break;
		case WindowEvent::WINDOW_RENDER:
			this->width = event->width;
			this->height = event->height;
			break;
		case WindowEvent::WINDOW_CLOSE:
			this->hide();
			break;
		case WindowEvent::WINDOW_DESTROY:
#ifdef _MSC_VER
#endif
			break;
		default:
			break;
		}
	}
}