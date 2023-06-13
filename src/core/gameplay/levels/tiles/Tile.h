#pragma once

#include "../../../systems/video/views/View.h"

class Tile {
private:
	std::shared_ptr<View> view;

	std::shared_ptr<IView> decoration;

public:
	Tile(std::shared_ptr<View>& view);

	std::shared_ptr<View>& getView();

	void addDecoration(std::shared_ptr<IView> v);
	std::shared_ptr<IView>& getDecoration();

	char type;
};