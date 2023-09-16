#include "graphics.h"
#include "maze.h"
#include "Robot.h"
#pragma comment(lib,"graphics.lib")


int main()
{

    int width = 800;
    initwindow(width, width);

    // random maze
    int numOfCells = 20;
    int cellSize = width / 25;
    int lineThickness = cellSize / 4;
    int borderOffset = (width - numOfCells * cellSize - 2 * lineThickness) / 2;
    Maze maze(numOfCells, cellSize, lineThickness, borderOffset);
    maze.drawMaze();
    maze.generateMaze();

    // draw one wall with green color
    srand(time(0));
    int i = std::rand() % cellSize;
    srand(time(0));
    int j = std::rand() % cellSize;
    setfillstyle(1, GREEN);
    bar(j * cellSize + borderOffset, i * cellSize + borderOffset - lineThickness,
        j * cellSize + borderOffset + cellSize + lineThickness, i * cellSize + borderOffset + 2 * lineThickness);

    // initialize robot
    int robotWidth = lineThickness * 2;
    int robotHeight = lineThickness * 2;
    int pose[] = { borderOffset - (robotWidth - lineThickness) / 2,
                    borderOffset - (robotWidth - lineThickness) / 2 };
    int robotSensorPose = robotHeight / 2;
    Robot robot(pose, robotWidth, robotHeight, RED, LIGHTRED, robotSensorPose, cellSize);

    // start moving
    Sleep(200);
    robot.exploreMaze("F");
    robot.showPath();
    
    getch();
    closegraph();
    return 0;
}
