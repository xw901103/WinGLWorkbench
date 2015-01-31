#ifndef APPLICATION_H
#define APPLICATION_H
#ifdef _MSC_VER
#include <Windows.h>
#endif

class Application
{
	static Application* instance;

	bool running;
public:
	Application();
#ifdef _MSC_VER
	explicit Application(HINSTANCE hInitInstance);
#endif
	virtual ~Application();

	int execute();
	void quit();

	static Application* getInstance();

#ifdef _MSC_VER
	HINSTANCE hInstance;
#endif
};

#endif