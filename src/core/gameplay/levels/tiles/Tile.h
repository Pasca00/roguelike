#pragma once

#include "../../../systems/video/views/View.h"

class Tile {
private:
	std::shared_ptr<View> view;

public:
	Tile(std::shared_ptr<View>& view);

	std::shared_ptr<View>& getView();
};