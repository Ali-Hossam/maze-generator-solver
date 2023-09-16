#include "Robot.h"
Robot::Robot(int startPose[2], int width, int height, int color, 
	int borderClr, int sensorPose, int cellSize)
{
	this->startPose[0] = startPose[0];
	this->startPose[1] = startPose[1];
	pose[0] = startPose[0];
	pose[1] = startPose[1];
	this->width = width;
	this->height = height;
	this->fillClr = color;
	this->borderClr = borderClr;
	this->sensorPose = sensorPose;
	this->cellSize = cellSize;
	bkgPixelMap = new int *[width+1];
	for (int i = 0; i < width+1; i++)
	{
		bkgPixelMap[i] = new int[height+1];
	}

	readingsMap[15] = 1;
	readingsMap[0] = 0;
	readingsMap[7] = 0;
	readingsMap[LIGHTBLUE] = 1;
	readingsMap[GREEN] = 2;

	pathOptimization["LBL"] = "F";
	pathOptimization["LBF"] = "R";
	pathOptimization["RBL"] = "B";
	pathOptimization["LBR"] = "B";
	pathOptimization["FBL"] = "R";
	pathOptimization["FBF"] = "B";

	updateSensorReadings();

}

void Robot::rotate(int isRight)
{
	// change robot angle from 0 to 270, based on it's orientation
	// where 0 is the orientation at which the robot points upward, 90-> right, 270->left and 180 -> bottom
	if (isRight)
	{
		robotAngle == 270 ? robotAngle = 0 : robotAngle += 90;
	}
	else
	{
		robotAngle == 0 ? robotAngle = 270 : robotAngle -= 90;
		
	}
	if (robotAngle == 0) { direction = 1; }
	else if (robotAngle == 90) { direction = 2; }
	else if (robotAngle == 180) { direction = 0; }
	else if (robotAngle == 270) { direction = 3; }
	clearRobotDrawing();
	updateSensorReadings();
	
}

void Robot::clearRobotDrawing()
{
	// replaces current robot drawing with pixel colors stored in bkgPixelMap matrix
	int pixelColor;
	// clear the current robot drawing
	for (int i = 0; i <= width; i++)
	{
		for (int j = 0; j <= height; j++)
		{
			bkgPixelMap[i][j] == WHITE ? pixelColor = LIGHTBLUE : pixelColor = bkgPixelMap[i][j];
			if (isPath)
			{
				bkgPixelMap[i][j] == LIGHTBLUE ? pixelColor = MAGENTA : pixelColor = bkgPixelMap[i][j];
			}
			putpixel(pose[0] + i, pose[1] + j, pixelColor);
		}
	}
}
void Robot::updateSensorReadings()
{
	// updates sensor readings based on the direction of the robot
	// 0 - down
	// 1 - up
	// 2 - right
	// 3 - left

	int x1, x2, x3, y1, y2, y3;
	switch (direction)
	{
	case 0:
		x1 = pose[0];
		x2 = x1 + width / 2;
		x3 = x1 + width;
		y1 = y3 = pose[1] + sensorPose;
		y2 = y1 + sensorPose - 3;
		//sign == 1 ? y2 = y1 + sensorPose - 5 : y2 = y1 - sensorPose + 7;
		break;

	case 1:
		x3 = pose[0];
		x2 = x3 + width / 2;
		x1 = x3 + width;
		y1 = y3 = pose[1] + sensorPose;
		y2 = y1 - sensorPose + width/3;
		break;

	case 2:
		x1 = x3 = pose[0] + sensorPose;
		x2 = x1 + sensorPose - width/4;
		y3 = pose[1];
		y2 = pose[1] + height / 2;
		y1 = pose[1] + height;
		break;
	
	case 3:
		x1 = x3 = pose[0] + sensorPose;
		x2 = x1 - sensorPose + width/3;
		y1 = pose[1];
		y2 = pose[1] + height / 2;
		y3 = pose[1] + height;
		break;
	}

	// update sensor line to draw it
	sensorLine[0] = x1;
	sensorLine[1] = y1;
	sensorLine[2] = x2;
	sensorLine[3] = y2;
	sensorLine[4] = x3;
	sensorLine[5] = y3;

	// get sensors readings
	readings[0] = readingsMap[getpixel(x1, y1)];
	readings[1] = readingsMap[getpixel(x2, y2)];
	readings[2] = readingsMap[getpixel(x3, y3)];
	//std::cout << "\nReadings\n" << readings[0] << readings[1] << readings[2];
	drawRobot();
}

