#include "BoomBox.h"


bool MatthewsNamespace::BoomBox::IS_SOUND_ENABLED = 1;
bool MatthewsNamespace::BoomBox::IS_MUSIC_ENABLED = 1;
std::unique_ptr<MatthewsNamespace::BoomBox::DJ> MatthewsNamespace::BoomBox::LocalDJ = std::make_unique<DJ>();
std::vector<std::string> MatthewsNamespace::BoomBox::WavFilesFromDirectory;

void MatthewsNamespace::BoomBox::MainWindowThreadExecution(TripleItemHolder<sf::RenderWindow, sf::Thread, BoomBox>& ITEM_HOLDER) {
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
	std::unique_ptr<DoubleItemHolder<sf::RenderWindow, BoomBox>> CurrentHolder = std::make_unique<DoubleItemHolder<sf::RenderWindow, BoomBox>>(WindowPointer, this);
	RenderTextures(*CurrentHolder.get());
	//////// Temporary - work in progress


	// Display main Window
	while (ITEM_HOLDER.getA()->isOpen()) {
		sf::Event* Event = new sf::Event();
		while (ITEM_HOLDER.getA()->pollEvent(*Event)) {
			if (Event->type == sf::Event::Closed) {
				ITEM_HOLDER.getA()->close();
				BoomBox::WindowSoundEffect();
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
					BoomBox::WindowSoundEffect();
					break;
				}
			}
			else if (Event->type == sf::Event::TextEntered) {
				if (Event->text.unicode < 128)
					break;
			}
		}
		std::free(Event);
		MatthewsNamespace::BoomBox::DrawInsideMainWindow(ITEM_HOLDER.getA(), ITEM_HOLDER.getB(), ITEM_HOLDER.getC());
	}
}
void MatthewsNamespace::BoomBox::DrawInsideMainWindow(sf::RenderWindow* WINDOW, sf::Thread* WINTHREAD, BoomBox* C) {
	WINDOW->clear(sf::Color::Red);
	WINDOW->draw(BackGround->SPRITE);
	WINDOW->draw(Speaker1.get()->SPRITE); // The first Speaker
	WINDOW->draw(Speaker2.SPRITE); // The second Speaker
 	
	WINDOW->display();
}
void MatthewsNamespace::BoomBox::RenderTextures(DoubleItemHolder<sf::RenderWindow, BoomBox> ITEM_HOLDER) {
	// Inside a separate thread -> Background
	BackGround = std::make_unique<ImageToBeDrawn>();
	BackGround->TEXTURE.loadFromFile("TechWall.jpg");
	BackGround->SPRITE.setTexture(BackGround->TEXTURE);
	BackGround->SPRITE.setScale(0.5, 0.5);

	Speaker1 = std::make_unique<ImageToBeDrawn>();
	Speaker1->TEXTURE.loadFromFile("Speaker.png");
	Speaker1->SPRITE.setTexture(Speaker1->TEXTURE);
	Speaker1->SPRITE.setScale(0.1, 0.1);
	Speaker1->SPRITE.setPosition(ITEM_HOLDER.getA()->getSize().x / 2 - 250, ITEM_HOLDER.getA()->getSize().y / 2 - 100);
	
	Speaker2.SPRITE.setTexture(Speaker1->TEXTURE);
	Speaker2.SPRITE.setScale(0.1, 0.1);
	Speaker2.SPRITE.setPosition(ITEM_HOLDER.getA()->getSize().x / 2, ITEM_HOLDER.getA()->getSize().y / 2 - 100);
}