#include "PlayerHUD.h"

PlayerHUD::PlayerHUD(
	std::shared_ptr<View>& healthbarContainer,
	std::shared_ptr<View>& healthbar,
	std::shared_ptr<View>& energybar)
	: healthbarContainer(healthbarContainer), healthbar(healthbar), energybar(energybar) {

}

std::shared_ptr<View>& PlayerHUD::getHealthbarContainer() {
	return this->healthbarContainer;
}

std::shared_ptr<View>& PlayerHUD::getHealthbar() {
	return this->healthbar;
}

std::shared_ptr<View>& PlayerHUD::getEnergybar() {
	return this->energybar;
}

void PlayerHUD::updateHealth(float maxPlayerHealth, float currPlayerHealth) {
	float currPercentage = currPlayerHealth / maxPlayerHealth;

	this->healthbar->setWidth(this->healthbar->getWidth() * currPercentage);
}