void Robot::drawRobot()
{
	int x = pose[0], y = pose[1];
	// store background pixels before drawing the robot
	for (int i = 0; i <= width; i++)
	{
		for (int j = 0; j <= height; j++)
		{
			bkgPixelMap[i][j] = getpixel(x + i, y + j);
		}
	}

	// draw the robot
	setcolor(borderClr);
	rectangle(x, y, x + width, y + height);
	setfillstyle(1, fillClr);
	floodfill(x+1, y+1, borderClr);

	// draw sensors line
	setcolor(borderClr);
	line(sensorLine[0], sensorLine[1], sensorLine[4], sensorLine[5]);
	circle(sensorLine[2], sensorLine[3], width/4);
	setfillstyle(1, YELLOW);
	floodfill(sensorLine[2] + 1, sensorLine[3] + 1, borderClr);

}

void Robot::move()
{
	// updates positing robot pose based on it's direction
	clearRobotDrawing();
	// change pose
	// 0 - down
	// 1 - up
	// 2 - right
	// 3 - left
	switch (direction)
	{
		case 0:
			pose[1] += cellSize/2;
			break;
		case 1:
			pose[1] -= cellSize/2;
			break;
		case 2:
			pose[0] += cellSize/2;
			break;
		case 3:
			pose[0] -= cellSize/2;
			break;

	}
	updateSensorReadings();
	Sleep(20);
}

int Robot::exploreMaze(std::string route, bool updatePath)
{
	// finds the possible solution of the maze
	if (route == "R")
	{
		path.append("R");
		rotate(1);
		move();
	}
	else if (route == "L")
	{
		path.append("L");
		rotate(0);
		move();
	}
	else if (route == "F")
	{
		// if the robot is at a node or there is no F in the stack
		if (updatePath) path.append("F");
		move();
	}
	while (readings[1]== 1 && readings[1] + readings[0] + readings[2] == 1) move();

	if (readings[0] + readings[1] + readings[2] == 0)
	{
		//rotate 180 deg
		rotate(1);
		rotate(1);
		path.append("B");
		return 0;
	}

	else if (readings[2] == 1)
	{
		exploreMaze("L");
	}
	else if (readings[1] == 1)
	{
		exploreMaze("F");
	}
	else if (readings[0] == 1)
	{
		exploreMaze("R");
	}

	if (readings[1] == 2) { return 1; }
	else
	{
		exploreMaze("F", 0);
	}
}

void Robot::showPath()
{
	// reduce the path and get the optimized one
	// before optimization
	std::cout << "\nPath Before Optimization: \n" << path;

	// optimizing the path
	int idx = path.find("B");
	std::string substring;
	while (idx != -1)
	{
		substring = path.substr(idx - 1, 3);
		path.replace(idx - 1, 3, pathOptimization[substring]);
		idx = path.find("B");
	}
	std::cout << "\nOptimized Path: \n" << path;


	// change the color of the optimized path
	// Clear Robot Drawing
	Sleep(1000);
	// reset position
	pose[0] = startPose[0];
	pose[1] = startPose[1];
	direction = 0;
	robotAngle = 180;
	isPath = true;
	drawRobot();
	move();
	
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == 'R') rotate(1);
		else if (path[i] == 'L') rotate(0);
		move();
		while (readings[1] == 1 && readings[1] + readings[0] + readings[2] == 1) move();

	}
	//move();
}

Robot::~Robot()
{
	for (int i = 0; i < width; i++)
	{
		delete[] bkgPixelMap[i];
	}
	delete[] bkgPixelMap;
}