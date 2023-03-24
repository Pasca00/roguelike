#pragma once

class IGenerator {
protected:
	char** layout;
	int h;
	int w;

public:
	IGenerator(int h = 50, int w = 50) : h(h), w(w) {
		layout = new char* [h];
		for (int i = 0; i < h; i++) {
			layout[i] = new char[w];
			for (int j = 0; j < w; j++) {
				layout[i][j] = '#';
			}
		}
	}

	virtual ~IGenerator() {
		for (int i = 0; i < h; i++) {
			delete[] layout[i];
		}
		delete[] layout;
	}

	virtual char** generateLayout() = 0;
};