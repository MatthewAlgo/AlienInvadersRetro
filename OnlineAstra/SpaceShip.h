#pragma once
#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SpaceShipBullet.h"
#include "EnemySpaceShip.h"
#include "EnemySpaceShipBullet.h"

#pragma region SPACESHIP_CLASS
namespace MatthewsNamespace {
	class SpaceShip {
	protected:
		typedef struct Position {
			double x, y;
		};
		Position POS;

		sf::Texture TextureOfSpaceShip;
		sf::Sprite SpriteOfSpaceShip;
		sf::Vector2f WinSize;

		// It produces explosions on its bullet collisions -> their data is located here
		sf::Texture Explosion; sf::Sprite ExplosionSprite;

	public:
		short Life = 50;
		// Each spaceship has a queue of bullets
		std::deque<SpaceShipBullet*> BulletDeque;

		inline void MoveUp() { if(POS.y - 20 > 0 && POS.y-20 < WinSize.y) POS.y -= 30; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveDown() { if (POS.y + 20 > 0 && POS.y + 20 < WinSize.y) POS.y += 30; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveLeft() { if (POS.x - 20 > -60 && POS.x - 20 < WinSize.x) POS.x -= 30; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveRight() { if (POS.x + 20 > -60 && POS.x + 20 < WinSize.x) POS.x += 30; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }

		SpaceShip(sf::Vector2f WindowSize, sf::Texture Texture) : WinSize(WindowSize), TextureOfSpaceShip(Texture) {
			// Init the explosion if this constructor is called
			POS.y = WindowSize.y / 2;
			POS.x = WindowSize.x / 2;
			// Set the position of the spaceship
			SpriteOfSpaceShip.setTexture(TextureOfSpaceShip);
			SpriteOfSpaceShip.setPosition(sf::Vector2f(POS.x, POS.y));

			// Init the explosion
			Explosion.loadFromFile("Explosion.png");
			ExplosionSprite.setTexture(Explosion);
			ExplosionSprite.setScale(0.1, 0.1);
		}
		SpaceShip(sf::Vector2f WindowSize) : WinSize(WindowSize) {
			// Init the explosion if this constructor is called
			Explosion.loadFromFile("Explosion.png");
			ExplosionSprite.setTexture(Explosion);
			ExplosionSprite.setScale(0.1, 0.1);
		}
		SpaceShip() {
			// Init the explosion if this constructor is called
			Explosion.loadFromFile("Explosion.png");
			ExplosionSprite.setTexture(Explosion);
			ExplosionSprite.setScale(0.1, 0.1);
		}
		~SpaceShip() = default;

		void setTexture(sf::Texture& texture) { TextureOfSpaceShip = texture; }
		void setSprite(sf::Sprite& sprite) { SpriteOfSpaceShip = sprite; }
		void centerInWindow(sf::Vector2f WindowSize) { POS.y = WindowSize.y / 2; POS.x = WindowSize.x / 2; }
		void setSpaceShipPosition(double X, double Y) { POS.x = X; POS.y = Y; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		void scaleSpaceShip(int scx, int scy) { SpriteOfSpaceShip.setScale(scx, scy); }

		sf::Texture* getSpaceShipTexture()  { return &TextureOfSpaceShip; }
		sf::Sprite* getSpaceShipSprite()  { return &SpriteOfSpaceShip; }
		sf::Vector2f getSpaceShipPosition() { return sf::Vector2f(POS.x, POS.y); }
		short* getLife() { return &Life; }
		void setLife(short L) { Life = L; }

		void setMainWindowSize(int X, int Y) { WinSize.x = X; WinSize.y = Y; }

		void Shoot() {
			// Load the bullet -> The bullet receives some speed of +10 per frame
			// The initial bullet / shuttle
			SpaceShipBullet CurrentBullet;
			BulletDeque.push_back(new SpaceShipBullet(CurrentBullet));
			BulletDeque.back()->setTexture("NeonVertical.png");
			BulletDeque.back()->scaleSpaceShipBullet(0.25, 0.25);
			BulletDeque.back()->setSpaceShipBulletPosition(POS.x+13, POS.y-40); // Front Bullet
		}
		int IterateThroughBullets(sf::RenderWindow* WINDOW, std::vector<EnemySpaceShip*>& Enemies) {
			int scoreByPlayer = 0;
			for (int i{}; i < this->BulletDeque.size(); ++i) {
				WINDOW->draw(*this->BulletDeque[i]->getSpaceShipBulletSprite());
				if (this->BulletDeque.at(i) != nullptr) {
					this->BulletDeque[i]->setSpaceShipBulletPosition(this->BulletDeque[i]->getSpaceShipBulletPosition().x,
						this->BulletDeque[i]->getSpaceShipBulletPosition().y - 45);
				}
			}
			// Check if the bullets hit enemy spaceships
			for (int i{}; i < BulletDeque.size(); ++i) {
				// Iterate through enemies and their positions
				for (int j{}; j < Enemies.size(); ++j) {
					if (BulletDeque.at(i)->getSpaceShipBulletPosition().x > Enemies.at(j)->getSpaceShipPosition().x - 40 && BulletDeque.at(i)->getSpaceShipBulletPosition().x < Enemies.at(j)->getSpaceShipPosition().x+20)
						if (BulletDeque.at(i)->getSpaceShipBulletPosition().y > Enemies.at(j)->getSpaceShipPosition().y - 40 && BulletDeque.at(i)->getSpaceShipBulletPosition().y < Enemies.at(j)->getSpaceShipPosition().y+20) {
							// Draw Explosion on Impact
							ExplosionSprite.setPosition(BulletDeque.at(i)->getSpaceShipBulletPosition().x, BulletDeque.at(i)->getSpaceShipBulletPosition().y);
							WINDOW->draw(ExplosionSprite);
							scoreByPlayer++;
							// Decrease life + set death behavior
							(*Enemies.at(j)->getLife())-= *BulletDeque.at(i)->getDamage();
							if (*Enemies.at(j)->getLife() == 0) {
								Enemies.at(j)->Die();
								EnemySpaceShip* Iterator = Enemies.at(j);
								delete Iterator;
								Enemies.erase(Enemies.begin() + j);
							}
							if (this->BulletDeque.at(i) != NULL) {
								// We delete the bullet and deliver damage to the main space shuttle
								SpaceShipBullet* it = this->BulletDeque.at(i);
								delete it; it = nullptr; this->BulletDeque.erase(this->BulletDeque.begin() + i);
							}
							break; // Exit the loop so that variables are updated -> Seems to be time consuming
						}
				}
			}
			return scoreByPlayer*100;

		}
		void FreeUpMemoryBullets() {
			for (unsigned int i{}; i < this->BulletDeque.size(); i++) { // Manage and free up the memory
				if (this->BulletDeque.at(i)->getSpaceShipBulletPosition().y < -300) { // If the bullet is offscreen
					SpaceShipBullet* it = this->BulletDeque.at(i);
					delete it; it = nullptr; this->BulletDeque.erase(this->BulletDeque.begin() + i);
				}
			}
		}

	};
}
#pragma endregion SPACESHIP_CLASS
#endif