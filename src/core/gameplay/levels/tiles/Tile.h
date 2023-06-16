#pragma once

#include <vector>

#include "../../../systems/video/views/View.h"

class Tile {
private:
	std::shared_ptr<View> view;

	std::shared_ptr<IView> decoration;

	std::vector<std::shared_ptr<IView>> additionalViews;

public:
	Tile(std::shared_ptr<View>& view);

	std::shared_ptr<View>& getView();

	void addDecoration(std::shared_ptr<IView> v);
	std::shared_ptr<IView>& getDecoration();

	void addAdditionalView(std::shared_ptr<IView>& v);
	std::vector<std::shared_ptr<IView>>& getAdditionalViews();
	void clearAdditionalViews();

	char type;
};