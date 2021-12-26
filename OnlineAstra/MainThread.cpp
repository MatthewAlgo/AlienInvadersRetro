#include "MainWindowHeader.h"
#include "AnimationWindow.h"
#include "ServerHeader.h"
#include "ClientHeader.h"

#include <iostream>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main()
{
	// Linux Compile: g++ MainThread.cpp MainWindowSource.cpp -lsfml-graphics -lsfml-system -lsfml-window -lsfml-audio -lX11

#ifdef __linux__
	XInitThreads();
#endif

	MatthewsNamespace::MainWindowClass* MyMainWindow;
	MyMainWindow = new MatthewsNamespace::MainWindowClass("MainWindow", 1000, 500);

	// MatthewsNamespace::ServerClass* MyServer;
	// MyServer = new MatthewsNamespace::ServerClass(1337, "Hello from the other byte");

	std::cin.get();
	return 0;
}
