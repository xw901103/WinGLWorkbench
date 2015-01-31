#ifndef WINDOW_H
#define WINDOW_H

#ifdef _MSC_VER
#include <Windows.h>
#endif

struct WindowEvent {
	typedef enum {
		WINDOW_CREATE,
		WINDOW_RENDER,
		WINDOW_RESIZE,
		WINDOW_CLOSE,
		WINDOW_DESTROY,
		WINDOW_KEYDOWN
	} Event;

#ifdef _MSC_VER
	HWND hWnd;
#endif
	Event event;
	int x, y, width, height, key;
};

class Window
{
	Window* parent;
	bool visibility;
#ifdef _MSC_VER
	HWND hWnd;
#endif
	int width, height;
public:
	Window(Window* initParent = 0);
	virtual ~Window();
	
	Window* getParent() const;
	void setParent(Window*);

	void show();
	void hide();

	bool isVisible() const;

	virtual void notifyEvent(WindowEvent*);
protected:
	virtual void processEvent(WindowEvent*);
};

#endif