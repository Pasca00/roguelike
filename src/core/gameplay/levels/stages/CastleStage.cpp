#include "CastleStage.h"

#include <stack>

#include "../../../gameplay/inventory/StatusEffect.h"

CastleStage::CastleStage(
	std::shared_ptr<IGenerator>& generator,
	std::shared_ptr<TextureManager>& textureManager,
	std::shared_ptr<SoundSystem>& soundSystem,
	int tileSize
) : IStage(generator, textureManager, soundSystem, tileSize) {
	this->texturesDir = Paths::TILESETS_DIR +  "/castle/";
}

CastleStage::~CastleStage() { }

void CastleStage::generateStage() {
	this->loadTextures();

	auto stageTemplate = this->generateTemplate();
	auto stageTiles = this->findTileType(stageTemplate);
	
	this->createTileMap(stageTemplate, stageTiles);

	auto smallestRoom = this->findSmallestRoom();
	for (int i = smallestRoom->y; i < smallestRoom->h; i++) {
		for (int j = smallestRoom->x; j < smallestRoom->w; j++) {
			if (stageTemplate[i][j] == IGenerator::EMPTY) {
				auto& playerTileView = tileMap[i][j]->getView();
				this->playerStartPosX = playerTileView->getX() + this->tileSize / 2;
				this->playerStartPosY = playerTileView->getY() + this->tileSize / 2;
			}
		}
	}

	this->placeDoors();
	this->placeItems();
	this->placeEnemiesStartPositions();
}

void CastleStage::update(float dtime) {
	for (auto& t : this->animations) {
		t->update(dtime);
	}

	for (auto& i : this->items) {
		if (i->isEnabled())
			i->update(dtime);
	}
}

