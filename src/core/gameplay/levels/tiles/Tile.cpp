#include "Tile.h"

Tile::Tile(std::shared_ptr<View>& view) {
	this->view = view;
	this->decoration = nullptr;
}

std::shared_ptr<View>& Tile::getView() {
	return this->view;
}

void Tile::addDecoration(std::shared_ptr<IView> v) {
	this->decoration = v;
}

std::shared_ptr<IView>& Tile::getDecoration() {
	return this->decoration;
}
