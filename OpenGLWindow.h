#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include "Window.h"

class OpenGLWindow : public Window
{
public:
	OpenGLWindow(Window* initParent = 0);
	virtual ~OpenGLWindow();

	virtual void notifyEvent(WindowEvent*);
protected:
	virtual void processEvent(WindowEvent*);
	virtual void initialize() = 0;
	virtual void render() = 0;
};

#endif