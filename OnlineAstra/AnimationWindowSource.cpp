#include "AnimationWindow.h"

#include "MainWindowHeader.h"
#include "AnimationWindow.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iterator>

#pragma region MAINCLASS_FUNC_IMPLEMENTATIONS
void MatthewsNamespace::AnimationWindow::MainWindowThreadExecution(TripleItemHolder<sf::RenderWindow, sf::Thread, AnimationWindow>& ITEM_HOLDER) {
	sf::WindowHandle handle = ITEM_HOLDER.getA()->getSystemHandle(); // Use the handle with OS specific functions
	// Main Window Settings
	ITEM_HOLDER.getA()->setActive(true);
	ITEM_HOLDER.getA()->setVerticalSyncEnabled(true);
	ITEM_HOLDER.getA()->setFramerateLimit(60);

	//////// Create a separate thread to render the textures
	std::unique_ptr<sf::Thread> ThreadRenderer = std::make_unique<sf::Thread>([&]()->void {}); // TODO: Render Textures Asynchronously
	std::unique_ptr<DoubleItemHolder<sf::RenderWindow, AnimationWindow>> CurrentHolder = std::make_unique<DoubleItemHolder<sf::RenderWindow, AnimationWindow>>(WindowPointer, this);
	RenderTextures(*CurrentHolder.get());
	//////// Temporary - work in progress


	// Display main Window
	while (ITEM_HOLDER.getA()->isOpen()) {
		sf::Event* Event = new sf::Event();
		while (ITEM_HOLDER.getA()->pollEvent(*Event)) {
			if (Event->type == sf::Event::Closed) {
				ITEM_HOLDER.getA()->close();
				break;
			}
			else if (Event->type == sf::Event::MouseButtonReleased) {
				std::cout << "Mouse button clicked\n";
				std::unique_ptr<sf::Mouse> MyMouse = std::make_unique<sf::Mouse>();
				std::cout << "XPos: " << MyMouse.get()->getPosition(*WindowPointer).x << " ; YPos: "
					<< MyMouse.get()->getPosition(*WindowPointer).y << "\n";
			}
			
			else if (Event->type == sf::Event::KeyPressed) {
				if (Event->key.code == sf::Keyboard::Escape) { // Exits on ESC pressed
					ITEM_HOLDER.getA()->close();
					break;
				}
			}
			else if (Event->type == sf::Event::TextEntered) {
				if (Event->text.unicode == 'w') {} // Keyboard input control here
			}
		}
		// Check for continuous key presses
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			SpaceShip1.MoveLeft();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			SpaceShip1.MoveRight();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			SpaceShip1.MoveUp();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			SpaceShip1.MoveDown();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			SpaceShip1.Shoot();
		}

		std::free(Event);
		MatthewsNamespace::AnimationWindow::DrawInsideMainWindow(ITEM_HOLDER.getA(), ITEM_HOLDER.getB(), ITEM_HOLDER.getC());
	}
}
void MatthewsNamespace::AnimationWindow::DrawInsideMainWindow(sf::RenderWindow* WINDOW, sf::Thread* WINTHREAD, MatthewsNamespace::AnimationWindow* C) {
	WINDOW->clear(sf::Color::Red);
	WINDOW->draw(BackGround->SPRITE);

	WINDOW->draw(*SpaceShip1.getSpaceShipSprite()); // Draw the first spaceship
	for (int i{}; i < SpaceShip1.BulletDeque.size(); ++i) {
		WINDOW->draw(*SpaceShip1.BulletDeque[i]->getSpaceShipBulletSprite());
		if (SpaceShip1.BulletDeque.at(i) != NULL) {
			SpaceShip1.BulletDeque[i]->setSpaceShipBulletPosition(SpaceShip1.BulletDeque[i]->getSpaceShipBulletPosition().x,
				SpaceShip1.BulletDeque[i]->getSpaceShipBulletPosition().y - 30);
		}
	}
	for (unsigned int i{}; i < SpaceShip1.BulletDeque.size(); i++) {
		if (SpaceShip1.BulletDeque.at(i)->getSpaceShipBulletPosition().y < 0) {
			SpaceShipBullet* it = SpaceShip1.BulletDeque.at(i);
			delete(it);
			it = NULL;
		}
			
	}

	WINDOW->display();
}
void MatthewsNamespace::AnimationWindow::RenderTextures(DoubleItemHolder<sf::RenderWindow, AnimationWindow> ITEM_HOLDER) {
	// Inside a separate thread -> Background
	BackGround = std::make_unique<ImageToBeDrawn>();
	BackGround->TEXTURE.loadFromFile("SpaceWallpaper.jpg");
	BackGround->SPRITE.setTexture(BackGround->TEXTURE);
	BackGround->SPRITE.setScale(0.4, 0.4);

	// SpaceShip
	SpaceShip1.getSpaceShipTexture()->loadFromFile("SpaceshipNo1.png");
	SpaceShip1.setSpaceShipPosition(WWidth / 2, WHeight / 2);
	SpaceShip1.getSpaceShipSprite()->setTexture(*SpaceShip1.getSpaceShipTexture());
	SpaceShip1.getSpaceShipSprite()->setScale(0.2, 0.2);
	
	// Render Font for text
	GlobalWindowFont.loadFromFile("Fonts/Emulogic.ttf");
	GreetingText.setFont(GlobalWindowFont);
	GreetingText.setString("My Application");
	GreetingText.setCharacterSize(24);
	GreetingText.setFillColor(sf::Color::Blue);
	GreetingText.setStyle(sf::Text::Bold);
	GreetingText.setPosition(WWidth / 3.25, WHeight / 100);

}

#pragma endregion MAINCLASS_FUNC_IMPLEMENTATIONS