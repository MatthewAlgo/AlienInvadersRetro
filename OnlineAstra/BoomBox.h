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
#include <optional>

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
		sf::Text GreetingText, SoundToggleText, MusicToggleText;
		std::unique_ptr<ImageToBeDrawn> WindowTitleTextbox, Speaker1;
		ImageToBeDrawn Speaker2, SoundBox, MusicBox;

		// Variables related to the main window
		sf::RenderWindow* WindowPointer = NULL;
		sf::Thread* MainWindowThread;
		sf::VideoMode* MainWindowVideo;
		
	public:
		typedef struct DJ {
			sf::Sound CollisionSound, ShootSound, PShootSound, DeathSound, WelcomeSound, WindowSound, MainThemeSound, SOUND_MAIN;
			sf::SoundBuffer CollisionSoundBuffer, ShootSoundBuffer, PShootSoundBuffer, DeathSoundBuffer, WelcomeSoundBuffer, WindowSoundBuffer, MainThemeSoundBuffer, SOUND_MAIN_BUFFER;
		};

		// Static variables -> The boombox can be accessed from anywhere
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

		// Static functions -> The boombox can be accessed from anywhere
		static void LoadAudioInMemory() {
			LocalDJ->ShootSoundBuffer.loadFromFile("BoomBoxRes/EnemyShoot.wav"); // Enemy shoot
			LocalDJ->ShootSound.setBuffer(LocalDJ->ShootSoundBuffer);
			LocalDJ->PShootSoundBuffer.loadFromFile("BoomBoxRes/PlayerShoot.wav"); // Player shoot
			LocalDJ->PShootSound.setBuffer(LocalDJ->PShootSoundBuffer);
			LocalDJ->WindowSoundBuffer.loadFromFile("BoomBoxRes/WindowEffect.wav"); // Window effect
			LocalDJ->WindowSound.setBuffer(LocalDJ->WindowSoundBuffer);
			LocalDJ->CollisionSoundBuffer.loadFromFile("BoomBoxRes/CollisionEffect.wav"); // Collision effect
			LocalDJ->CollisionSound.setBuffer(LocalDJ->CollisionSoundBuffer);
			LocalDJ->MainThemeSoundBuffer.loadFromFile("Music/WelcomeMusic.wav"); // Collision effect
			LocalDJ->MainThemeSound.setBuffer(LocalDJ->MainThemeSoundBuffer);
		}
		static void ShootSoundEffect() { // Effects
			if (IS_SOUND_ENABLED)
				LocalDJ->ShootSound.play();
		}
		static void PShootSoundEffect() { // Effects
			if (IS_SOUND_ENABLED)
				LocalDJ->PShootSound.play();
		}
		static void WindowSoundEffect() { // Effects
			if (IS_SOUND_ENABLED)
				LocalDJ->WindowSound.play();
		}
		static void WindowCollisionEffect() { // Effects
			if (IS_SOUND_ENABLED)
				LocalDJ->CollisionSound.play();
		}

		static void StartMainThemeSong() { // Main Theme
			if (IS_MUSIC_ENABLED) {
				LocalDJ->MainThemeSound.play();
				LocalDJ->MainThemeSound.setLoop(true);
			}
		}
		static sf::Sound* getMainTheme() {
			return &LocalDJ->MainThemeSound;
		}

		// For the AnimationWindow BoomBox
		static void LoadFileNameVector() {
			std::string path = "./Music";
			for (const auto& entry : fs::directory_iterator(path)) {
				std::string path_string{ entry.path().u8string() };
				if (strstr(path_string.c_str(), ".wav") != NULL) {
					WavFilesFromDirectory.push_back(path_string); // Outputs the .wav files in the current directory
				}
			}
		}
		static void INIT_BOOMBOX_MAIN() {
			LoadAudioInMemory(); StartMainThemeSong(); LoadFileNameVector();
		}
		static std::optional<std::string> RandomSongGenerator() { // Is going to generate a random song name at every call
			if (WavFilesFromDirectory.size() > 0 && IS_MUSIC_ENABLED) {
				int choice = RandomParticlesGenerator::Mersenne_Twister_Generator(0, WavFilesFromDirectory.size() - 1);
				LocalDJ->SOUND_MAIN_BUFFER.loadFromFile(WavFilesFromDirectory.at(choice));
				LocalDJ->SOUND_MAIN.setBuffer(LocalDJ->SOUND_MAIN_BUFFER);
				LocalDJ->SOUND_MAIN.play();
				return WavFilesFromDirectory.at(choice);
			}
			return {};
		}
	};
}
#pragma endregion ANIMATION_WINDOW

#endif