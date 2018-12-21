#include <iostream>
#include <fstream>
#include "functions.h"
using namespace std;

#include <cstring>

// Normally you should not use global variables. However, the stack is
//   limited in size and will not allow arrays of this size.
static int elevations[MAX_ROWS][MAX_COLS];
static Pixel image[MAX_ROWS][MAX_COLS];

int main() {
	ifstream inFS;
	char fileName[500];
	int numRows = 0;
	int numColumns = 0;

	//Get input
	cin >> numRows >> numColumns >> fileName;

	//Check if user's input is valid
	if ((!cin.good()) || (numRows <= 0) || (numColumns <= 0) || (numRows > 1000) || (numColumns > 1000)) {
		cout << "Error: Problem reading in rows and columns." << endl;
		return 1;
	}

	//Open file
	inFS.open(fileName);

	//Message if error occurs when opening file
	if (!inFS.is_open()) {
		cout << "Error: Unable to open file <" << fileName << ">." << endl;
		return 1;
	}

	//Read the file's data and put into elevations matrix
	loadData(elevations, numRows, numColumns, inFS);

	//Close file
	inFS.close();

	//Find Min & Max of data
	int max;
	int min;
	findMaxMin(elevations, numRows, numColumns, max, min);

	//Find grey shade of each elevation point in elevation matrix and put into image matrix
	loadGreyscale(image, elevations, numRows, numColumns, max, min);

	//Part 2
	//Color each path red
	Pixel colorRed = { 252, 25, 63 };
	int i = 0;

	//Find the greedy path
	int shortestDistance = colorPath(elevations, image, numRows, numColumns, colorRed, 0);
	int greedyRow = 0;

	while (i < numRows) {
		int distance = colorPath(elevations, image, numRows, numColumns, colorRed, i);
		if (distance < shortestDistance) {	//Check if its the shortest path thus far
			shortestDistance = distance;
			greedyRow = i;
		}
		i += 1;
	}

	//Color the greedy path green
	Pixel colorGreen = { 31, 253, 13 };
	colorPath(elevations, image, numRows, numColumns, colorGreen, greedyRow);

	//Create PPM file
	ofstream outFS;
	char ppmFileName[500];
	char ending[10] = ".ppm";
	strcpy(ppmFileName, fileName);
	strcat(ppmFileName, ending);

	//Open PPM file
	outFS.open(ppmFileName);

	//Message if error occurs when opening file
	if (!outFS.is_open()) {
		cout << "Error: Unable to open file <" << ppmFileName << ">." << endl;
		return 1;
	}

	//Write PPM data into PPM file
	outputImage(image, numRows, numColumns, outFS);

	//Close PPM file
	outFS.close();

	return 0;
}