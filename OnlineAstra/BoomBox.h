#pragma once
#ifndef PLAYER_STATS_WINDOW_H
#define PLAYER_STATS_WINDOW_H

#pragma region INCLUDES
#include "StructuresAndOtherFunctions.h"
#include "MainWindowHeader.h"
#include "SpaceShip.h"
#include "RandomParticlesGenerator.h"

#include <thread>
#include <functional>
#include <filesystem>
#include <string>
#include <cstring>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#pragma endregion INCLUDES

namespace fs = std::filesystem;

#pragma region ANIMATION_WINDOW
namespace MatthewsNamespace {
	class BoomBox {
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
		std::unique_ptr<ImageToBeDrawn> WindowTitleTextbox, Speaker1;
		ImageToBeDrawn Speaker2;

		// Variables related to the main window
		sf::RenderWindow* WindowPointer = NULL;
		sf::Thread* MainWindowThread;
		sf::VideoMode* MainWindowVideo;
		
	public:
		typedef struct DJ {
			sf::Sound CollisionSound, ShootSound, PShootSound, DeathSound, WelcomeSound, WindowSound, MainThemeSound;
			sf::SoundBuffer CollisionSoundBuffer, ShootSoundBuffer, PShootSoundBuffer, DeathSoundBuffer, WelcomeSoundBuffer, WindowSoundBuffer, MainThemeSoundBuffer;
		};
		static std::unique_ptr<DJ> LocalDJ;
		static bool IS_SOUND_ENABLED, IS_MUSIC_ENABLED;
		static std::vector<std::string> WavFilesFromDirectory;


		BoomBox(const std::string TITLE, int W, int H) : WindowTitle(TITLE), MainWindowVideo(new sf::VideoMode(W, H)),
			WWidth(static_cast<int>(W)), WHeight(static_cast<int>(H)) {
			// MainWindowThread = new sf::Thread(std::bind(&MainWindowClass::MainWindowThreadExecution,this, *TripleHolder));
		
			MainWindowThread = new sf::Thread([&]() -> void {
				// Create window and set active
				BoomBox::WindowPointer = new sf::RenderWindow(*MainWindowVideo, WindowTitle, sf::Style::Titlebar | sf::Style::Close); // Create the window
				WindowPointer->setActive(false);

				std::unique_ptr<TripleItemHolder<sf::RenderWindow, sf::Thread, BoomBox>> TripleHolder = std::make_unique<TripleItemHolder
					<sf::RenderWindow, sf::Thread, BoomBox>>(WindowPointer, MainWindowThread, this);

				BoomBox::MainWindowThreadExecution(*TripleHolder);
				});
			// Create and launch the window thread
			MainWindowThread->launch();
			// Load all buffers
			// LoadAudioInMemory();
		};

		~BoomBox() = default; // Auto deallocate smart pointers 

		void MainWindowThreadExecution(TripleItemHolder<sf::RenderWindow, sf::Thread, BoomBox>& ITEM_HOLDER);
		void DrawInsideMainWindow(sf::RenderWindow* WINDOW, sf::Thread* WINTHREAD, BoomBox* C);
		void RenderTextures(DoubleItemHolder<sf::RenderWindow, BoomBox> ITEM_HOLDER);

		static void LoadAudioInMemory() {
			LocalDJ->ShootSoundBuffer.loadFromFile("BoomBoxRes/EnemyShoot.wav"); // Enemy shoot
			LocalDJ->ShootSound.setBuffer(LocalDJ->ShootSoundBuffer);
			LocalDJ->PShootSoundBuffer.loadFromFile("BoomBoxRes/PlayerShoot.wav"); // Player shoot
			LocalDJ->PShootSound.setBuffer(LocalDJ->PShootSoundBuffer);
			LocalDJ->WindowSoundBuffer.loadFromFile("BoomBoxRes/WindowEffect.wav"); // Window effect
			LocalDJ->WindowSound.setBuffer(LocalDJ->WindowSoundBuffer);
			LocalDJ->CollisionSoundBuffer.loadFromFile("BoomBoxRes/CollisionEffect.wav"); // Collision effect
			LocalDJ->CollisionSound.setBuffer(LocalDJ->CollisionSoundBuffer);
			LocalDJ->MainThemeSoundBuffer.loadFromFile("BoomBoxRes/WelcomeMusic.wav"); // Collision effect
			LocalDJ->MainThemeSound.setBuffer(LocalDJ->MainThemeSoundBuffer);

		}
		static void ShootSoundEffect() {
			if (IS_SOUND_ENABLED)
				LocalDJ->ShootSound.play();
		}
		static void PShootSoundEffect() {
			if (IS_SOUND_ENABLED)
				LocalDJ->PShootSound.play();
		}
		static void WindowSoundEffect() {
			if (IS_SOUND_ENABLED)
				LocalDJ->WindowSound.play();
		}
		static void WindowCollisionEffect() {
			if (IS_SOUND_ENABLED)
				LocalDJ->CollisionSound.play();
		}

		static void StartMainThemeSong() {
			if (IS_MUSIC_ENABLED) {
				LocalDJ->MainThemeSound.play();
				LocalDJ->MainThemeSound.setLoop(true);
			}
		}
		static sf::Sound* getMainTheme() {
			return &LocalDJ->MainThemeSound;
		}
		static void getRandomFileNameFromDir() {
			std::string path = "./BoomBoxRes";
			for (const auto& entry : fs::directory_iterator(path)) {
				std::string path_string{ entry.path().u8string() };
				if (strstr(path_string.c_str(), ".wav") != NULL)
					WavFilesFromDirectory.push_back(path_string); // Outputs the .wav files in the current directory
			}
		}
		static void INIT_BOOMBOX_MAIN() {
			LoadAudioInMemory(); StartMainThemeSong(); getRandomFileNameFromDir();
		}
		static std::string GenerateRandomSongName() {
			// All Wav files in WavFilesFromDirectory vector -> TODO: Init the random engine
		}
	};
}
#pragma endregion ANIMATION_WINDOW

#endif