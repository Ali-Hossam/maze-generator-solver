#pragma once
#include "graphics.h"
#include <string>
#include <map>
#include <iostream>
class Robot
{
	private:
		int pose[2];	// robot position
		int startPose[2]; // robot intial position
		int width, height;	// robot dimensions
		int fillClr, borderClr;	// robot colors
		int robotAngle = 180;  // 1 for forward motion and -1 for backward 
		int readings[3];	// IR sensor readings
		int **bkgPixelMap;	// a matrix to store background data
		int sensorPose;	// distance between robot starting point to sensor
		int sensorLine[6];	
		int cellSize;	// maze cell size
		int direction;	// robot direction of motion
		std::string path;	// store moving directions
		std::map <int, int> readingsMap;	// convert colors to 0s and 1s
		std::map <std::string, std::string> pathOptimization;	// for reducing path
		bool isPath;	// a boolean to change the color of the optimized path
		
		// functions
		void drawRobot();
		void updateSensorReadings();
		void move();
		void rotate(int);
		void clearRobotDrawing();

	public:
		Robot(int startPose[2], int width, int height, int color, int borderClr, int sensorPose, int cellSize);
		int exploreMaze(std::string, bool=true);
		void showPath();
		~Robot();
};

