#pragma once

#define NULL 0

#include "IGenerator.h"

struct TreeNode {
	int x;
	int y;
	int h;
	int w;

	TreeNode* l;
	TreeNode* r;

	TreeNode(int x, int y, int h, int w) : x(x), y(y), h(h), w(w) {
		l = NULL;
		r = NULL;
	}
};

class BSPGenerator : public IGenerator {
private:
	int depth;

	TreeNode* buildTree(int depth, int x, int y, int h, int w);

	void generateRoomForNode(TreeNode* node);
	void connectRoomsForNode(TreeNode* node);

public:
	BSPGenerator(int h = 50, int w = 50, int depth = 4);
	~BSPGenerator();

	char** generateLayout() override;
};