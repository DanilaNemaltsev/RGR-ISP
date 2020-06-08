//Перед началом работы смените адрес текстового документа (строка 42)

#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

struct LabrItem
{
	char Symb;
	int NumStep;
};

LabrItem Labr[256][256];

void SetAttr(WORD attr) //чтобы можно было менять атрибуты
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, attr);
}

void GotoXY(short X, short Y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cor = {0, 0};
	cor.X = X;
	cor.Y = Y;
	SetConsoleCursorPosition(hConsole, cor);
}

void PrintXY(char T, short X, short Y, WORD attr)
{
	GotoXY(X, Y);
	SetAttr(attr);
	cout << T;
}
int main()
{
	setlocale(LC_ALL, "RUS");

	ifstream file; // создаем объект класса ifstream
    file.open("Лабиринт.txt"); // открываем файл



	int row = 0, col;

	while (!file.eof())
	{
		char str[256];
		file.getline(&str[0], 256, '\n');

		col = 0;
		while (str[col] != 0)
		{
			Labr[row][col].Symb = str[col];
			Labr[row][col].NumStep = INT_MAX;
			++col;
		}
		++row;
	}


	file.close();

	// Вывод лабиринта на экран, поиск начального положения игрока и выхода
	int RowBegin, ColBegin;
	int RowEnd, ColEnd;
	for (row = 0; row < 256; row++)
	{
		if (Labr[row][0].Symb == 0) break;
		for (col = 0; col < 256; col++)
		{
			if (Labr[row][col].Symb != 0)
			{
				std::cout << Labr[row][col].Symb;

				if (Labr[row][col].Symb == '2')
				{
					RowBegin = row;
					ColBegin = col;
				}
				else if (Labr[row][col].Symb == '3')
				{
					RowEnd = row;
					ColEnd = col;
					Labr[row][col].NumStep = 0;
				}
			}
		}
		std::cout << '\n';
	}

	//АЛГОРИТМ ЛИ
	int d = 0;
	bool FindBegin = false;
	bool WasWave = true;
	while (!FindBegin && WasWave)
	{
		WasWave = false;
		for (row = 0; row < 256; row++)
		{
			for (col = 0; col < 256; col++)
			{
				if (Labr[row][col].Symb != 0 && Labr[row][col].NumStep == d)
				{
					if (Labr[row][col - 1].NumStep == INT_MAX && col > 0 && (Labr[row][col - 1].Symb == '1' || Labr[row][col - 1].Symb == '2'))
					{
						Labr[row][col - 1].NumStep = d + 1;
						WasWave = true;
						if (Labr[row][col - 1].Symb == '2') FindBegin = true;
					}
					if (Labr[row - 1][col].NumStep == INT_MAX && row > 0 && (Labr[row - 1][col].Symb == '1' || Labr[row - 1][col].Symb == '2'))
					{
						Labr[row - 1][col].NumStep = d + 1;
						WasWave = true;
						if (Labr[row - 1][col].Symb == '2') FindBegin = true;
					}
					if (Labr[row][col + 1].NumStep == INT_MAX && col < 255 && (Labr[row][col + 1].Symb == '1' || Labr[row][col + 1].Symb == '2'))
					{
						Labr[row][col + 1].NumStep = d + 1;
						WasWave = true;
						if (Labr[row][col + 1].Symb == '2') FindBegin = true;
					}
					if (Labr[row + 1][col].NumStep == INT_MAX && row < 255 && (Labr[row + 1][col].Symb == '1' || Labr[row + 1][col].Symb == '2'))
					{
						Labr[row + 1][col].NumStep = d + 1;
						WasWave = true;
						if (Labr[row + 1][col].Symb == '2') FindBegin = true;
					}
				}
			}
		}
		++d;
	}

	if (FindBegin)
	{
		col = ColBegin;
		row = RowBegin;
		GotoXY(ColBegin, RowBegin);
		PrintXY('2', col, row, (WORD)((0 << 4) | 14));
		do
		{
			Sleep(50);
			if (col > 0 && Labr[row][col - 1].NumStep == Labr[row][col].NumStep - 1) --col;
			else if (row > 0 && Labr[row - 1][col].NumStep == Labr[row][col].NumStep - 1) --row;
			else if (col < 255 && Labr[row][col + 1].NumStep == Labr[row][col].NumStep - 1) ++col;
			else if (row < 255 && Labr[row + 1][col].NumStep == Labr[row][col].NumStep - 1) ++row;
			if(Labr[row][col].Symb != '3') PrintXY('¤'/*'1'*/, col, row, (WORD)((0 << 4) | 14));
			else PrintXY('3', col, row, (WORD)((0 << 4) | 14));
		}
		while (col != ColEnd || row != RowEnd);
	}
	else std::cout << "\n\nПрохода к выходу нет!\n";

	GotoXY(0, 20);
	SetAttr((WORD)((0 << 4) | 7));
}

