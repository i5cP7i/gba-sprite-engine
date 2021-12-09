
//{{BLOCK(Tile)

//======================================================================
//
//	Tile, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 10 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 640 + 2048 = 3200
//
//	Time-stamp: 2021-12-09, 22:20:35
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TILE_H
#define GRIT_TILE_H

#define TileTilesLen 640
extern const unsigned int TileTiles[160];

#define TileMapLen 2048
extern const unsigned short TileMap[1024];

#define TilePalLen 512
extern const unsigned short TilePal[256];

#endif // GRIT_TILE_H

//}}BLOCK(Tile)
