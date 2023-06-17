#pragma once

#define NULL 0

#include "IGenerator.h"

int randomInRange(int lower, int upper);

class BSPGenerator : public IGenerator {
private:
	int depth;

	TreeNode* buildTree(int depth, int x, int y, int h, int w);

	void generateRoomForNode(TreeNode* node);
	void connectRoomsForNode(TreeNode* node);

public:
	BSPGenerator(int h = 50, int w = 50, int depth = 4, int chestCount = 5, int itemCount = 2);
	~BSPGenerator();

	char** generateLayout() override;

	int getNumItems() override;
};