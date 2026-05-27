#include "font.h"
#include <stdlib.h>
#include <string.h>

static Font gameFont   = { 0 };
static bool fontLoaded = false;

static const int FONT_CODEPOINTS[] = {
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    0x00C2,0x00C1,0x00C0,0x00C4,0x00C3,0x00C7,0x00CA,0x00C9,0x00C8,0x00CB,0x00CE,0x00CD,0x00CC,
    0x00CF,0x00D1,0x00D4,0x00D3,0x00D2,0x00D6,0x0152,0x00DB,0x00DA,0x00D9,0x00DC,
    0x00E2,0x00E1,0x00E0,0x00E4,0x00E3,0x00E7,0x00EA,0x00E9,0x00E8,0x00EB,0x00EE,0x00ED,0x00EC,
    0x00EF,0x00F1,0x00F4,0x00F3,0x00F2,0x00F6,0x0153,0x00FB,0x00FA,0x00F9,0x00FC,
    0x00DF,
    '0','1','2','3','4','5','6','7','8','9',
    0x00B4,0x00B0,',','.',0x2022,':',';','?','!','-','_','~',
    0x2018,0x2019,0x201C,0x201D,'(',')','[',']','+',0x00D7,'=',0x00F7,'%','&',
};

static const Rectangle fontRects[] = {
    { 0, 0, 9, 15 },{ 11, 0, 9, 15 },{ 22, 0, 10, 15 },{ 34, 0, 10, 15 },
    { 46, 0, 9, 15 },{ 57, 0, 9, 15 },{ 68, 0, 10, 15 },{ 80, 0, 10, 15 },
    { 92, 0, 6, 15 },{ 100, 0, 9, 15 },{ 111, 0, 10, 15 },{ 123, 0, 9, 15 },
    { 134, 0, 11, 15 },{ 147, 0, 10, 15 },{ 159, 0, 11, 15 },{ 172, 0, 9, 15 },
    { 183, 0, 12, 15 },{ 197, 0, 10, 15 },{ 209, 0, 9, 15 },{ 220, 0, 10, 15 },
    { 232, 0, 10, 15 },{ 244, 0, 11, 15 },{ 257, 0, 11, 15 },{ 270, 0, 11, 15 },
    { 283, 0, 10, 15 },{ 295, 0, 10, 15 },
    { 307, 0, 8, 15 },{ 317, 0, 8, 15 },{ 327, 0, 8, 15 },{ 337, 0, 8, 15 },
    { 347, 0, 8, 15 },{ 357, 0, 7, 15 },{ 366, 0, 8, 15 },{ 376, 0, 8, 15 },
    { 386, 0, 4, 15 },{ 392, 0, 6, 15 },{ 400, 0, 8, 15 },{ 410, 0, 5, 15 },
    { 417, 0, 8, 15 },{ 427, 0, 8, 15 },{ 437, 0, 8, 15 },{ 447, 0, 8, 15 },
    { 457, 0, 8, 15 },{ 467, 0, 7, 15 },{ 476, 0, 8, 15 },{ 486, 0, 7, 15 },
    { 495, 0, 8, 15 },{ 505, 0, 8, 15 },{ 515, 0, 8, 15 },{ 525, 0, 8, 15 },
    { 535, 0, 8, 15 },{ 545, 0, 8, 15 },
    { 555, 0, 10, 15 },{ 567, 0, 10, 15 },{ 579, 0, 10, 15 },{ 591, 0, 10, 15 },
    { 603, 0, 10, 15 },{ 615, 0, 10, 15 },{ 627, 0, 9, 15 },{ 638, 0, 9, 15 },
    { 649, 0, 9, 15 },{ 660, 0, 9, 15 },{ 671, 0, 8, 15 },{ 681, 0, 6, 15 },
    { 689, 0, 6, 15 },{ 697, 0, 8, 15 },{ 707, 0, 10, 15 },{ 719, 0, 11, 15 },
    { 732, 0, 11, 15 },{ 745, 0, 11, 15 },{ 758, 0, 11, 15 },{ 771, 0, 12, 15 },
    { 785, 0, 10, 15 },{ 797, 0, 10, 15 },{ 809, 0, 10, 15 },{ 821, 0, 10, 15 },
    { 833, 0, 8, 15 },{ 843, 0, 8, 15 },{ 853, 0, 8, 15 },{ 863, 0, 8, 15 },
    { 873, 0, 8, 15 },{ 883, 0, 8, 15 },{ 893, 0, 8, 15 },{ 903, 0, 8, 15 },
    { 913, 0, 8, 15 },{ 923, 0, 8, 15 },{ 933, 0, 6, 15 },{ 941, 0, 5, 15 },
    { 948, 0, 5, 15 },{ 955, 0, 6, 15 },{ 963, 0, 8, 15 },{ 973, 0, 8, 15 },
    { 983, 0, 8, 15 },{ 993, 0, 8, 15 },{ 1003, 0, 8, 15 },{ 1013, 0, 9, 15 },
    { 1024, 0, 8, 15 },{ 1034, 0, 8, 15 },{ 1044, 0, 8, 15 },{ 1054, 0, 8, 15 },
    { 1064, 0, 9, 15 },
    { 1075, 0, 10, 15 },{ 1087, 0, 6, 15 },{ 1095, 0, 10, 15 },{ 1107, 0, 10, 15 },
    { 1119, 0, 10, 15 },{ 1131, 0, 10, 15 },{ 1143, 0, 10, 15 },{ 1155, 0, 10, 15 },
    { 1167, 0, 10, 15 },{ 1179, 0, 10, 15 },
    { 1191, 0, 6, 15 },{ 1199, 0, 5, 15 },{ 1206, 0, 4, 15 },{ 1212, 0, 4, 15 },
    { 1218, 0, 4, 15 },{ 1224, 0, 4, 15 },{ 1230, 0, 4, 15 },{ 1236, 0, 10, 15 },
    { 1248, 0, 4, 15 },{ 1254, 0, 11, 15 },{ 1267, 0, 11, 15 },{ 1280, 0, 12, 15 },
    { 1294, 0, 4, 15 },{ 1300, 0, 4, 15 },{ 1306, 0, 7, 15 },{ 1315, 0, 7, 15 },
    { 1324, 0, 6, 15 },{ 1332, 0, 6, 15 },{ 1340, 0, 6, 15 },{ 1348, 0, 6, 15 },
    { 1356, 0, 10, 15 },{ 1368, 0, 11, 15 },{ 1381, 0, 10, 15 },{ 1393, 0, 10, 15 },
    { 1405, 0, 11, 15 },{ 1418, 0, 12, 15 },
};

