#pragma once
struct Cell
{
	int row;	// row of the cell
	int col;	// column of the cell
	bool isVisited = false;	// whether or not the cell has been visited
	bool north=true, south=true, east=true, west=true;	// to indicate where wall are around the cell
};

