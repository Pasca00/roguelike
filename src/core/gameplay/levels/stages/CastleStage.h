#pragma once 

#include <unordered_map>

#include "IStage.h"
#include "../../../Paths.h"

class CastleStage : public IStage {
private:
	std::string texturesDir;

	char** findTileType(char** stageTemplate);
	void loadTextures() override;
	void createTileMap(char** stageTemplate, char** stageTiles) override;
	void placeItems() override;

	TreeNode* getRoomForCoords(float x, float y);

public:
	CastleStage(
		std::shared_ptr<IGenerator>& generator,
		std::shared_ptr<TextureManager>& textureManager,
		int tileSize = 32
	);
	~CastleStage();

	void generateStage() override;

	void update(float dtime);

private:
	static const int corridor_T_down		 = 18;
	static const int corridor_T_up			 = 17;
	static const int corridor_T_left		 = 16;
	static const int corridor_T_right		 = 15;
	static const int corridor_vertical		 = 14;
	static const int corridor_horizontal	 = 13;
	static const int corridor_deadend_bottom = 12;
	static const int corridor_deadend_right  = 11;
	static const int corridor_deadend_top	 = 10;
	static const int corridor_deadend_left	 = 9;
	static const int no_wall				 = 8;
	static const int right_wall				 = 7;
	static const int left_wall				 = 6;
	static const int bottom_wall			 = 5;
	static const int top_wall				 = 4;
	static const int corner_bottom_right	 = 3;
	static const int corner_bottom_left		 = 2;
	static const int corner_top_right		 = 1;
	static const int corner_top_left		 = 0;


	static const int wall_left				 = 0;
	static const int wall_right				 = 1;
	static const int wall_base				 = 2;
	static const int wall_corridor_deadend	 = 3;
	static const int wall_stairway			 = 4;

	static const int ceil_corridor_vertical			= 14;
	static const int ceil_corridor_horizontal		= 13;
	static const int ceil_corridor_deadend_bottom	= 12;
	static const int ceil_corridor_deadend_right	= 11;
	static const int ceil_corridor_deadend_top		= 10;
	static const int ceil_corridor_deadend_left		= 9;
	static const int ceil_right						= 8;
	static const int ceil_bottom					= 7;
	static const int ceil_top						= 6;
	static const int ceil_left						= 5;
	static const int ceil_corner_bottom_left		= 4;
	static const int ceil_corner_bottom_right		= 3;
	static const int ceil_corner_top_right			= 2;
	static const int ceil_corner_top_left			= 1;
	static const int ceil_base						= 0;
};