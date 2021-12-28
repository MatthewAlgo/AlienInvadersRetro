#include "SpaceShip.h"
#include "EnemySpaceShip.h"
#include "AnimationWindow.h"
#include "BoomBox.h"

void MatthewsNamespace::SpaceShip::Shoot() {
	// Load the bullet -> The bullet receives some speed of +10 per frame
	// The initial bullet / shuttle
	SpaceShipBullet CurrentBullet;
	BulletDeque.push_back(new SpaceShipBullet(CurrentBullet));
	BulletDeque.back()->setTexture("NeonVertical.png");
	BulletDeque.back()->scaleSpaceShipBullet(0.25, 0.25);
	BulletDeque.back()->setSpaceShipBulletPosition(POS.x + 13, POS.y - 40); // Front Bullet
}

int MatthewsNamespace::SpaceShip::IterateThroughBullets(sf::RenderWindow* WINDOW, std::vector<EnemySpaceShip*>& Enemies) {
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
			if (BulletDeque.at(i)->getSpaceShipBulletPosition().x > Enemies.at(j)->getSpaceShipPosition().x - 40 && BulletDeque.at(i)->getSpaceShipBulletPosition().x < Enemies.at(j)->getSpaceShipPosition().x + 20)
				if (BulletDeque.at(i)->getSpaceShipBulletPosition().y > Enemies.at(j)->getSpaceShipPosition().y - 40 && BulletDeque.at(i)->getSpaceShipBulletPosition().y < Enemies.at(j)->getSpaceShipPosition().y + 20) {
					// Draw Explosion on Impact
					ExplosionSprite.setPosition(BulletDeque.at(i)->getSpaceShipBulletPosition().x, BulletDeque.at(i)->getSpaceShipBulletPosition().y);
					WINDOW->draw(ExplosionSprite);
					scoreByPlayer++;
					// Decrease life + set death behavior
					(*Enemies.at(j)->getLife()) -= *BulletDeque.at(i)->getDamage();
					if (*Enemies.at(j)->getLife() == 0) {
						BoomBox::WindowCollisionEffect(); // Will generate a collision sound effect
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
	return scoreByPlayer * 100;

}
void MatthewsNamespace::SpaceShip::FreeUpMemoryBullets() {
	for (unsigned int i{}; i < this->BulletDeque.size(); i++) { // Manage and free up the memory
		if (this->BulletDeque.at(i)->getSpaceShipBulletPosition().y < -300) { // If the bullet is offscreen
			SpaceShipBullet* it = this->BulletDeque.at(i);
			delete it; it = nullptr; this->BulletDeque.erase(this->BulletDeque.begin() + i);
		}
	}
}