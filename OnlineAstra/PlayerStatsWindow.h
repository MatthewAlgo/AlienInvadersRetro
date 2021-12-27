#pragma once
#ifndef PLAYER_STATS_WINDOW_H
#define PLAYER_STATS_WINDOW_H

#pragma region INCLUDES
#include "StructuresAndOtherFunctions.h"
#include "MainWindowHeader.h"
#include "SpaceShip.h"
#include "RandomParticlesGenerator.h"
#include "EnemySpaceShip.h"

#include <thread>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#pragma endregion INCLUDES

#pragma region ANIMATION_WINDOW
namespace MatthewsNamespace {
	class PlayerStatsWindow {
	private:
		// Same variables
	protected:
		struct ImageToBeDrawn {
			sf::Sprite SPRITE;
			sf::Texture TEXTURE;
		};

		sf::Int32 WWidth;
		sf::Int32 WHeight;
		const std::string WindowTitle;

		std::unique_ptr<ImageToBeDrawn> BackGround;
		sf::Font GlobalWindowFont;
		sf::Text GreetingText;
		std::unique_ptr<ImageToBeDrawn> WindowTitleTextbox;

		// Variables related to the main window
		sf::RenderWindow* WindowPointer = NULL;
		sf::Thread* MainWindowThread;
		sf::VideoMode* MainWindowVideo;

	public:
		PlayerStatsWindow(const std::string TITLE, int W, int H) : WindowTitle(TITLE), MainWindowVideo(new sf::VideoMode(W, H)),
			WWidth(static_cast<int>(W)), WHeight(static_cast<int>(H)) {
			// MainWindowThread = new sf::Thread(std::bind(&MainWindowClass::MainWindowThreadExecution,this, *TripleHolder));
		
			MainWindowThread = new sf::Thread([&]() -> void {
				// Create window and set active
				PlayerStatsWindow::WindowPointer = new sf::RenderWindow(*MainWindowVideo, WindowTitle, sf::Style::Titlebar | sf::Style::Close); // Create the window
				WindowPointer->setActive(false);

				std::unique_ptr<TripleItemHolder<sf::RenderWindow, sf::Thread, PlayerStatsWindow>> TripleHolder = std::make_unique<TripleItemHolder
					<sf::RenderWindow, sf::Thread, PlayerStatsWindow>>(WindowPointer, MainWindowThread, this);

				PlayerStatsWindow::MainWindowThreadExecution(*TripleHolder);
				});
			// Create and launch the window thread
			MainWindowThread->launch();
		};

		~PlayerStatsWindow() = default; // Auto deallocate smart pointers 

		void MainWindowThreadExecution(TripleItemHolder<sf::RenderWindow, sf::Thread, PlayerStatsWindow>& ITEM_HOLDER);
		void DrawInsideMainWindow(sf::RenderWindow* WINDOW, sf::Thread* WINTHREAD, PlayerStatsWindow* C);
		void RenderTextures(DoubleItemHolder<sf::RenderWindow, PlayerStatsWindow> ITEM_HOLDER);
	};
}
#pragma endregion ANIMATION_WINDOW

#endif