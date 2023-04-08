#include "LevelManager.h"

LevelManager::LevelManager(std::shared_ptr<CastleStage>& castleStage, int tileSize) 
	: castleStage(castleStage) {
	this->tileSize = tileSize;
}

std::vector<std::vector<std::shared_ptr<Tile>>> LevelManager::getTileMap() {
	return this->castleStage->getTileMap();
}