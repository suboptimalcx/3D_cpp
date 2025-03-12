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

extern float fPlayerX = 8.0f;
extern float fPlayerY = 8.0f;
extern float fPlayerA = 0.0f;
extern int nScore = 0;

void drawCol(int &y, int& x, float& fDistanceToWall, wchar_t* screen, int& nCeiling, int& nFloor, short& nShade);
void detectBoundry(bool& bHitWall, bool& bBoundary, int& nTestX, int& nTestY, float& fEyeX, float& fEyeY);