// This is the implementation of the recursive backracking algorithm for random maze generation

#pragma once
#include "Cell.h"
#include <cmath>
#include "graphics.h"
#include <vector>
#include <stack>
#include <iostream>


class Maze
{
	private:
		int size;
		int cellSize;
		int lineThickness;
		int borderOffset;
		Cell* mazeMatrix;

		void initMazeData();
		int findNeighbors(int row, int col);
		void removeWalls(int cellIdx, int neighborIdx);
		void drawCircle(int row, int col, int color);
		void updateMaze(int currentRow, int currentCol);

	public:
		void drawMaze();
		Maze(int size_ ,int cellSize_, int lineThickness_, int borderOffset_);
		void generateMaze();
		~Maze();

};
