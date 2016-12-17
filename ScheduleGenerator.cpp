///////////////////////////////////////////////////////////////////////////////
// File Name:      ScheduleGenerator.cpp
//
// Author:         Curtis Weber and Omair Chughtai
// CS email:       cweber@cs.wisc.edu , omair@cs.wisc.edu
//
// Description:	   Working concept program with the aim of demonstrating that
//				   the UW-Madison Schedule Generator could use the addition of
//				   two new functions: finding schedules that end earliest and 
//				   start latest
//
// IMPORTANT NOTE: This file should NOT be modified.
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

void createDB(vector<string> &courseNames, vector<vector<int> > &coursesDB);

void startLatest(vector<string> courseNames, vector<vector<int> > coursesDB,
		int sectionsPerClass);

void endEarliest(vector<string> courseNames, vector<vector<int> > coursesDB,
		int sectionsPerClass);

void printSchedule(vector<string> courseNames, vector<vector<int> > coursesDB,
		int classSections[], int classTimes[] );

/**
 * Fills in the vector based databases of courseNames and courses used by the
 * startLatest and endEarliest functions to find the correct schedule by reading
 * in the courses.txt file.
 * @param courseNames The string vector of courseNames used by the main program
 * @param coursesDB The multidemensional vector of int vectors used to hold the
 * schedules of courses (index corresponds between courseNames and coursesDB)
 */
void createDB(vector<string> &courseNames, vector<vector<int> > &coursesDB){
	ifstream inFile;
	inFile.open("courses.txt");
	string line;

	//Read in the courses.txt file
	while (getline(inFile, line)) {
		string delimiter = ",";
		int commaIndex = 0;
		string token;
		vector<int> holdCourse;

		// Splitting line at commas into strings of the information
		int count = 0;
		while ((commaIndex = line.find(delimiter)) != string::npos) {
			token = line.substr(0, commaIndex);
			line.erase(0, commaIndex + delimiter.length());

			// Checking if token is course name or time information
			if (count == 0) {
				courseNames.push_back(token);
				count++;
			} else {
				holdCourse.push_back(atoi(token.c_str()));
			}
		}
		coursesDB.push_back(holdCourse);
	}
}

/**
 * Generates the schedule with all sections starting as late as possible in the
 * day. Called by the main function.
 * @param courseNames The string vector of courseNames used by the main program
 * @param coursesDB The multidemensional vector of int vectors used to hold the
 * schedules of courses (index corresponds between courseNames and coursesDB)
 * @param sectionsPerClass The number of sections per class
 */
void startLatest(vector<string> courseNames, vector<vector<int> > coursesDB,
		int sectionsPerClass) {

	int sectionStartTimes[coursesDB.size()];
	int numSections = (sizeof(sectionStartTimes)/sizeof(int));
	int numClasses = numSections/sectionsPerClass;
	int classLatestSectionIndex[numClasses];
	int classLatestSectionTimes[numClasses];

	//Find latest start time
	//For every class
	for (int i = 0; i < numClasses; i++) {

		int classLatestStartTime = 0;
		int section = 0;

		//For every section of each class
		for(int j = 0; j < sectionsPerClass; j++) {

			int startTime = 0;
			int sectionIndex = (i*sectionsPerClass)+j;
			vector<int> hold = coursesDB[sectionIndex];

			//Find latest starting section
			for (int k = 24; k > 0; k--) {

				sectionStartTimes[sectionIndex] = k;

				if (hold[k-1] == 1) {
					startTime = k;
				}
			}

			if(startTime > classLatestStartTime) {
				classLatestStartTime = startTime;
				section = sectionIndex;
			}

		}
		classLatestSectionIndex[i] = section;
		classLatestSectionTimes[i] = classLatestStartTime;
	}

	vector<int> classLatestSectionTimesCopy;

	for (int i = 0; i < numClasses; i++) {

		classLatestSectionTimesCopy.push_back(classLatestSectionTimes[i]);

	}

	bool scheduleFound = true;

	//Sort sections
	sort(classLatestSectionTimesCopy.begin(), classLatestSectionTimesCopy.end());

	for(int i = 0; i < numClasses-1; i++) {

		if (classLatestSectionTimesCopy[i] == classLatestSectionTimesCopy[i + 1]) {

			scheduleFound = false;
			break;
		}
	}

	if (scheduleFound) {
		printSchedule(courseNames, coursesDB, classLatestSectionIndex,
				classLatestSectionTimes);
	} else {
		cout << "SCHEDULE NOT FOUND" << endl;
	}
}

/**
 * Generates the schedule with all sections ending as early as possible in the
 * day. Called by the main function.
 * @param courseNames The string vector of courseNames used by the main program
 * @param coursesDB The multidemensional vector of int vectors used to hold the
 * schedules of courses (index corresponds between courseNames and coursesDB)
 * @param sectionsPerClass The number of sections per class
 */

