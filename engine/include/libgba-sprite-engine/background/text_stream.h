//
// Created by Wouter Groeneveld on 28/07/18.
//

#ifndef GBA_SPRITE_ENGINE_TEXT_STREAM_H
#define GBA_SPRITE_ENGINE_TEXT_STREAM_H

#include "background.h"
#include "text.h"

#include <string>
#include <memory>
#include <libgba-sprite-engine/palette/palette_manager.h>

#define CHAR_OFFSET_INDEX 32
#define TILE_WIDTH 32
#define PALETTE_COLOR_INDEX 15
#define PALETTE_TEXT_BANK 0

#define failure_gba(__mess) (consoleLog_func(__FILE__, __LINE__, __PRETTY_FUNCTION__, #__mess))
void log_text(const char* text);
void consoleLog_func(const char* fileName, const int lineNr, const char* fnName, const char* msg);

class TextStream : public Background {
private:
    int currRow, currCol;


    static TextStream* inst;
    TextStream();
    TextStream(TextStream& other) = delete;
    TextStream(TextStream&& other) = delete;

    u32 FontColorR = 31;
    u32 FontColorG = 31;
    u32 FontColorB = 31;

public:
    std::unique_ptr<BackgroundPaletteManager> palette;
    void clear();
    void setText(std::string text, int row, int col);
    void setText(const char* text, int row, int col);

    void setFontColor(COLOR color);
    void setFontStyle(const void *data, int size);

    static TextStream& instance();

    void persist() override;

    void setRGB(u32 r, u32 g, u32 b);

    TextStream& operator << (const char* s);
    TextStream& operator << (const int s);
    TextStream& operator << (const u32 s);
    TextStream& operator << (const bool s);
};


#endif //GBA_SPRITE_ENGINE_TEXT_STREAM_H