#define GLYPH_COUNT (sizeof(FONT_CODEPOINTS) / sizeof(FONT_CODEPOINTS[0]))

void LoadGameFont(void) {
    if (fontLoaded) return;

    Image img = LoadImage("../assets/fonts/menu_font.png");
    if (img.data == NULL) {
        TraceLog(LOG_WARNING, "FONT: Nao foi possivel carregar menu_font.png");
        return;
    }

    Image atlas = ImageCopy(img);
    UnloadImage(img);

    gameFont.baseSize     = 15;
    gameFont.glyphCount   = GLYPH_COUNT;
    gameFont.glyphPadding = 0;
    gameFont.texture      = LoadTextureFromImage(atlas);

    gameFont.recs   = (Rectangle *)RL_MALLOC(GLYPH_COUNT * sizeof(Rectangle));
    gameFont.glyphs = (GlyphInfo *)RL_MALLOC(GLYPH_COUNT * sizeof(GlyphInfo));

    for (int i = 0; i < (int)GLYPH_COUNT; i++) {
        gameFont.recs[i] = fontRects[i];

        gameFont.glyphs[i].value    = FONT_CODEPOINTS[i];
        gameFont.glyphs[i].offsetX  = 0;
        gameFont.glyphs[i].offsetY  = 0;
        gameFont.glyphs[i].advanceX = (int)fontRects[i].width;
        gameFont.glyphs[i].image    = GenImageColor(1, 1, BLANK);
    }

    UnloadImage(atlas);
    fontLoaded = true;

    TraceLog(LOG_INFO, "FONT: menu_font.png carregada (%d glifos)", (int)GLYPH_COUNT);
}

void UnloadGameFont(void) {
    if (!fontLoaded) return;
    for (int i = 0; i < gameFont.glyphCount; i++)
        UnloadImage(gameFont.glyphs[i].image);
    RL_FREE(gameFont.glyphs);
    RL_FREE(gameFont.recs);
    UnloadTexture(gameFont.texture);
    fontLoaded = false;
}

Font GetGameFont(void) {
    return gameFont;
}