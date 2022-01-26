
//{{BLOCK(text)

//======================================================================
//
//	text, 256x24@4, 
//	+ palette 256 entries, not compressed
//	+ 79 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x3 
//	Total size: 512 + 2528 + 192 = 3232
//
//	Time-stamp: 2021-12-28, 04:37:22
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEXT_H
#define GRIT_TEXT_H

#define textTilesLen 2528
extern const unsigned short textTiles[1264];

#define textMapLen 192
extern const unsigned short textMap[96];

#define textPalLen 512
extern const unsigned short textPal[256];

#endif // GRIT_TEXT_H

//}}BLOCK(text)
