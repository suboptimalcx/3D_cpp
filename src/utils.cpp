#include "utils.h"

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;
int nScore = 0;

wstring map = 
    L"################"
    L"#......#.......#"
    L"#......#.......#"
    L"#......#.......#"
    L"#......#....####"
    L"#..............#"
    L"#..............#"
    L"#........#######"
    L"#..#...........#"
    L"#..#...........#"
    L"#..............#"
    L"#..............#"
    L"########.......#"
    L"#..............#"
    L"#..............#"
    L"################";

void drawCol(const int &y, const int& x, const float& fDistanceToWall, wchar_t* screen, const bool& bBoundary) {
    // illusion of depth, calculate distance to ceiling and floor
    // ceiling = midpoint - proprtion of the screenheight proportionate to the distance to wall
    // so the further away we are, we see more ceiling. ( ilussion of depth )
    int nCeiling = (float)(SCREEN_HEIGHT / 2.0) - SCREEN_HEIGHT / ((float)fDistanceToWall);
    int nFloor = SCREEN_HEIGHT - nCeiling;
    int nWallWidth = 1;
    short nShade = ' ';

    if (y <= nCeiling) { // ceiling
        screen[y * SCREEN_WIDTH + x] = ' ';
    }
    else if (y > nCeiling && y <= nFloor) { // wall
        if (fDistanceToWall <= DEPTH / 4.0f)			nShade = 0x2588;	// close
        else if (fDistanceToWall < DEPTH / 3.0f)		nShade = 0x2593;
        else if (fDistanceToWall < DEPTH / 2.0f)		nShade = 0x2592;
        else if (fDistanceToWall < DEPTH)				nShade = 0x2591;
        else											nShade = ' ';		// far
        if (bBoundary) nShade = ' ';
        screen[y * SCREEN_WIDTH + x] = nShade;
    }
    else { // floor
        float fFloorDist = 1.0f - (((float)y - SCREEN_HEIGHT / 2.0f) / ((float)SCREEN_HEIGHT / 2.0f));
        if (fFloorDist < 0.25)			nShade = '#';
        else if (fFloorDist < 0.5)		nShade = 'x';
        else if (fFloorDist < 0.75)		nShade = '-';
        else if (fFloorDist < 0.9)		nShade = '.';
        else							nShade = ' ';
        screen[y * SCREEN_WIDTH + x] = nShade;
    }

    // if (bHitCollectible && y > nCeiling+fDistanceToCollectible && y <= nFloor-fDistanceToCollectible) {
    //     screen[y * SCREEN_WIDTH + x] = '@'; 
    // }
}

void detectBoundry(bool& bBoundary, const int& nTestX, const int& nTestY, const float& fEyeX, const float& fEyeY){
    /*
    cast a ray from each perfect corner of the cell back to the player
    look at the angle between the ray thats been cast out, and that perfect ray back
    we want the two closest (i.e the 2 smallest angles) to represent the boundary
    */
    vector<pair<float, float>> p;

    for (int cx = 0; cx < 2; cx++) { // increment because the corner is basically the cell to the left and right when you look at the map
        for (int cy = 0; cy < 2; cy++) {
            // vector from the player to the corner that was 'hit' with the ray*
            float vx = (float)nTestX + cx - fPlayerX;
            float vy = (float)nTestY + cy - fPlayerY;
            float distance = sqrt(vx * vx + vy * vy); // distance to the perfect corner
            float dot = (fEyeX * vx / distance) + (fEyeY * vy / distance); // the angle between the two
            p.push_back(make_pair(distance, dot));
        }
    }
    // sort pairs from closest to farthest
    sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

    float fBound = 0.01;
    if (acos(p.at(0).second) < fBound) bBoundary = true;
    if (acos(p.at(1).second) < fBound) bBoundary = true;
}

void playerMovement(const float& fElapsedTime, bool& bExit){
    if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
        fPlayerA -= (1.3f) * fElapsedTime;
    }
    if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
        fPlayerA += (1.3f) * fElapsedTime;
    }
    if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
        fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
        fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

        if (map[(int)fPlayerY * MAP_WIDTH + (int)fPlayerX] == '#') {
            fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
        }

    }
    if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
        fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
        fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

        if (map[(int)fPlayerY * MAP_WIDTH + (int)fPlayerX] == '#') {
            fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
        }
    }
    if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
        bExit = true;
    }
}