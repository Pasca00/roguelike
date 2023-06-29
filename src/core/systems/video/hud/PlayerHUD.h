#pragma once

#include "../views/View.h"

class PlayerHUD {
private:
	/*float& maxPlayerHealth;
	float& currPlayerHealth;*/

	std::shared_ptr<View> healthbarContainer;
	std::shared_ptr<View> healthbar;
	std::shared_ptr<View> energybar;

public:
	PlayerHUD(std::shared_ptr<View>& healthbarContainer, std::shared_ptr<View>& healthbar, std::shared_ptr<View>& energybar);

	std::shared_ptr<View>& getHealthbarContainer();
	std::shared_ptr<View>& getHealthbar();
	std::shared_ptr<View>& getEnergybar();

	void updateHealth(float maxPlayerHealth, float currPlayerHealth);
};