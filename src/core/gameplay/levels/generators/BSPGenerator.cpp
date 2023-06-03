#include "BSPGenerator.h"

#include <stdlib.h>
#include <iostream>

int randomInRange(int lower, int upper) {
	if (lower == upper)
		return lower;

	return (rand() % (upper - lower)) + lower;
}

BSPGenerator::BSPGenerator(int h, int w, int depth, int chestCount, int itemCount) : IGenerator(h, w, chestCount, itemCount) {
	this->depth = depth;
}

BSPGenerator::~BSPGenerator() {
	IGenerator::~IGenerator();
}

TreeNode* BSPGenerator::buildTree(int depth, int x, int y, int h, int w) {
	if (depth == 0) {
		return NULL;
	}

	auto node = new TreeNode(x, y, h, w);

	int x1, x2, w1, w2, y1, y2, h1, h2;

	int divide;

	bool done = false;

	while (!done) {
		float r1_h_ratio;
		float r1_w_ratio;
		float r2_h_ratio;
		float r2_w_ratio;
		int r1_h, r1_w;
		int r2_h, r2_w;

		if (rand() % 2) {
			// Vertical partition

			divide = randomInRange(x, w);
			r1_h = h - y;
			r1_w = divide - x;

			r2_h = h - y;
			r2_w = w - divide;

			r1_w_ratio = (float)r1_w / r1_h;
			r2_w_ratio = (float)r2_w / r2_h;

			if (r1_w_ratio < 0.45 || r2_w_ratio < 0.45) {	
				done = false;
				continue;
			} else {
				done = true;
			}

			x1 = x; x2 = divide;
			y1 = y; y2 = y;
			h1 = h; h2 = h;
			w1 = divide; w2 = w;

			/*node->l = buildTree(depth - 1, x, y, h, divide);
			node->r = buildTree(depth - 1, divide, y, h, w);*/
		} else {
			// Horizontal partition
			divide = randomInRange(y, h);

			r1_h = divide - y;
			r1_w = w - x;

			r2_h = h - divide;
			r2_w = w - x;

			r1_h_ratio = (float)r1_h / r1_w;
			r2_h_ratio = (float)r2_h / r2_w;

			if (r1_h_ratio < 0.45 || r2_h_ratio < 0.45) {
				done = false;
				continue;
			} else {
				done = true;
			}

			x1 = x; x2 = x;
			y1 = y; y2 = divide;
			h1 = divide; h2 = h;
			w1 = w; w2 = w;

			/*node->l = buildTree(depth - 1, x, y, divide, w);
			node->r = buildTree(depth - 1, x, divide, h, w);*/
		}
	}

	node->l = buildTree(depth - 1, x1, y1, h1, w1);
	node->r = buildTree(depth - 1, x2, y2, h2, w2);

	if (node->l == NULL && node->r == NULL) {
		this->generateRoomForNode(node);
	} else {
		this->connectRoomsForNode(node);
	}

	return node;
}

void BSPGenerator::generateRoomForNode(TreeNode* node) {
	int x = randomInRange(node->x + 1, node->x + (node->w - node->x) / 4);
	int y = randomInRange(node->y + 1, node->y + (node->h - node->y) / 4);
	int w = node->w - 1;
	int h = node->h - 1;

	for (int i = y; i < h; i++) {
		for (int j = x; j < w; j++) {
			layout[i][j] = EMPTY;
		}
	}
}

void BSPGenerator::connectRoomsForNode(TreeNode* node) {
	int center1X = node->l->x + (node->l->w - node->l->x) / 2;
	int center1Y = node->l->y + (node->l->h - node->l->y) / 2;

	int center2X = node->r->x + (node->r->w - node->r->x) / 2;
	int center2Y = node->r->y + (node->r->h - node->r->y) / 2;

	if (center1X == center2X) {
		if (center1Y < center2Y) {
			for (int i = center1Y; i <= center2Y; i++) {
				layout[i][center1X] = ' ';
			}
		} else {
			for (int i = center2Y; i <= center1Y; i++) {
				layout[i][center1X] = ' ';
			}
		}
	} else {
		if (center1X < center2X) {
			for (int i = center1X; i <= center2X; i++) {
				layout[center1Y][i] = ' ';
			}
		}
		else {
			for (int i = center2X; i <= center1X; i++) {
				layout[center1Y][i] = ' ';
			}
		}
	}
}

char** BSPGenerator::generateLayout() {
	this->mapTree = buildTree(this->depth, 1, 1, this->h - 1, this->w - 1);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			printf("%c", layout[i][j]);
		}
		printf("\n");
	}

	return layout;
}

