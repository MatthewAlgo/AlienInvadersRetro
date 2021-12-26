#pragma once
#ifndef SPACE_SHIP_BULLET_H
#define SPACE_SHIP_BULLET_H

#include "SpaceShip.h"

namespace MatthewsNamespace {
	class SpaceShipBullet {
	protected:
		typedef struct Position {
			double x, y;
		};
		Position POS;

		sf::Texture TextureOfSpaceShipBullet;
		sf::Sprite SpriteOfSpaceShipBullet;
		sf::Vector2f WinSize;

	public:
		SpaceShipBullet(sf::Vector2f WindowSize, sf::Texture Texture) : WinSize(WindowSize), TextureOfSpaceShipBullet(Texture) {
			POS.y = WindowSize.y / 2;
			POS.x = WindowSize.x / 2;
			// Set the position of the spaceship
			SpriteOfSpaceShipBullet.setTexture(TextureOfSpaceShipBullet);
			SpriteOfSpaceShipBullet.setPosition(sf::Vector2f(POS.x, POS.y));
		}
		SpaceShipBullet() = default;
		~SpaceShipBullet() = default;

		inline void MoveUp() { POS.y -= 10; SpriteOfSpaceShipBullet.setPosition(POS.x, POS.y); }

		void setTexture(sf::Texture& texture) { TextureOfSpaceShipBullet = texture; SpriteOfSpaceShipBullet.setTexture(TextureOfSpaceShipBullet); } // Plus Sprite
		void setTexture(std::string FileName) { TextureOfSpaceShipBullet.loadFromFile(FileName); SpriteOfSpaceShipBullet.setTexture(TextureOfSpaceShipBullet); } // Plus Sprite
		void setSprite(sf::Sprite& sprite) { SpriteOfSpaceShipBullet = sprite; }
		void centerInWindow(sf::Vector2f WindowSize) { POS.y = WindowSize.y / 2; POS.x = WindowSize.x / 2; }
		void setSpaceShipBulletPosition(double X, double Y) { POS.x = X; POS.y = Y; SpriteOfSpaceShipBullet.setPosition(POS.x, POS.y); }
		void scaleSpaceShipBullet(int scx, int scy) { SpriteOfSpaceShipBullet.setScale(scx, scy); }

		sf::Texture* getSpaceShipBulletTexture() { return &TextureOfSpaceShipBullet; }
		sf::Sprite* getSpaceShipBulletSprite() { return &SpriteOfSpaceShipBullet; }
		sf::Vector2f getSpaceShipBulletPosition() { return sf::Vector2f(POS.x, POS.y); }
	};
}
#endif