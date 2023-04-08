#include "Tile.h"

Tile::Tile(std::shared_ptr<View>& view) {
	this->view = view;
}

std::shared_ptr<View>& Tile::getView() {
	return this->view;
}
