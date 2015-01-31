#include "OpenGLWindow.h"
#include <gl/GL.h>

OpenGLWindow::OpenGLWindow(Window* initParent) : Window(initParent)
{
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::initialize()
{
}

void OpenGLWindow::render()
{
}

void OpenGLWindow::notifyEvent(WindowEvent* event)
{
	this->processEvent(event);
}

#ifdef _MSC_VER
bool setupPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd, *ppfd;
	int pixelformat;

	ppfd = &pfd;

	ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	ppfd->nVersion = 1;
	ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	ppfd->dwLayerMask = PFD_MAIN_PLANE;
	ppfd->iPixelType = PFD_TYPE_RGBA;
	ppfd->cColorBits = 32;
	ppfd->cDepthBits = 16;
	ppfd->cAccumBits = 0;
	ppfd->cStencilBits = 0;

	pixelformat = ChoosePixelFormat(hDC, ppfd);

	if ((pixelformat = ChoosePixelFormat(hDC, ppfd)) == 0)
	{
		return false;
	}

	if (SetPixelFormat(hDC, pixelformat, ppfd) == FALSE)
	{
		return false;
	}

	return true;
}
#endif

void OpenGLWindow::processEvent(WindowEvent* event)
{
#ifdef _MSC_VER
	static HGLRC hRC;
#endif
	if (event)
	{
#ifdef _MSC_VER
		HDC hDC;
#endif
		switch (event->event)
		{
		case WindowEvent::WINDOW_CREATE:
			Window::processEvent(event);
#ifdef _MSC_VER
			hDC = GetDC(event->hWnd);
			if (!setupPixelFormat(hDC))
				PostQuitMessage(0);

			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC); 
#endif
			this->initialize();
			break;
		case WindowEvent::WINDOW_RENDER:
			Window::processEvent(event);
			this->render();
#ifdef _MSC_VER
			hDC = GetDC(event->hWnd);
			SwapBuffers(hDC);
			UpdateWindow(event->hWnd);
#endif
			break;
		case WindowEvent::WINDOW_DESTROY:
#ifdef _MSC_VER
			wglDeleteContext(hRC);
#endif
			break;
		default:
			Window::processEvent(event);
			break;
		}
	}
}
