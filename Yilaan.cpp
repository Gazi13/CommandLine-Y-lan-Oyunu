/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

BU KOD ALINTIDIR SADECE T�RK�E YORUM EKLENM��T�R.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
OneLoneCoder.com - Command Line Snake
"Give me a break, I'm on holiday..." - @Javidx9

License
~~~~~~~
Copyright (C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.
Original works located at:
https://www.github.com/onelonecoder
https://www.onelonecoder.com
https://www.youtube.com/javidx9

GNU GPLv3
https://github.com/OneLoneCoder/videos/blob/master/LICENSE

From Javidx9 :)
~~~~~~~~~~~~~~~
Hello! Ultimately I don't care what you use this for. It's intended to be
educational, and perhaps to the oddly minded - a little bit of fun.
Please hack this, change it and use it in any way you see fit. You acknowledge
that I am not responsible for anything bad that happens as a result of
your actions. However this code is protected by GNU GPLv3, see the license in the
github repo. This means you must attribute me if you use it. You can view this
license here: https://github.com/OneLoneCoder/videos/blob/master/LICENSE
Cheers!


Background
~~~~~~~~~~
Classic Snake! Controls are Arrow keys Left & Right, eat food, grow larger, avoid self!


Author
~~~~~~
Twitter: @javidx9
Blog: www.onelonecoder.com

Video:
~~~~~~
https://youtu.be/e8lYLYlrGLg

Last Updated: 24/06/2017
*/

#include <iostream>
#include <list>
#include <thread>
using namespace std;

#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment
{
	int x;
	int y;
};

int main()
{
	// Create Screen Buffer - Ekrana yazma i�lemi i�in
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	while (1)
	{
		// Oyunu Ba�lat - S�f�rla

		list<sSnakeSegment> snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15} };//Y�lan�n ilk konumu
		int nFoodX = 30;//Yem konumu
		int nFoodY = 15;
		int nScore = 0;
		int nSnakeDirection = 3; // 0-1-2-3 olarak her y�n bir say� ile ifade ediliyor.
		bool bDead = false;
		bool bKeyLeft = false, bKeyRight = false, bKeyLeftOld = false, bKeyRightOld = false; // Hareket y�n�n�n de�i�ti�ini anlamak i�in eskisini tutuyoruz.

		while (!bDead)
		{
			// Frame Timing, compensate for aspect ratio of command line
			// Konsolda yatay ile dikeyde hareket h�z� fark�n� �nlemek i�in y�ne g�re ge�ikme s�resini de�i�tiriyoruz.
			auto t1 = chrono::system_clock::now();
			while ((chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 1) ? 120ms : 200ms))
			{
				// Get Input,
				//Sa�-Sol tu�lar�n� al�yoruz.
				bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
				bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

				// Y�n 0-1-2-3-0-1-2-3 �eklinde 0-3 aras� kals�n diye kontrol ediyoruz.
				if (bKeyRight && !bKeyRightOld)
				{
					nSnakeDirection++;
					if (nSnakeDirection == 4) nSnakeDirection = 0;
				}

				if (bKeyLeft && !bKeyLeftOld)
				{
					nSnakeDirection--;
					if (nSnakeDirection == -1) nSnakeDirection = 3;
				}

				bKeyRightOld = bKeyRight;
				bKeyLeftOld = bKeyLeft;
			}


			// Update Snake Position, place a new head at the front of the list in the right direction

			// Y�lan kafans�n� hareket ettiriyoruz gidece�i y�ne g�re.
			switch (nSnakeDirection)
			{
			case 0: // Yukar�
				snake.push_front({ snake.front().x, snake.front().y - 1 });
				break;
			case 1: // Sa�
				snake.push_front({ snake.front().x + 1, snake.front().y });
				break;
			case 2: // A�a��
				snake.push_front({ snake.front().x, snake.front().y + 1 });
				break;
			case 3: // Sol
				snake.push_front({ snake.front().x - 1, snake.front().y });
				break;
			}

			// �arp��malar      Y�lan V Yem
			if (snake.front().x == nFoodX && snake.front().y == nFoodY)
			{
				nScore++;
				// Bo� bir yere yeni yem koyuyoruz.
				while (screen[nFoodY * nScreenWidth + nFoodX] != L' ')
				{
					nFoodX = rand() % nScreenWidth;
					nFoodY = (rand() % (nScreenHeight - 3)) + 3;
				}

				for (int i = 0; i < 5; i++)
					snake.push_back({ snake.back().x, snake.back().y });
			}

		    // �arp��malar      Y�lan V Ortam

			if (snake.front().x < 0 || snake.front().x >= nScreenWidth)
				bDead = true;
			if (snake.front().y < 3 || snake.front().y >= nScreenHeight)
				bDead = true;

			// �arp��malar      Y�lan V Y�lan  (y�lan kendine �arparsa)
			for (list<sSnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
					bDead = true;

			// Y�lan�n kuyru�undan siliyoruz
			snake.pop_back();

	

			// Ekran� Temizle
			for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

			// �st men� �izimi
			for (int i = 0; i < nScreenWidth; i++)
			{
				screen[i] = L'=';
				screen[2 * nScreenWidth + i] = L'=';
			}
			wsprintf(&screen[nScreenWidth + 5], L"www.OneLoneCoder.com - S N A K E ! !   Bu Kod Al�nt�d�r.    SCORE: %d", nScore);


			// Y�lan� �iziyoruz
			for (auto s : snake)
				screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'O';

			// Y�lan�n ba��n� �iziyoruz
			screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'@';

			// Yem 
			screen[nFoodY * nScreenWidth + nFoodX] = L'%';

			// E�er �l�rse oyun duruyor bo�luk tu�unu bekliyoruz.
			if (bDead)
				wsprintf(&screen[15 * nScreenWidth + 40], L"    PRESS 'SPACE' TO PLAY AGAIN    ");

			// Ekrana bas�yoruz.
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}

		// Bo�luk tu�unu bekle 
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
	}

	return 0;
}


