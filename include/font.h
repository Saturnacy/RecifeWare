#ifndef FONT_H
#define FONT_H

#include "raylib.h"
#include <stdbool.h>

void LoadGameFont(void);
void UnloadGameFont(void);
Font GetGameFont(void);

void LoadIntroFont(void);
void UnloadIntroFont(void);
Font GetIntroFont(void);

#endif