char** CastleStage::findTileType(char** stageTemplate) {
	int w = generator->getW();
	int h = generator->getH();
	char** stageTiles = new char* [h];
	for (int i = 0; i < h; i++) {
		stageTiles[i] = new char[w];

		for (int j = 0; j < w; j++) {
			if (stageTemplate[i][j] == IGenerator::EMPTY) {
				if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_top;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_left;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_right;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_bottom;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_top_right;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_top_left;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_bottom_left;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_bottom_right;
				} else if (
					stageTemplate[i - 1][j] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_horizontal;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i][j + 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_vertical;
				} else if (
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = top_wall;
				} else if (
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = bottom_wall;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = left_wall;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = right_wall;
				} else {
					stageTiles[i][j] = no_wall;
				}
			}
			else if (i < h - 1 && stageTemplate[i][j] == IGenerator::WALL && stageTemplate[i + 1][j] == IGenerator::EMPTY) {
				if (
					stageTemplate[i + 1][j - 1] == IGenerator::WALL 
					&& stageTemplate[i + 1][j + 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = wall_corridor_deadend;
				} else if (stageTemplate[i + 1][j - 1] == IGenerator::WALL) {
					stageTiles[i][j] = wall_left;

					if (j >= 1) 
						stageTiles[i][j - 1] = ceil_right;
					if (i >= 1 && j >= 1) 
						stageTiles[i - 1][j - 1] = ceil_right;
				} else if (stageTemplate[i + 1][j + 1] == IGenerator::WALL) {
					stageTiles[i][j] = wall_right;
					
					if (j < w - 1)
						stageTiles[i][j + 1] = ceil_left;
					if (j < w - 1 && i >= 1)
						stageTiles[i - 1][j + 1] = ceil_left;
				} else {
					stageTiles[i][j] = wall_base;
				}

				/*if (i - 2 >= 0 && stageTiles[i - 2][j]) {
					stageTiles[i - 2][j] = ceil_bottom;
				}*/
			}
			else {
				if (i == 0 || i == h - 1 || j == 0 || j == w - 1) {
					stageTiles[i][j] = ceil_base;
					continue;
				}

				if (
					stageTemplate[i][j - 1] == IGenerator::EMPTY &&
					stageTemplate[i][j + 1] == IGenerator::EMPTY &&
					stageTemplate[i - 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corridor_deadend_top;
				}
				else if (
					stageTemplate[i][j - 1] == IGenerator::EMPTY &&
					stageTemplate[i - 1][j] == IGenerator::EMPTY &&
					stageTemplate[i + 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corridor_deadend_left;
				}
				else if (
					stageTemplate[i][j + 1] == IGenerator::EMPTY &&
					stageTemplate[i - 1][j] == IGenerator::EMPTY &&
					stageTemplate[i + 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corridor_deadend_right;
				}
				else if (
					stageTemplate[i][j - 1] == IGenerator::EMPTY &&
					stageTemplate[i][j + 1] == IGenerator::EMPTY &&
					stageTemplate[i + 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corridor_deadend_bottom;
				}
				else if (
					stageTemplate[i][j + 1] == IGenerator::EMPTY &&
					stageTemplate[i - 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corner_top_right;
				}
				else if (
					stageTemplate[i][j - 1] == IGenerator::EMPTY &&
					stageTemplate[i - 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corner_top_left;
				}
				else if (
					stageTemplate[i][j - 1] == IGenerator::EMPTY &&
					stageTemplate[i + 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corner_bottom_left;
				}
				else if (
					stageTemplate[i][j + 1] == IGenerator::EMPTY &&
					stageTemplate[i + 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corner_bottom_right;
				}
				else if (
					stageTemplate[i - 1][j] == IGenerator::EMPTY &&
					stageTemplate[i + 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corridor_horizontal;
				}
				else if (
					stageTemplate[i][j - 1] == IGenerator::EMPTY &&
					stageTemplate[i][j + 1] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_corridor_vertical;
				}
				else if (
					stageTemplate[i - 1][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_top;
				}
				else if (
					i < h - 3
					&& stageTemplate[i + 2][j] == IGenerator::WALL
					&& stageTemplate[i + 3][j] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_bottom;
				}
				else if (
					stageTemplate[i][j - 1] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_left;
				}
				else if (
					stageTemplate[i][j + 1] == IGenerator::EMPTY
					) {
					stageTiles[i][j] = ceil_right;
				}
				else {
					stageTiles[i][j] = ceil_base;
				}
			}
		}
	}

	return stageTiles;
}

void CastleStage::loadTextures() {
	this->floorTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "floor.png",
		{
			1, // corridor T down
			1, // corridor T up
			1, // corridor T left
			1, // corridor T right
			2, // corridor vertical
			2, // corridor horizontal
			1, // corridor dead-end bottom
			1, // corridor dead-end right
			1, // corridor dead-end top
			1, // corridor dead-end left
			8, // no wall
			3, // right wall
			3, // left wall
			3, // bottom wall
			3, // top wall
			2, // corner bottom-right
			2, // corner bottom-left
			2, // corner top-right
			2, // corner top-left
		},
		16,
		16
	);

	this->wallTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "walls.png",
		{
			2, // stairway
			1, // corridor dead-end
			4, // base wall
			1, // wall right
			1, // wall left
		},
		16,
		32
	);

	this->ceilingTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "ceiling.png",
		{
			2, // ceiling corridor vertical
			2, // ceiling corridor horizontal
			1, // ceiling corridor deadend bottom 
			1, // ceiling corridor deadend right
			1, // ceiling corridor deadend top
			1, // ceiling corridor deadend left
			3, // ceiling right
			3, // ceiling bottom
			3, // ceiling top
			3, // ceiling left
			1, // ceiling corner bottom left
			1, // ceiling corner bottom right
			1, // ceiling corner top right
			1, // ceiling corner top left
			1, // ceiling base
		},
		16,
		16
	);

	this->wallDecorations = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "wall_decoration.png",
		{
			6 // flags
		},
		16,
		32
	)[0];

	this->floorDecorations = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "floor_decoration.png",
		{
			3
		},
		16,
		16
	)[0];

	this->torchTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "torch.png",
		{
			3
		},
		16,
		16
	)[0];

	this->chestTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "chest.png",
		{
			2
		},
		16,
		16
	)[0];

	this->doorTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "iron_door.png",
		{
			2
		},
		16,
		32
	)[0];

	this->brasierTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "brasier.png",
		{
			3
		},
		16,
		32
	)[0];

	this->scrollsTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "scrolls.png",
		{
			6
		},
		16,
		16
	)[0];
}

