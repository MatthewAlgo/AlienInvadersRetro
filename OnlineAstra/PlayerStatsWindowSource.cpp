#include "PlayerStatsWindow.h"


void MatthewsNamespace::PlayerStatsWindow::MainWindowThreadExecution(TripleItemHolder<sf::RenderWindow, sf::Thread, PlayerStatsWindow>& ITEM_HOLDER) {
	sf::WindowHandle handle = ITEM_HOLDER.getA()->getSystemHandle(); // Use the handle with OS specific functions
	// Main Window Settings
	ITEM_HOLDER.getA()->setActive(true);
	ITEM_HOLDER.getA()->setVerticalSyncEnabled(true);
	ITEM_HOLDER.getA()->setFramerateLimit(60);

	//////// Create a separate thread to render the textures
	std::unique_ptr<sf::Thread> ThreadRenderer = std::make_unique<sf::Thread>([&]()->void {
		// TODO: Render Textures Asynchronously
		}
	);
	std::unique_ptr<DoubleItemHolder<sf::RenderWindow, PlayerStatsWindow>> CurrentHolder = std::make_unique<DoubleItemHolder<sf::RenderWindow, PlayerStatsWindow>>(WindowPointer, this);
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
				if (Event->text.unicode < 128)
					break;
			}
		}
		std::free(Event);
		MatthewsNamespace::PlayerStatsWindow::DrawInsideMainWindow(ITEM_HOLDER.getA(), ITEM_HOLDER.getB(), ITEM_HOLDER.getC());
	}
}
void MatthewsNamespace::PlayerStatsWindow::DrawInsideMainWindow(sf::RenderWindow* WINDOW, sf::Thread* WINTHREAD, PlayerStatsWindow* C) {
	WINDOW->clear(sf::Color::Red);
	WINDOW->draw(BackGround->SPRITE);

	WINDOW->display();
}
void MatthewsNamespace::PlayerStatsWindow::RenderTextures(DoubleItemHolder<sf::RenderWindow, PlayerStatsWindow> ITEM_HOLDER) {
	// Inside a separate thread -> Background
	BackGround = std::make_unique<ImageToBeDrawn>();
	BackGround->TEXTURE.loadFromFile("TechWall.jpg");
	BackGround->SPRITE.setTexture(BackGround->TEXTURE);
	BackGround->SPRITE.setScale(0.5, 0.5);
}