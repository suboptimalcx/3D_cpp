#include "3D_cpp.h"

using namespace std;

int nScreenWidth = 120; //COLS
int nScreenHeight = 40; //ROWS 

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f; //angle - the center of where the palyer is looking

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14 / 4.0;
float fDepth = 16;


int main()
{
	//create screen buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight]; //char array 
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); //screen buffer to be written into
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;
	map += L"################";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#......#....####";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#........#######";
	map += L"#..#...........#";
	map += L"#..#...........#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"########.......#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"###########....#";


	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (true) {

		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		//CONTROLS
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
			fPlayerA -= (1.3f) * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			fPlayerA += (1.3f) * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') { //collision detection
				fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			}

		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#') { //collision detection
				fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}

		for (int x = 0; x < nScreenWidth; x++) {
			//starts from the leftmost columns, and calculates its angle
			//fov is being split in the middle by playerAngle so the leftmost is fPlayerA0-fFOV/2 and the rightmost fPlayerA0+fFOV/2
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fDistanceToWall = 0;
			bool bHitWall = false;
			bool bBoundary = false;

			//unit vectors, when we have the direction the player is looking, we convert it into a cartesian coordinate to determine where the ray goes globally
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth) {

				fDistanceToWall += 0.1f;

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				//OP OOB RAY CHECK
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else {
					if (map[nTestY * nMapWidth + nTestX] == '#') {
						bHitWall = true;

						/*
						cast a ray from each perfect corner of the cell back to the player
						look at the angle between the ray thats been cast out, and that perfect ray back
						we want the two closest (i.e the 2 smallest angles) to represent the boundary
						*/
						vector<pair<float, float>> p;

						for (int cx = 0; cx < 2; cx++) {
							for (int cy = 0; cy < 2; cy++) {
								//vector from the player to the corner that was 'hit' with the ray
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
				}
			}
			//illusion of depth, calculate distance to ceiling and floor
			//ceiling = midpoint - proprtion of the screenheight proportionate to the distance to wall
			//so the further away we are, we see more ceiling. ( ilussion of depth )
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShade = ' ';
			if (fDistanceToWall <= fDepth / 4.0f)			nShade = 0x2588;	//close
			else if (fDistanceToWall < fDepth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToWall < fDepth)				nShade = 0x2591;
			else											nShade = ' ';		//far

			if (bBoundary) nShade = ' ';

			for (int y = 0; y < nScreenHeight; y++) {
				if (y <= nCeiling) { //ceiling
					screen[y * nScreenWidth + x] = ' ';
				}
				else if (y > nCeiling && y <= nFloor) { //wall
					screen[y * nScreenWidth + x] = nShade;
				}
				else { //floor
					float fFloorDist = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (fFloorDist < 0.25)			nShade = '#';
					else if (fFloorDist < 0.5)		nShade = 'x';
					else if (fFloorDist < 0.75)		nShade = '.';
					else if (fFloorDist < 0.9)		nShade = '-';
					else							nShade = ' ';
					screen[y * nScreenWidth + x] = nShade;
				}
			}
		}

		//stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);

		//map
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
			}
		screen[((int)fPlayerY + 1) * nScreenWidth + (int)fPlayerX] = 'P';

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten); //writes the screen into the console buffer (0,0 starting pos)
	}

	return 0;
}