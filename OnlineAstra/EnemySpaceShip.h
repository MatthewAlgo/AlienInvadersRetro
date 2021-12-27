#pragma once
#ifndef ENEMY_SPACE_SHIP_H
#define ENEMY_SPACE_SHIP_H

#include <deque>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "EnemySpaceShipBullet.h"
#include "RandomParticlesGenerator.h"
#include "SpaceShip.h"

#pragma region ENEMY_SPACESHIP_REGION
namespace MatthewsNamespace {
	class EnemySpaceShip {
	protected:
		typedef struct Position {
			double x, y, speed = 0;
		};
		Position POS;

		sf::Texture TextureOfSpaceShip;
		sf::Sprite SpriteOfSpaceShip;
		sf::Vector2f WinSize;

		int shooter_clock = 0;
		short Life = 10;
		// It produces explosions on its bullet collisions -> their data is located here
		sf::Texture Explosion; sf::Sprite ExplosionSprite;

	public:
		// Each spaceship has a queue of bullets
		std::deque<EnemySpaceShipBullet*> BulletDeque;

		inline void MoveUp() { if (POS.y - 5 > 0 && POS.y - 5 < WinSize.y) POS.y -= 5; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveDown() { if (POS.y + 5 > 0 && POS.y + 5 < WinSize.y) POS.y += 5; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveLeft() { if (POS.x - 5 > 0 && POS.x - 5 < WinSize.y) POS.x -= 5; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveRight() { if (POS.x + 5 > 0 && POS.x + 5 < WinSize.y) POS.x += 5; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }

		EnemySpaceShip(sf::Vector2f WindowSize, sf::Texture Texture, int WWidth) : WinSize(WindowSize), TextureOfSpaceShip(Texture) {
			// POS.y = WindowSize.y / 2;
			POS.x = 0;
			// Set the position of the spaceship
			SpriteOfSpaceShip.setTexture(TextureOfSpaceShip);
			this->setSpaceShipPosition(MatthewsNamespace::RandomParticlesGenerator::Mersenne_Twister_Generator(0, WWidth), 0);
			
			// Init the explosion if this constructor is called
			Explosion.loadFromFile("Explosion.png");
			ExplosionSprite.setTexture(Explosion);
			ExplosionSprite.setScale(0.1, 0.1);
		}
		EnemySpaceShip(sf::Vector2f WindowSize) : WinSize(WindowSize) { // Init the explosion if this constructor is called
			Explosion.loadFromFile("Explosion.png");
			ExplosionSprite.setTexture(Explosion);
			ExplosionSprite.setScale(0.1, 0.1);
		}
		EnemySpaceShip() { // Init the explosion if this constructor is called
			Explosion.loadFromFile("Explosion.png");
			ExplosionSprite.setTexture(Explosion);
			ExplosionSprite.setScale(0.1, 0.1);
		}
		~EnemySpaceShip() = default;

		void setTexture(sf::Texture& texture) { TextureOfSpaceShip = texture; SpriteOfSpaceShip.setTexture(TextureOfSpaceShip); }
		void setTexture(std::string FileName) { TextureOfSpaceShip.loadFromFile(FileName); SpriteOfSpaceShip.setTexture(TextureOfSpaceShip); }
		
		void setSprite(sf::Sprite & sprite) { SpriteOfSpaceShip = sprite; }
		void centerInWindow(sf::Vector2f WindowSize) { POS.y = WindowSize.y / 2; POS.x = WindowSize.x / 2; }
		void setSpaceShipPosition(double X, double Y) { POS.x = X; POS.y = Y; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		void scaleSpaceShip(int scx, int scy) { SpriteOfSpaceShip.setScale(scx, scy); }

		sf::Texture* getSpaceShipTexture() { return &TextureOfSpaceShip; }
		sf::Sprite* getSpaceShipSprite() { return &SpriteOfSpaceShip; }
		sf::Vector2f getSpaceShipPosition() { return sf::Vector2f(POS.x, POS.y); }
		short* getLife() { return &Life; }
		void setLife(short L) { Life = L; }

		void setMainWindowSize(int X, int Y) { WinSize.x = X; WinSize.y = Y; }

		void Shoot() {
			shooter_clock++;
			if (shooter_clock % 70 == 0) {
				// Load the bullet -> The bullet receives some speed of +10 per frame
				EnemySpaceShipBullet CurrentBullet;
				BulletDeque.push_back(new EnemySpaceShipBullet(CurrentBullet));
				BulletDeque.back()->setTexture("NeonVertical.png");
				BulletDeque.back()->scaleSpaceShipBullet(0.4, 0.2);
				BulletDeque.back()->getSpaceShipBulletSprite()->setColor(sf::Color::Yellow);
				BulletDeque.back()->setSpaceShipBulletPosition(POS.x-13, POS.y+10); // Front Bullet
				shooter_clock = MatthewsNamespace::RandomParticlesGenerator::Mersenne_Twister_Generator(0, 50);
			}

		}

		void GenerateInDrawFunctionOfMainWindow(sf::RenderWindow* Window, std::string SpaceShipTextureName = "EnemySpaceShip.png") {
			// Generate a new spaceship
			this->setSpaceShipPosition(MatthewsNamespace::RandomParticlesGenerator::Mersenne_Twister_Generator(0, Window->getSize().x), -20); // Spawn with offset so that enemies appear fluently on screen
			this->setTexture(SpaceShipTextureName);
			this->getSpaceShipSprite()->setScale(0.1, 0.1);
			Window->draw(*this->getSpaceShipSprite());
			this->POS.speed = 0.1;
		}
		void DrawBulletsInWindow(sf::RenderWindow* Window, int XSSPos, int YSSPos, short& SS) {
			for (int i{}; i < BulletDeque.size(); ++i) {
				Window->draw(*this->BulletDeque[i]->getSpaceShipBulletSprite());
				if (this->BulletDeque.at(i) != nullptr) {
					this->BulletDeque[i]->setSpaceShipBulletPosition(this->BulletDeque[i]->getSpaceShipBulletPosition().x,
						this->BulletDeque[i]->getSpaceShipBulletPosition().y + 10); // Bullet Speed
				}
			}
			// Check if the bullets hit the spaceship
			for (int i{}; i < BulletDeque.size(); ++i) {
				if(BulletDeque.at(i)->getSpaceShipBulletPosition().x > XSSPos - 40 && BulletDeque.at(i)->getSpaceShipBulletPosition().x < XSSPos+40)
					if (BulletDeque.at(i)->getSpaceShipBulletPosition().y > YSSPos - 40 && BulletDeque.at(i)->getSpaceShipBulletPosition().y < YSSPos + 80) {
						
						// Draw Explosion on Impact
						Explosion.loadFromFile("Explosion.png");
						ExplosionSprite.setTexture(Explosion);
						ExplosionSprite.setScale(0.1, 0.1);
						ExplosionSprite.setPosition(BulletDeque.at(i)->getSpaceShipBulletPosition().x, BulletDeque.at(i)->getSpaceShipBulletPosition().y);
						Window->draw(ExplosionSprite); // Will output a white square -> Retro style explosion
						
						SS--; SS -= EnemySpaceShipBullet::DAMAGE_SUPPLIER; if (SS <= 0) {}
							// The Player Died
							// Show game over screen or whatever
						
						// We delete the bullet and deliver damage to the main space shuttle
						EnemySpaceShipBullet* it = this->BulletDeque.at(i);
						delete it; it = nullptr; this->BulletDeque.erase(this->BulletDeque.begin() + i);
					}
			}
		}
		void FreeUpMemoryFromBullets(sf::RenderWindow* Window) {
			for (unsigned int i{}; i < this->BulletDeque.size(); i++) { // Manage and free up the memory
				if (this->BulletDeque.at(i)->getSpaceShipBulletPosition().y > Window->getSize().y) { // If the bullet is offscreen
					EnemySpaceShipBullet* it = this->BulletDeque.at(i);
					delete it; it = nullptr; this->BulletDeque.erase(this->BulletDeque.begin() + i);
				}}
		}
		void Draw_IterateExistingItem(sf::RenderWindow* Window) {
			POS.y += this->POS.speed;
			MoveRandomLeftOrRightorUpOrDown();
			this->setSpaceShipPosition(POS.x, POS.y);
			this->getSpaceShipSprite()->setScale(0.1, 0.1);
			Window->draw(*this->getSpaceShipSprite());
		}
		void MoveRandomLeftOrRightorUpOrDown() {
			int randomx = MatthewsNamespace::RandomParticlesGenerator::Mersenne_Twister_Generator(-10, 10);
			int randomy = MatthewsNamespace::RandomParticlesGenerator::Mersenne_Twister_Generator(-10, 10);

			if (POS.x + randomx > 0 && POS.x + randomx < WinSize.x)
				POS.x += randomx;
			if (POS.y + randomy > -20 && POS.y + randomy < WinSize.x)
				POS.y += randomy;

		}
		void Die() { // Free up the buffer of bullets
			for (unsigned int i{}; i < this->BulletDeque.size(); i++) { // Manage and free up the memory
				EnemySpaceShipBullet* it = this->BulletDeque.at(i);
				delete it; it = nullptr; this->BulletDeque.erase(this->BulletDeque.begin() + i);
			}
		}

	};

}
#pragma endregion ENEMY_SPACESHIP_REGION

#endif
