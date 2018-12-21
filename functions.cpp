#include <iostream>
#include <fstream>
#include <cmath>
#include "functions.h"
using namespace std;

void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int& max, int& min) {
	max = elevations[0][0];
	min = elevations[0][0];

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (elevations[i][j] > max) {	//Finding maximum value
				max = elevations[i][j];
			}
			if (elevations[i][j] < min) {	//Finding minimum value
				min = elevations[i][j];
			}
		}
	}

}

void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, istream& inData) {
	//Find total number of elements in elevations matrix
	int totalIntegers = rows * cols;

	int count = 0;		//Keep count of numbers in matrix
	double checkNum;	//number being tested
	int num;			//number being placed in matrix

	int j = 0;
	int i = 0;
	while (!inData.eof()) {
		inData >> checkNum;
		num = checkNum;

		//Message if character found in data
		if ((checkNum == 0) && (inData.fail())) {
			cout << "Error: Read a non-integer value." << endl;
			exit(1);
		}

		if (!inData.fail()) {
			if (count == totalIntegers) {
				cout << "Error: Too many data points." << endl;		//Message if matrix is already fully loaded
				exit(1);
			}
			if (num == checkNum) {		//Put number in elevation matrix if it's an integer
				elevations[i][j] = num;
				count += 1;
				j += 1;
			}
			else if (num != checkNum) {
				cout << "Error: Read a non-integer value." << endl;	//Message if value is not an integer
				exit(1);
			}
			if (inData.fail()) {
				cout << "Error: Problem reading the file." << endl;	//Message if error occurs
				exit(1);
			}
			if (j == cols) {
				i += 1;
				j = 0;
			}
		}
	}

	//Message if end-of-file was not reached
	if (!inData.eof()) {
		cout << "Error: End of file reached prior to getting all required data." << endl;
		exit(1);
	}

	//Message if matrix wasn't fully loaded
	if (count != totalIntegers) {
		cout << "Error: End of file reached prior to getting all required data." << endl;
		exit(1);
	}
}

int scaleValue(int value, int max, int min) {
	double shade = ((value - min) / static_cast<double>(max - min)) * 255;	//Calculate shade
	int roundedShade = round(shade);										//Round shade

	return roundedShade;
}

void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], const int elevations[MAX_ROWS][MAX_COLS],
	int rows, int cols, int max, int min) {

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int elevationNum = elevations[i][j];
			int shade = scaleValue(elevationNum, max, min);		//Find shade
			image[i][j] = { shade, shade, shade };				//Put shade value into corresponding address in image matrix
		}														//Notice each has three values, one for each color in Pixel
	}
}

void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, ostream& outData) {
	//Write in PPM file
	outData << "P3" << endl;
	outData << cols << " " << rows << endl;  //Write width & height

	int maxColorValue = 255;
	outData << maxColorValue << endl;

	//Write RGB values
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			outData << image[i][j].red << " " << image[i][j].green << " " << image[i][j].blue << " ";
		}
		outData << endl;
	}
}

int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row) {
	int totalDistance = 0;			//Keep track of vertical distance
	int currentRow = start_row;
	int currentColumn = 0;
	
	image[currentRow][currentColumn] = color;

	//Find greedy path
	while (currentColumn < cols-1) {

		//if at top of map
		if (currentRow == 0) {
			cout << elevations[currentRow][currentColumn] << endl;
			int distance1 = abs((elevations[currentRow][currentColumn + 1]) - (elevations[currentRow][currentColumn]));     //forward-right
			int distance2 = abs((elevations[currentRow + 1][currentColumn + 1]) - (elevations[currentRow][currentColumn]));     //lower-right

			if (distance1 <= distance2) {							//move fwd
				totalDistance += distance1;
				image[currentRow][currentColumn + 1] = color;		//change RGB value
				currentColumn += 1;
			}
			else {													//move fwd-down
				totalDistance += distance2;
				image[currentRow + 1][currentColumn + 1] = color;	//change RGB value
				currentRow = currentRow + 1;
				currentColumn += 1;
			}
		}

		//if at bottom of map
		else if (currentRow == rows - 1) {	
			int distance1 = abs((elevations[currentRow - 1][currentColumn + 1]) - (elevations[currentRow][currentColumn]));     //upper-right
			int distance2 = abs((elevations[currentRow][currentColumn + 1]) - (elevations[currentRow][currentColumn]));     //forward-right

			if (distance2 <= distance1) {										//move fwd
				totalDistance += distance2;
				image[currentRow][currentColumn + 1] = color;					//change RGB value
				currentColumn += 1;
			}
			else {																//move fwd-up
				totalDistance += distance1;
				image[currentRow - 1][currentColumn + 1] = color;				//change RGB value
				currentRow = currentRow - 1;
				currentColumn += 1;
			}
		}

		else {
			int distance1 = abs((elevations[currentRow - 1][currentColumn + 1]) - (elevations[currentRow][currentColumn]));     //upper-right
			int distance2 = abs((elevations[currentRow][currentColumn + 1]) - (elevations[currentRow][currentColumn]));     //forward-right
			int distance3 = abs((elevations[currentRow + 1][currentColumn + 1]) - (elevations[currentRow][currentColumn]));     //lower-right

			if ((distance2 <= distance1) && (distance2 <= distance3)) {			//move fwd
				totalDistance += distance2;
				image[currentRow][currentColumn + 1] = color;					//change RGB value
				currentColumn += 1;
			}
			else if ((distance3 <= distance1) && (distance3 < distance2)) {		//move fwd-down
				totalDistance += distance3;
				image[currentRow + 1][currentColumn + 1] = color;				//change RGB value
				currentRow = currentRow + 1;
				currentColumn += 1;
			}
			else {																//move fwd-up
				totalDistance += distance1;
				image[currentRow - 1][currentColumn + 1] = color;				//change RGB value
				currentRow = currentRow - 1;
				currentColumn += 1;
			}
		}
	}
	return totalDistance;
}