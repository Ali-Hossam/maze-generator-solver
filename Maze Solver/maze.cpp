#include "maze.h"
Maze::Maze(int size, int cellSize, int lineThickness, int borderOffset)
{
    // maze size, where the maze is an nxn matrix
    this->size = size;

    // cell length assuming it is a rectangle
    this->cellSize = cellSize;
    
    // wall thickness
    this->lineThickness = lineThickness;
    
    // offset from left and up
    this->borderOffset = borderOffset;
    
    // an array to store data about cells
    mazeMatrix = new Cell[pow(size, 2)];

    // initalize cells row and columns data
    this->initMazeData();

}

Maze::~Maze()
{
    delete[] mazeMatrix;
}

void Maze::initMazeData()
{
    // initialize row and columns data in maze matrix
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mazeMatrix[i*size + j].row = i;
            mazeMatrix[i*size + j].col = j;
        }
    }
}

void Maze::updateMaze(int currentRow, int currentCol)
{
    // highlight visited area
    // change drawing color to Black for visited cells
    setfillstyle(1, BLACK);
    bar(currentCol * cellSize + borderOffset, currentRow * cellSize + borderOffset,
        currentCol * cellSize + borderOffset + cellSize, currentRow * cellSize + borderOffset + cellSize);

    // draw walls with White color
    setfillstyle(1, WHITE);
    int startRow, startCol, endRow, endCol;
    currentRow > 0 ? startRow = currentRow - 1 : startRow = 0;
    currentRow < size - 1 ? endRow = currentRow + 1 : endRow = currentRow;
    currentCol > 0 ? startCol = currentCol - 1 : startCol = 0;
    currentCol < size - 1 ? endCol = currentCol + 1 : endCol = currentCol;
    for (int i = startRow; i <= endRow; i++)
    {
        for (int j = startCol; j <= endCol; j++)
        {
            if (mazeMatrix[i * size + j].north)
            {
                bar(j * cellSize + borderOffset, i * cellSize + borderOffset,
                    j * cellSize + borderOffset + cellSize, i * cellSize + borderOffset + lineThickness);
            }
            if (mazeMatrix[i * size + j].south)
            {
                bar(j * cellSize + borderOffset, i * cellSize + borderOffset + cellSize,
                    j * cellSize + borderOffset + cellSize, i * cellSize + borderOffset + lineThickness + cellSize);
            }
            if (mazeMatrix[i * size + j].west)
            {
                bar(j * cellSize + borderOffset, i * cellSize + borderOffset,
                    j * cellSize + borderOffset + lineThickness, i * cellSize + borderOffset + cellSize + lineThickness);
            }
            if (mazeMatrix[i * size + j].east)
            {
                bar(j * cellSize + borderOffset + cellSize, i * cellSize + borderOffset,
                    j * cellSize + borderOffset + lineThickness + cellSize, i * cellSize + borderOffset + cellSize + lineThickness);
            }

        }
    }


}
void Maze::drawMaze()
{
    // set background color
    setbkcolor(LIGHTGRAY);
    cleardevice();

    // draw walls with White color
    setfillstyle(1, WHITE);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (mazeMatrix[i * size + j].north)
            {
                bar(j * cellSize + borderOffset, i * cellSize + borderOffset,
                    j * cellSize + borderOffset + cellSize, i * cellSize + borderOffset+lineThickness);
            }
            if (mazeMatrix[i * size + j].south)
            {
                bar(j * cellSize + borderOffset, i * cellSize + borderOffset + cellSize,
                    j * cellSize + borderOffset + cellSize, i * cellSize + borderOffset + lineThickness + cellSize);
            }
            if (mazeMatrix[i * size + j].west)
            {
                bar(j * cellSize + borderOffset, i * cellSize + borderOffset,
                    j * cellSize + borderOffset + lineThickness, i * cellSize + borderOffset + cellSize + lineThickness);
            }
            if (mazeMatrix[i * size + j].east)
            {
                bar(j * cellSize + borderOffset + cellSize, i * cellSize + borderOffset,
                    j * cellSize + borderOffset + lineThickness + cellSize, i * cellSize + borderOffset + cellSize + lineThickness);
            }

        }
    }   
}

