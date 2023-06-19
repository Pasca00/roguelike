#pragma once

#include <functional>
#include <string>

#include "../../systems/video/views/TextView.h";
#include "../../systems/video/views/View.h";
#include "../entities/Player.h"

class Item {
private:
	std::string name;
	std::string description;

	std::shared_ptr<View> scrollView;

	std::shared_ptr<TextView> nameView;
	std::shared_ptr<TextView> descriptionView;

	bool enabled;

	float timeSinceEnabled;

public:
	Item(std::shared_ptr<Texture>& scrollTexture, std::string name, std::string description, float x, float y);

	void update(float dtime);

	void enable();
	void disable();
	bool isEnabled();
	bool canPickup();

	std::shared_ptr<View>& getScrollView();

	std::shared_ptr<TextView>& getNameView();
	std::shared_ptr<TextView>& getDescriptionView();

	std::function<void(std::unique_ptr<Player> const&)> onPickup;
};