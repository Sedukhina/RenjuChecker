#include "MIPZ_1.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

// Returns array with: winner, row number, column number 
std::array<int, 3> RenjuCheckResult(std::array<std::array<int, 19>, 19>& Board)
{
	// Arrays to count stones in columns and diagonals
	// If count > 0 streak belongs to player 1, if count < 0 — player 2
	std::array<int, 19> ColumnsCount = {};
	// Formula for diagonal lines count: 2n-1
	std::array<int, 38> DiagonalsCount = {};
	std::array<int, 38> ReverseDiagonalsCount = {};

	int CurDiag;
	int CurInvDiag;

	for (int i = 0; i < 19; i++)
	{
		CurDiag = i;
		CurInvDiag = 18 - i;

		volatile int RowCount = 0;
		for (int j = 0; j < 19; j++)
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
					if (Coef * RowCount == 4 &&
						((j + 1 == 19) || Board[i][j] != Board[i][j+1])) { return { Board[i][j], i, j - 4 }; }
					RowCount += Coef;
				}
				else { RowCount = Coef; }

				if (Coef * ColumnsCount[j] >= 0)
				{
					if (Coef * ColumnsCount[j] == 4 &&
						((i + 1 == 19) || Board[i][j] != Board[i + 1][j])) { return { Board[i][j], i - 4, j }; }
					ColumnsCount[j] += Coef;
				}
				else { ColumnsCount[j] = Coef; }

				if (Coef * DiagonalsCount[CurDiag] >= 0)
				{
					if (Coef * DiagonalsCount[CurDiag] == 4 &&
						((i + 1 == 19) || ((j - 1 == -1) || Board[i][j] != Board[i][j - 1]))) { return { Board[i][j], i - 4, j + 4 }; }
					DiagonalsCount[CurDiag] += Coef;
				}
				else { DiagonalsCount[CurDiag] = Coef; }

				if (Coef * ReverseDiagonalsCount[CurInvDiag] >= 0)
				{
					if (Coef * ReverseDiagonalsCount[CurInvDiag] == 4 &&
						((i + 1 == 19) || ((j + 1 == 19) || Board[i][j] != Board[i][j + 1])))  { return { Board[i][j], i - 4, j - 4 }; }
					ReverseDiagonalsCount[CurInvDiag] += Coef;
				}
				else { ReverseDiagonalsCount[CurInvDiag] = Coef; };
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
		std::array<std::array<int, 19>, 19> Board = { {} };
		for (size_t i = 0; i < 19; i++)
		{
			if (getline(Input, InpuRow))
			{
				std::stringstream ParsedRow(InpuRow);
				for (size_t j = 0; j < 19; j++)
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