void endEarliest(vector<string> courseNames, vector<vector<int> > coursesDB,
		int sectionsPerClass) {

	int sectionStartTimes[coursesDB.size()];
	int numSections = (sizeof(sectionStartTimes)/sizeof(int));
	int numClasses = numSections/sectionsPerClass;
	int classLatestSectionIndex[numClasses];
	int classLatestSectionTimes[numClasses];

	//For every class
	for (int i = 0; i < numClasses; i++) {

		int classLatestStartTime = 25;
		int section = 0;

		//For every section per class
		for(int j = 0; j < sectionsPerClass; j++) {

			int startTime = 0;
			int sectionIndex = (i*sectionsPerClass)+j;
			vector<int> hold = coursesDB[sectionIndex];

			//Find latest starting section
			for (int k = 24; k > 0; k--) {

				sectionStartTimes[sectionIndex] = k;

				if (hold[k-1] == 1) {

					startTime = k;
				}
			}

			if(startTime < classLatestStartTime) {

				classLatestStartTime = startTime;
				section = sectionIndex;
			}

		}
		classLatestSectionIndex[i] = section;
		classLatestSectionTimes[i] = classLatestStartTime;

	}

	vector<int> classLatestSectionTimesCopy;

	//Copy vector of sections
	for (int i = 0; i < numClasses; i++) {
		classLatestSectionTimesCopy.push_back(classLatestSectionTimes[i]);
	}

	bool scheduleFound = true;

	//Sort vector of sections
	sort(classLatestSectionTimesCopy.begin(), classLatestSectionTimesCopy.end());


	for(int i = 0; i < numClasses-1; i++) {
		if (classLatestSectionTimesCopy[i] == classLatestSectionTimesCopy[i + 1]) {
			scheduleFound = false;
			break;
		}
	}

	if (scheduleFound) {
		printSchedule(courseNames, coursesDB, classLatestSectionIndex,
				classLatestSectionTimes);
	} else {
		cout << "SCHEDULE NOT FOUND" << endl;
	}
}

/**
 * Corrrectly formats and outputs the schedule of choice to the console so for
 * the user to view.
 * @param courseNames The string vector of courseNames used by the main program
 * @param coursesDB The multidemensional vector of int vectors used to hold the
 * schedules of courses (index corresponds between courseNames and coursesDB)
 * @param classSections[] An array of the class sections
 * @param classTimes[] An array of the class times
 */
void printSchedule(vector<string> courseNames, vector<vector<int> > coursesDB,
		int classSections[], int classTimes[] ) {

	int size = sizeof(classSections)/2;

	cout << "One Day Schedule" << endl;
	cout << "Time | Class" <<endl;

	for ( int i= 0; i < 24; i++) {

		if(i < 12){
			cout << (i % 12) +1 << "am    ";
		}

		if(i > 11){
			cout << (i % 12) +1 << "pm    ";
		}

		bool classHere = false;

		for ( int j = 0; j < size ;j++) {

			if ( i+1 == classTimes[j]) {

				cout << courseNames[classSections[j]] << endl;
				classHere = true;
			}
		}

		if (!classHere) {
			cout << "------" << endl;
		}

	}

}





/**
 * @brief The program execution begins here. Executes the code, calls functions
 * for database creation and the functions that find the user specified schedule.
 * @param argv The command line arguments.
 * @return 0 for normal program termination, -1 for abnormal termination.
 */
int main(int argc, char *argv[]) {

	vector<string> courseNames;
	vector<vector<int> > coursesDB;
	int sectionsPerClass = 3;

	// Create courses database
	createDB(courseNames, coursesDB);

	// Create user chosen database of courses
	vector<string> courseNamesSelected;
	vector<vector<int> > coursesDBSelected;

	string courses[] = {"chem103","cs367","cs354", "anthro101", "math319",
			"bio522", "lit455", "epd677" };
	cout << "Choose Classes from following by entering number (Type Done to finish): "
			<< endl;

	//List courses
	for(int i = 1; i<= sizeof(courses)/sizeof(courses[0]);i++){
		cout << i << ": " << courses[i-1] << endl;
	}

	//Get user courses selection
	string input = "";
	while(input != "Done") {

		input = "";
		getline(cin,input);

		if (input != "Done") {

			int num = atoi(input.c_str())-1;
			int sizeCourses = sizeof(courses)/sizeof(courses[0]);

			if(num >= 0 && num < sizeCourses ) {

				int sectionsIndexes = num*sectionsPerClass;

				for( int i = 0; i < sectionsPerClass; i++) {

					courseNamesSelected.push_back(courseNames[sectionsIndexes+i]);
					coursesDBSelected.push_back(coursesDB[sectionsIndexes+i]);

				}

			} else {
				cout << "Enter integer between 1 and " << sizeCourses
						<< " to select a class." << endl;
			}
		}
	}

	// Ask user if they want to start early or latest
	cout << "Select type of schedule desired by entering number:" << endl;
	cout << "1. End classes as early as possible." << endl;
	cout << "2. Start classes as late as possible." << endl;

	bool notCorrect = true;
	string input2 = "";

	while (notCorrect){

		input = "";
		getline(cin,input2);
		int selection = atoi(input2.c_str());

		// Generate earliest end time
		if (selection == 1) {
			endEarliest(courseNamesSelected, coursesDBSelected, sectionsPerClass);
			notCorrect = false;

			// Generate latest start time
		} else if (selection == 2) {
			startLatest(courseNamesSelected, coursesDBSelected, sectionsPerClass);
			notCorrect = false;
		} else {
			cout << "Invalid Selection. Enter either 1 or 2." << endl;
		}
	}
	return 0;
}