int Maze::findNeighbors(int row, int col)
{
    // given the row and col of a cell, this function returns the idx of an unvisited cell
    // if no unvisted cell exists return -1
    std::vector<int> neighbors;
    int neighborIdx;

    // right neighbor
    if (col < (size - 1))
    {
        neighborIdx = row * size + col + 1;
        if (!mazeMatrix[neighborIdx].isVisited) { neighbors.push_back(neighborIdx); }
    }
    // left neighbor
    if (col > 0)
    {
        neighborIdx = row * size + col - 1;
        if (!mazeMatrix[neighborIdx].isVisited) { neighbors.push_back(neighborIdx); }
    }
    // lower neighbor
    if (row < (size - 1))
    {
        neighborIdx = (row + 1) * size + col;
        if (!mazeMatrix[neighborIdx].isVisited) { neighbors.push_back(neighborIdx); }
    }
    // upper neighbor
    if (row > 0)
    {
        neighborIdx = (row - 1) * size + col;
        if (!mazeMatrix[neighborIdx].isVisited) { neighbors.push_back(neighborIdx); }
    }
    
    // if no neighbors return -1
    if (!neighbors.size()) return -1;

    // return a random neighbor
    srand(time(0));
    int random = std::rand() % neighbors.size();
    return neighbors[random];
}

void Maze::removeWalls(int cellIdx, int neighborIdx)
{
    // removes wall between two cells
    // if right neighbor
    if (cellIdx == (neighborIdx - 1))
    {
        mazeMatrix[cellIdx].east = false;
        mazeMatrix[neighborIdx].west = false;
    }
    // if left neighbor
    else if (cellIdx == (neighborIdx + 1))
    {
        mazeMatrix[cellIdx].west = false;
        mazeMatrix[neighborIdx].east = false;
    }
    // if upper neighbor
    else if (cellIdx == (neighborIdx + size))
    {
        mazeMatrix[cellIdx].north = false;
        mazeMatrix[neighborIdx].south = false;
    }
    // if lower neighbor
    else if (cellIdx == (neighborIdx - size))
    {
        mazeMatrix[cellIdx].south = false;
        mazeMatrix[neighborIdx].north = false;
    }
}

void Maze::drawCircle(int row, int col, int color)
{
    int x, y, radius;
    x = col * cellSize + lineThickness / 2 + borderOffset + cellSize / 2;
    y = row * cellSize + lineThickness / 2 + borderOffset + cellSize / 2;
    radius = cellSize / 6;
    setcolor(color);
    circle(x, y, radius);
    setfillstyle(1, color);
    floodfill(x+1, y+1, color);
}

void Maze::generateMaze()
{
    drawMaze();
    std::stack<int> stacki;
    int neighbor;
    int currentCell;
    int currentRow, currentCol;

    // choose a random starting cell
    srand(time(0));
    currentRow = std::rand() % size;
    srand(time(0));
    currentCol = std::rand() % size;
    currentCell = currentRow * size + currentCol;
    stacki.push(currentCell);

    while (stacki.size())
    {
        // mark current cell as visited and find neighbors
        mazeMatrix[currentCell].isVisited = true;
        neighbor = findNeighbors(currentRow, currentCol);

        // remove walls, update stack, and update current cell
        if (neighbor != -1)
        {
            removeWalls(currentCell, neighbor);
            updateMaze(currentRow, currentCol);
            stacki.push(neighbor);
            currentCell = neighbor;
            currentRow = neighbor / size;
            currentCol = neighbor % size;  
        }
        else
        {
            // remove last element in the stack
            stacki.pop();
            updateMaze(currentRow, currentCol);

            if (stacki.size())
            {
                // make the last element in the stack the current Cell
                currentCell = stacki.top();
                currentRow = currentCell / size;
                currentCol = currentCell % size;

            }
        }
        // show progress
        drawCircle(currentRow, currentCol, YELLOW);
            Sleep(2);
    }
    Sleep(200);
    drawCircle(currentRow, currentCol, BLACK);
}