void CastleStage::createTileMap(char** stageTemplate, char** stageTiles) {
	int h = this->generator->getH();
	int w = this->generator->getW();

	this->tileMap = std::vector<std::vector<std::shared_ptr<Tile>>>(
		h,
		std::vector<std::shared_ptr<Tile>>(w, nullptr)
	);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			float tileX = j * this->tileSize;
			float tileY = (h - i - 1) * this->tileSize;

			int tileType = stageTiles[i][j];

			if (stageTemplate[i][j] == IGenerator::EMPTY) {
				auto texture = this->floorTextures[tileType][rand() % this->floorTextures[tileType].size()];
				auto view = std::make_shared<View>(texture, tileX, tileY, this->tileSize / 16);
				this->tileMap[i][j] = std::make_shared<Tile>(view);

				if (rand() % 100 < 2) {
					auto t = this->floorDecorations[rand() % this->floorDecorations.size()];
					auto v = std::make_shared<View>(t, tileX, tileY, this->tileSize / 16);
					this->tileMap[i][j]->addDecoration(v);
				}

			} else if (i < h - 1 && stageTemplate[i][j] == IGenerator::WALL && stageTemplate[i + 1][j] == IGenerator::EMPTY) {
				int k = rand() % this->wallTextures[tileType].size();
				auto texture = this->wallTextures[tileType][k];
				auto view = std::make_shared<View>(texture, tileX, tileY, this->tileSize / 16);
				this->tileMap[i][j] = std::make_shared<Tile>(view);

				if (rand() % 100 < 2) {
					auto t = this->wallDecorations[rand() % this->wallDecorations.size()];
					auto v = std::make_shared<View>(t, tileX, tileY, this->tileSize / 16);
					this->tileMap[i][j]->addDecoration(v);
				} else if (rand() % 100 < 6) {
					// Add torch to wall
					float offset = rand() % 10 / 100.f;
					auto v = std::make_shared<AnimationView>(
						this->torchTextures, 
						true, 
						0.15f + offset, 
						tileX, 
						tileY + this->tileSize, 
						this->tileSize / 16
					);

					this->lightPositions.push_back(glm::vec2(tileX, tileY));

					this->animations.push_back(v);
					this->tileMap[i][j]->addDecoration(std::static_pointer_cast<IView>(v));
				}

			} else {
				auto texture = this->ceilingTextures[tileType][rand() % this->ceilingTextures[tileType].size()];
				auto view = std::make_shared<View>(texture, tileX, tileY, this->tileSize / 16);
				this->tileMap[i][j] = std::make_shared<Tile>(view);
			}

			this->tileMap[i][j]->type = stageTemplate[i][j];
		}
	}

	auto texture = this->floorTextures[0][0];
	auto view = std::make_shared<View>(texture, 0, 0, 2);
	this->tileMap[0][0] = std::make_shared<Tile>(view);
}

void CastleStage::placeItems() {
	int nItems = this->generator->getNumItems();

	auto playerRoom = this->getRoomForCoords(this->playerStartPosX, this->playerStartPosY);

	int iItem = (playerRoom->h - playerRoom->y) / 2 + playerRoom->y;
	int jItem = (playerRoom->w - playerRoom->x) / 2 + playerRoom->x;

	auto t = this->tileMap[iItem][jItem];
	auto& v = t->getView();

	auto x = v->getX();
	auto y = v->getY();

	auto dec = std::make_shared<View>(this->chestTextures[0], x, y, this->tileSize / 16);
	this->tileMap[iItem][jItem]->addDecoration(dec);
	this->tileMap[iItem][jItem]->type = IGenerator::WALL;

	auto item = this->makeRandomItem(iItem, jItem);

	auto interactable = std::make_unique<IInteractable>(
		"INTERACT",
		[
			dec,
			item,
			&chestTextures = this->chestTextures, 
			&soundSystem = this->soundSystem, 
			&tile = tileMap[iItem][jItem]
		]() {
			dec->setTexture(chestTextures[1]);

			soundSystem->playSound("chest");
			
			item->enable();
		},
		x - interactRange,
		y - interactRange,
		(v->getWidth() + interactRange) * v->getSize(),
		(v->getHeight() + interactRange) * v->getSize(),
		1,
		false
	);

	this->interactables.push_back(std::move(interactable));

	this->items.push_back(item);
}

