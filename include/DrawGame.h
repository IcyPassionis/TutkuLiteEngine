#pragma once

#ifndef DRAWGAME_H
#define DRAWGAME_H
void DrawGame(); // Draws game every frame
void InitDraw(); // Init windows and other render settings for the first time
void StartScene(); // First time initialize the scene works before drawing the scene, this could be used for initializing lights or objects into memory.
void DrawScene(); // Works in the DrawGame function, and if shadows active it works on shadow-maps too, this where the scene should be created
void DrawDefaultScene(); // Draws default scene if no available scenes provided !
#endif