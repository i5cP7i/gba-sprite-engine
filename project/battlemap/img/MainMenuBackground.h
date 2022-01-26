
//{{BLOCK(MainMenuBackground)

//======================================================================
//
//	MainMenuBackground, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 530 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 16960 + 2048 = 19040
//
//	Time-stamp: 2022-01-25, 22:14:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAINMENUBACKGROUND_H
#define GRIT_MAINMENUBACKGROUND_H

#define MainMenuBackgroundTilesLen 16960
extern const unsigned int MainMenuBackgroundTiles[4240];

#define MainMenuBackgroundMapLen 2048
extern const unsigned short MainMenuBackgroundMap[1024];

#define MainMenuBackgroundPalLen 32
extern const unsigned short MainMenuBackgroundPal[16];

#endif // GRIT_MAINMENUBACKGROUND_H

//}}BLOCK(MainMenuBackground)
