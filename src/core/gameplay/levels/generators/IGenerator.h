#pragma once

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

class IGenerator {
protected:

	int chestCount;
	int itemCount;

	char** layout;
	int h;
	int w;

public:
	IGenerator(int h = 50, int w = 50, int chestCount = 5, int itemCount = 2) : h(h), w(w), chestCount(chestCount), itemCount(itemCount) {
		layout = new char* [h];
		for (int i = 0; i < h; i++) {
			layout[i] = new char[w];
			for (int j = 0; j < w; j++) {
				layout[i][j] = WALL;
			}
		}
	}

	virtual ~IGenerator() {
		/*for (int i = 0; i < h; i++) {
			delete[] layout[i];
		}
		delete[] layout;*/
	}

	virtual char** generateLayout() = 0;

	int getH() {
		return h;
	}

	int getW() {
		return w;
	}

	static const char WALL		 = '#';
	static const char EMPTY		 = ' ';
	static const char CHEST		 = 'C';
	static const char ITEM		 = 'I';
	static const char DECORATION = 'D';

	TreeNode* mapTree;
};