void CastleStage::placeDoors() {
	for (int i = 0; i < this->tileMap.size(); i++) {
		for (int j = 0; j < this->tileMap[i].size(); j++) {
			if (tileMap[i][j]->type != IGenerator::EMPTY) {
				continue;
			}

			if (
				this->tileMap[i][j - 1]->type == IGenerator::WALL 
				&& this->tileMap[i][j + 1]->type == IGenerator::WALL
				&& (this->tileMap[i + 1][j - 1]->type == IGenerator::EMPTY 
				|| this->tileMap[i + 1][j + 1]->type == IGenerator::EMPTY)
			) {
				auto t = tileMap[i][j]->getView();
				auto door = std::make_shared<View>(this->doorTextures[0], t->getX(), t->getY(), this->tileSize / 16);
				this->tileMap[i][j]->addDecoration(door);
				this->tileMap[i][j]->type = IGenerator::DOOR_VERTICAL;

				auto interactable = std::make_unique<IInteractable>(
					"INTERACT",
					[&tileType = tileMap[i][j]->type, door, &doorTextures = doorTextures, &soundSystem = soundSystem]() {
						if (tileType == IGenerator::DOOR_VERTICAL) {
							tileType = IGenerator::EMPTY;
							door->setTexture(doorTextures[1]);

							soundSystem->playSound("iron_door");
						} else {
							tileType = IGenerator::DOOR_VERTICAL;
							door->setTexture(doorTextures[0]);

							soundSystem->playSound("iron_door");
						}
					},
					t->getX() - interactRange,
					t->getY() - interactRange,
					t->getWidth() * t->getSize() + interactRange + 64.f,
					t->getHeight()* t->getSize() + interactRange + 64.f,
					-1,
					false
				);

				this->interactables.push_back(std::move(interactable));
			} else if (
				this->tileMap[i - 1][j]->type == IGenerator::WALL
				&& this->tileMap[i + 1][j]->type == IGenerator::WALL
				&& (this->tileMap[i - 1][j - 1]->type == IGenerator::EMPTY
					|| this->tileMap[i + 1][j - 1]->type == IGenerator::EMPTY)
			) {
				auto t = tileMap[i][j]->getView();
				auto door = std::make_shared<View>(this->doorTextures[1], t->getX(), t->getY(), this->tileSize / 16);
				this->tileMap[i][j]->addDecoration(door);
				this->tileMap[i][j]->type = IGenerator::DOOR_HORIZONTAL;

				auto interactable = std::make_unique<IInteractable>(
					"INTERACT",
					[&tileType = tileMap[i][j]->type, door, &doorTextures = doorTextures, &tileSize = tileSize, &soundSystem = soundSystem]() {
						if (tileType == IGenerator::DOOR_HORIZONTAL) {
							tileType = IGenerator::EMPTY;
							door->setTexture(doorTextures[0]);
							door->setY(door->getY() + tileSize);

							soundSystem->playSound("iron_door");
						} else {
							tileType = IGenerator::DOOR_HORIZONTAL;
							door->setTexture(doorTextures[1]);
							door->setY(door->getY() - tileSize);

							soundSystem->playSound("iron_door");
						}
					},
					t->getX() - interactRange,
					t->getY() - interactRange,
					t->getWidth() * t->getSize() + interactRange,
					t->getHeight() * t->getSize() + interactRange,
					-1,
					false
					);

				this->interactables.push_back(std::move(interactable));
			}
		}
	}
}

