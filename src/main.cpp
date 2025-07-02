#include "utils.h"

int main()
{
    // create screen buffer
    wchar_t* screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT]; // char array 
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); // screen buffer to be written into
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();
	bool bExit = false;

    while (!bExit) {
        tp2 = chrono::system_clock::now(); 
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();
        //CONTROLS
        playerMovement(fElapsedTime, bExit); 
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            // starts from the leftmost columns, and calculates its angle
            // fov is being split in the middle by playerAngle so the leftmost is fPlayerA0-fFOV/2 and the rightmost fPlayerA0+fFOV/2
            float fRayAngle = (fPlayerA - FOV / 2.0f) + ((float)x / (float)SCREEN_WIDTH) * FOV;
            float fDistanceToWall = 0;
            float fDistanceToCollectible = 0;
            bool bHitWall = false;
            bool bHitCollectible = false;
            bool bBoundary = false;
            // unit vectors, when we have the direction the player is looking, we convert it into a cartesian coordinate to determine where the ray goes globally
            float fEyeX = sinf(fRayAngle);
            //sowinski chuj
            float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < DEPTH) {
                fDistanceToWall += 0.1f;
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
                // OP OOB RAY CHECK
                if (nTestX < 0 || nTestX >= MAP_WIDTH || nTestY < 0 || nTestY >= MAP_HEIGHT) {
                    bHitWall = true;
                    fDistanceToWall = DEPTH;
                }
                else {
                    if (map[nTestY * MAP_WIDTH + nTestX] == '#') {
                        bHitWall = true;
                        detectBoundry(bBoundary, nTestX, nTestY, fEyeX, fEyeY);
                    }
                    if (map[nTestY * MAP_WIDTH + nTestX] == '*') {
                        bHitCollectible = true;
                        fDistanceToCollectible = fDistanceToWall; 
                    }
                }
            }
            for (int y = 0; y < SCREEN_HEIGHT; y++) {     
                drawCol(y, x, fDistanceToWall, screen, bBoundary);                     
            }
        }
        // if(map[(int) fPlayerY * MAP_WIDTH + fPlayerX] == '*') {
        //     map[(int) fPlayerY * MAP_WIDTH + fPlayerX] = '.';
        //     nScore++;
        // }

        // stats
        swprintf_s(screen, 50, L"X=%3.2f, Y=%3.2f, A=%3.2f, SCR=%d, FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, nScore, 1.0f / fElapsedTime);

        // map
        for (int nx = 0; nx < MAP_WIDTH; nx++)
            for (int ny = 0; ny < MAP_WIDTH; ny++)
            {
                screen[(ny + 1) * SCREEN_WIDTH + nx] = map[ny * MAP_WIDTH + nx];
            }
        screen[((int)fPlayerY + 1) * SCREEN_WIDTH + (int)fPlayerX] = 'P';

        screen[SCREEN_WIDTH * SCREEN_HEIGHT - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &dwBytesWritten); // writes the screen into the console buffer (0,0 starting pos)
    }

    return 0;
}