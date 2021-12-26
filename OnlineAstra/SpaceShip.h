#pragma once
#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SpaceShipBullet.h"

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

	public:
		// Each spaceship has a queue of bullets
		std::deque<SpaceShipBullet*> BulletDeque;

		inline void MoveUp() { POS.y -= 10; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveDown() { POS.y += 10; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveLeft() { POS.x -= 10; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		inline void MoveRight() { POS.x += 10; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }

		SpaceShip(sf::Vector2f WindowSize, sf::Texture Texture) : WinSize(WindowSize), TextureOfSpaceShip(Texture) {
			POS.y = WindowSize.y / 2;
			POS.x = WindowSize.x / 2;
			// Set the position of the spaceship
			SpriteOfSpaceShip.setTexture(TextureOfSpaceShip);
			SpriteOfSpaceShip.setPosition(sf::Vector2f(POS.x, POS.y));
		}

		SpaceShip() = default;
		~SpaceShip() = default;

		void setTexture(sf::Texture& texture) { TextureOfSpaceShip = texture; }
		void setSprite(sf::Sprite& sprite) { SpriteOfSpaceShip = sprite; }
		void centerInWindow(sf::Vector2f WindowSize) { POS.y = WindowSize.y / 2; POS.x = WindowSize.x / 2; }
		void setSpaceShipPosition(double X, double Y) { POS.x = X; POS.y = Y; SpriteOfSpaceShip.setPosition(POS.x, POS.y); }
		void scaleSpaceShip(int scx, int scy) { SpriteOfSpaceShip.setScale(scx, scy); }

		sf::Texture* getSpaceShipTexture()  { return &TextureOfSpaceShip; }
		sf::Sprite* getSpaceShipSprite()  { return &SpriteOfSpaceShip; }
		sf::Vector2f getSpaceShipPosition() { return sf::Vector2f(POS.x, POS.y); }

		void Shoot() {
			// Load the bullet -> The bullet receives some speed of +10 per frame
			SpaceShipBullet CurrentBullet;
			BulletDeque.push_back(new SpaceShipBullet(CurrentBullet));
			BulletDeque.back()->setTexture("NeonVertical.png");
			// BulletDeque.back()->scaleSpaceShipBullet(0.25, 0.25);
			BulletDeque.back()->setSpaceShipBulletPosition(POS.x - 78, POS.y - 175); // Front Bullet

		}

	};
}
#pragma endregion SPACESHIP_CLASS
#endif