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
	MyMainWindow = new MatthewsNamespace::MainWindowClass("AlienInvasion - Main Menu", 1000, 500);

	std::cin.get();
	return 0;
}
