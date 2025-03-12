#include "functions.h"

void drawCol(int &y, int& x, float& fDistanceToWall, wchar_t* screen, int& nCeiling, int& nFloor, short& nShade) {
    if (y <= nCeiling) { //ceiling
        screen[y * SCREEN_WIDTH + x] = ' ';
    }
    else if (y > nCeiling && y <= nFloor) { //wall
        screen[y * SCREEN_WIDTH + x] = nShade;
    }
    else { //floor
        float fFloorDist = 1.0f - (((float)y - SCREEN_HEIGHT / 2.0f) / ((float)SCREEN_HEIGHT / 2.0f));
        if (fFloorDist < 0.25)			nShade = '#';
        else if (fFloorDist < 0.5)		nShade = 'x';
        else if (fFloorDist < 0.75)		nShade = '.';
        else if (fFloorDist < 0.9)		nShade = '-';
        else							nShade = ' ';
        screen[y * SCREEN_WIDTH + x] = nShade;
    }

    // if (bHitCollectible && y > nCeiling+fDistanceToCollectible && y <= nFloor-fDistanceToCollectible) {
    //     screen[y * SCREEN_WIDTH + x] = '@'; 
    // }
}

void detectBoundry(bool& bHitWall, bool& bBoundary, int& nTestX, int& nTestY, float& fEyeX, float& fEyeY){
    /*
    cast a ray from each perfect corner of the cell back to the player
    look at the angle between the ray thats been cast out, and that perfect ray back
    we want the two closest (i.e the 2 smallest angles) to represent the boundary
    */
    vector<pair<float, float>> p;

    for (int cx = 0; cx < 2; cx++) { //increment because the corner is basically the cell to the left and right when you look at the map
        for (int cy = 0; cy < 2; cy++) {
            //vector from the player to the corner that was 'hit' with the ray*
            float vx = (float)nTestX + cx - fPlayerX;
            float vy = (float)nTestY + cy - fPlayerY;
            float distance = sqrt(vx * vx + vy * vy); //distance to the perfect corner
            float dot = (fEyeX * vx / distance) + (fEyeY * vy / distance); //the angle between the two
            p.push_back(make_pair(distance, dot));
        }
    }
    //sort pairs from closest to farthest
    sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });

    //
    float fBound = 0.01;
    if (acos(p.at(0).second) < fBound) bBoundary = true;
    if (acos(p.at(1).second) < fBound) bBoundary = true;
}