void CastleStage::placeEnemiesStartPositions() {
	TreeNode* head = this->getMapTree();

	auto playerRoom = this->getRoomForCoords(this->playerStartPosX, this->playerStartPosY);

	std::stack<TreeNode*> stack;
	while (head != NULL || !stack.empty()) {
		while (head != NULL) {
			stack.push(head);
		
			head = head->l;
		}

		head = stack.top();
		stack.pop();

		if (head->l == NULL && head->r == NULL && head != playerRoom) {
			this->placeEnemiesInRoom(head);
		}

		head = head->r;
	}
}

TreeNode* CastleStage::getRoomForCoords(float x, float y) {
	int i = this->tileMap.size() - 1 - y / this->tileSize;
	int j = x / this->tileSize;

	return TreeNode::findRoomForCoords(this->generator->mapTree, i, j);
}

void CastleStage::placeEnemiesInRoom(TreeNode* room) {
	int roomW = room->w - room->x;
	int roomH = room->h - room->y;

	int surface = roomW * roomH;
	int surfaceChanceMod = surface / 100;

	int sign;

	if (surfaceChanceMod >= 5) {
		// If we have a lot of enemies we can only decrease
		sign = rand() % 2 == 0 ? 0 : -1;
	} else if (surfaceChanceMod <= 1) {
		sign = rand() % 2 == 0 ? 0 : 1;
	} else {
		sign = rand() % 2 == 0 ? -1 : 1;
	}

	// Can add or remove one enemy max
	int numEnemies = surface / 100 + sign * rand() % 2;

	if (numEnemies == 0) {
		return;
	}

	int numHeavyEnemies;
	int numLightEnemies;

	if (numEnemies == 1) {
		numHeavyEnemies = rand() % 2 == 0 ? 1 : 0;
	} else {
		numHeavyEnemies = rand() % (numEnemies - numEnemies / 2);
	}

	numLightEnemies = numEnemies - numHeavyEnemies;

	int groupSpawnChance;
	int spawnChance = 10;
	char enemyType;


	std::vector<char> enemies;
	while (numHeavyEnemies != 0 || numLightEnemies != 0) {
		if (rand() % 2) {
			if (numHeavyEnemies > 0) {
				enemies.push_back('H');
				numHeavyEnemies--;
			} else {
				enemies.push_back('L');
				numLightEnemies--;
			}
		} else {
			if (numLightEnemies > 0) {
				enemies.push_back('L');
				numLightEnemies--;
			} else {
				enemies.push_back('H');
				numHeavyEnemies--;
			}
		}
	}

	groupSpawnChance = 2;
	spawnChance = 5;


	while (!enemies.empty()) {
		int i = randomInRange(room->y, room->h);
		int j = randomInRange(room->x, room->w);

		if (tileMap[i][j]->type != IGenerator::EMPTY) {
			continue;
		}

		if (tileMap[i + 1][j]->type == IGenerator::WALL && tileMap[i - 1][j]->type == IGenerator::WALL
			|| tileMap[i][j + 1]->type == IGenerator::WALL && tileMap[i][j - 1]->type == IGenerator::WALL) {
			continue;
		}

		if (rand() % 100 + 1 < groupSpawnChance) {
			auto v = tileMap[i][j]->getView();
			auto dec = std::make_shared<AnimationView>(this->brasierTextures, true, 0.15f, v->getX(), v->getY(), this->tileSize / 16.f);
			tileMap[i][j]->addDecoration(dec);
			tileMap[i][j]->type = IGenerator::WALL;

			this->lightPositions.push_back(glm::vec2(v->getX(), v->getY()));

			this->animations.push_back(dec);

			if (tileMap[i][j + 1]->type == IGenerator::EMPTY) {
				this->placeEnemy(i, j + 1, enemies.back());
			}
			else {
				this->placeEnemy(i, j - 1, enemies.back());
			}
			enemies.pop_back();

			if (enemies.empty())
				return;

			if (tileMap[i + 1][j]->type == IGenerator::EMPTY) {
				this->placeEnemy(i + 1, j, enemies.back());
			}
			else {
				this->placeEnemy(i - 1, j, enemies.back());
			}
			enemies.pop_back();

			if (enemies.empty())
				return;

			groupSpawnChance = 5;
		} else if (rand() % 100 + 1 < spawnChance) {
			this->placeEnemy(i, j, enemies.back());
			enemies.pop_back();

			if (enemies.empty())
				return;

			spawnChance = 2;
		} else {
			spawnChance += 5;
			groupSpawnChance += 1;
		}
	}

	//while (!enemies.empty()) {
	//	if (enemies.size() == 1) {
	//		groupSpawnChance = 50;
	//		spawnChance = 50;
	//	}

	//	int randOffsetY = rand() % 3;
	//	int randOffsetX = rand() % 3;
	//	for (int i = room->y + randOffsetY; i < room->h; i++) {
	//		for (int j = room->x + randOffsetX; j < room->w; j++) {
	//			if (tileMap[i][j]->type == IGenerator::EMPTY) {
	//				int pickChance = rand() % 100 + 1;
	//				if (pickChance <= spawnChance) {
	//					
	//				} else if (pickChance <= spawnChance + groupSpawnChance) {
	//					
	//				}

	//				
	//			}
	//		}
	//	}
	//}
}

