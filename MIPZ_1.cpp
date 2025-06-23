#include "MIPZ_1.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

const int BOARD_SIZE = 19;
const int WIN_STONES_ROW_SIZE = 5;

bool InTheBoardBounds(int i)
{
	return ((BOARD_SIZE > i) && (i >= 0)) ? true : false;
}

// Returns array with: winner, row number, column number 
std::array<int, 3> RenjuCheckResult(std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>& Board)
{
	// Arrays to count stones in columns and diagonals
	// If count > 0 streak belongs to player 1, if count < 0 — player 2
	std::array<int, BOARD_SIZE> ColumnsCount = {};
	// Formula for diagonal lines count: 2n
	std::array<int, 2*BOARD_SIZE> DiagonalsCount = {};
	std::array<int, 2*BOARD_SIZE> ReverseDiagonalsCount = {};

	int CurDiag;
	int CurInvDiag;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		CurDiag = i;
		CurInvDiag = BOARD_SIZE - 1 - i;

		volatile int RowCount = 0;
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			CurDiag++;
			CurInvDiag++;

			if (Board[i][j] == 0)
			{
				RowCount = 0;
				ColumnsCount[j] = 0;
				DiagonalsCount[CurDiag] = 0;
				ReverseDiagonalsCount[CurInvDiag] = 0;
			}
			else
			{
				int Coef = (Board[i][j] == 1) ? 1 : -1;

				if (Coef * RowCount >= 0)
				{
					if (Coef * RowCount == WIN_STONES_ROW_SIZE - 1 &&
						// Check if more than WIN_STONES_ROW_SIZE stones placed
						(!InTheBoardBounds(j+1) || Board[i][j] != Board[i][j+1]))
					{ 
						return { Board[i][j], i, j - WIN_STONES_ROW_SIZE + 1 };
					}
					RowCount += Coef;
				}
				else 
				{ 
					RowCount = Coef; 
				}

				if (Coef * ColumnsCount[j] >= 0)
				{
					if (Coef * ColumnsCount[j] == WIN_STONES_ROW_SIZE &&
						// Check if more than WIN_STONES_ROW_SIZE stones placed
						(!InTheBoardBounds(i + 1) || Board[i][j] != Board[i + 1][j]))
					{ 
						return { Board[i][j], i - WIN_STONES_ROW_SIZE + 1, j };
					}
					ColumnsCount[j] += Coef;
				}
				else 
				{ 
					ColumnsCount[j] = Coef; 
				}

				if (Coef * DiagonalsCount[CurDiag] >= 0)
				{
					if (Coef * DiagonalsCount[CurDiag] == 4 &&
						// Check if more than WIN_STONES_ROW_SIZE stones placed
						(!InTheBoardBounds(i + 1) || (!InTheBoardBounds(j - 1) || Board[i][j] != Board[i][j - 1])))
					{ 
						return { Board[i][j], i - WIN_STONES_ROW_SIZE + 1, j + WIN_STONES_ROW_SIZE - 1 };
					}
					DiagonalsCount[CurDiag] += Coef;
				}
				else 
				{ 
					DiagonalsCount[CurDiag] = Coef; 
				}

				if (Coef * ReverseDiagonalsCount[CurInvDiag] >= 0)
				{
					if (Coef * ReverseDiagonalsCount[CurInvDiag] == WIN_STONES_ROW_SIZE - 1 &&
						// Check if more than WIN_STONES_ROW_SIZE stones placed
						(!InTheBoardBounds(i + 1) || (!InTheBoardBounds(j + 1) || Board[i][j] != Board[i][j + 1])))
					{ 
						return { Board[i][j], i - WIN_STONES_ROW_SIZE + 1, j - WIN_STONES_ROW_SIZE + 1 };
					}
					ReverseDiagonalsCount[CurInvDiag] += Coef;
				}
				else 
				{ 
					ReverseDiagonalsCount[CurInvDiag] = Coef; 
				};
			}
		}
	}
	return { 0 };
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Expected filename as an argument";
		getchar();
		return -1;
	}
	std::string InputFile(argv[1]);

	std::ifstream Input(InputFile);
	if (Input.fail()) {
		std::cout << "Invalid file";
		getchar();
		return -1;
	}

	std::string InpuRow;
	getline(Input, InpuRow);
	int x = std::stoi(InpuRow);

	for (size_t CaseNum = 0; CaseNum < x; CaseNum++)
	{
		std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> Board = { {} };
		for (size_t i = 0; i < BOARD_SIZE; i++)
		{
			if (getline(Input, InpuRow))
			{
				std::stringstream ParsedRow(InpuRow);
				for (size_t j = 0; j < BOARD_SIZE; j++)
				{
					ParsedRow >> Board[i][j];
				}
			}
			else
			{
				Input.close();
				std::cout << "WARNING: " << x << " test cases were expected, only " << i << "were given";
				return -1;
			}
		}
		std::array<int, 3> CheckResult = RenjuCheckResult(Board);
		std::cout << CheckResult[0] << "\n";
		if (CheckResult[0])
		{
			// +1 so indices count starts from 1
			std::cout << CheckResult[1]+1 << " " << CheckResult[2]+1 << "\n";
		}
	}
	
	Input.close();

	getchar();
	return 0;
}
