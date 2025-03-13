#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <cmath>
#include <Windows.h>

using namespace std;

constexpr int SCREEN_WIDTH = 120;
constexpr int SCREEN_HEIGHT = 40;
constexpr int MAP_HEIGHT = 16;
constexpr int MAP_WIDTH = 16;
constexpr float FOV = 3.14f / 4.0f;
constexpr int DEPTH = 16;

extern wstring map;
extern float fPlayerX;
extern float fPlayerY;
extern float fPlayerA;
extern int nScore;

extern void drawCol(const int &y, const int& x, const float& fDistanceToWall, wchar_t* screen, const bool& bBoundary);
extern void detectBoundry(bool& bBoundary, const int& nTestX, const int& nTestY, const float& fEyeX, const float& fEyeY);
extern void playerMovement(const float& fElapsedTime, bool& bExit);