TreeNode* CastleStage::findSmallestRoom() {
	TreeNode* min = NULL;
	int minSurface = 10000;

	TreeNode* head = this->getMapTree();

	std::stack<TreeNode*> stack;
	while (head != NULL || !stack.empty()) {
		while (head != NULL) {
			stack.push(head);

			head = head->l;
		}

		head = stack.top();
		stack.pop();

		if (head->l == NULL && head->r == NULL) {
			int roomW = head->w - head->x;
			int roomH = head->h - head->y;

			int surface = roomW * roomH;

			if (surface < minSurface) {
				minSurface = surface;
				min = head;
			}
		}

		head = head->r;
	}

	return min;
}

void CastleStage::placeEnemy(int i, int j, char enemyType) {
	auto v = tileMap[i][j]->getView();
	
	float x = v->getX() + this->tileSize / 2;
	float y = v->getY() + this->tileSize / 2;

	if (enemyType == 'H') {
		this->enemyHeavyPositions.push_back({ x, y });

		return;
	}

	this->enemyLightPositions.push_back({ x, y });
}

std::shared_ptr<Item> CastleStage::makeRandomItem(int i, int j) {
	int randItem = rand() % this->itemPool.size();
	auto randTexture = this->scrollsTextures[rand() % this->scrollsTextures.size()];

	auto& tile = this->tileMap[i][j]->getView();

	float x = tile->getX();
	float y = tile->getY() + 48.f;

	auto& itemId = this->itemPool[randItem];

	std::shared_ptr<Item> item;

	switch (itemId) {
	case ItemIds::RUNE_OF_FIRE:
		item = std::make_shared<Item>(
			randTexture,
			"Rune of Fire",
			"Set Your Enemies Ablaze",
			x, y
		);

		item->onPickup = [](std::unique_ptr<Player> const& p) {
			//p->getMovableComponent()->onHitApplies.push_back(EffectName::BURN);
		};

		break;

	case ItemIds::RUNE_OF_ICE:
		item = std::make_shared<Item>(
			randTexture,
			"Rune of Ice",
			"Freeze Your Enemies",
			x, y
		);

		item->onPickup = [](std::unique_ptr<Player> const& p) {
			//p->getMovableComponent()->onHitApplies.push_back(EffectName::SLOW);
		};

		break;

	case ItemIds::GHOSTWALK:
		item = std::make_shared<Item>(
			randTexture,
			"Ghostwalk",
			"Speed Up + Walk Through Walls",
			x, y
		);

		item->onPickup = [](std::unique_ptr<Player> const&) {};

		break;
	}

	for (auto it = this->itemPool.begin(); it != this->itemPool.end(); it++) {
		if (*it == itemId) {
			this->itemPool.erase(it);
			break;
		}
	}

	return item;
}