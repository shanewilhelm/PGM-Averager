/**********************************************************************************************
Description: This program takes all PGM files in its directory and averages them into one PGM.

Author: Shane Wilhelm
Based on Information From:	http://courses.cs.vt.edu/cs2604/fall01/binio.html &
							StackOverflow users: Peter Parker and computinglife.

Date: 24 November 2013

Copywrite: This software is free for all non-commercial use.
**********************************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <direct.h>
#include "dirent.h"

using namespace std; 

int main(void)
{
	ifstream inputFile;
	ofstream outputFile;
	string userInput = "output.pgm";
	int numFiles = 0;
	vector<string> pgmList;

	cout << "*********PGM Image Averager*********" << endl;
	cout << "This program will take all .pgm files" << endl;
	cout << "in its local directory and average" << endl;
	cout << "each byte of data to write to an output" << endl;
	cout << "file." << endl << endl;

	cout << "Enter the name of your output file \n(one will be created if it does not already exist): ";
	getline(cin, userInput);
	outputFile.open(userInput, ios::out | ios::binary);

	char cCurrentPath[FILENAME_MAX];
	_getcwd(cCurrentPath, sizeof(cCurrentPath));

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(cCurrentPath)) != NULL) 
	{
		//Go through each file in the local directory.
		while ((ent = readdir(dir)) != NULL) 
		{
			string fName = ent->d_name;
			int size = fName.size();

			//Check if the file ends in "pgm"
			if (size >= 3 && fName[size - 3] == 'p' && fName[size - 2] == 'g' && fName[size - 1] == 'm')
			{
				//The found file is not equal to the output file
				if (fName != userInput)
				{
					//Make sure the pgmList can fit our added item.
					if (pgmList.capacity() <= (numFiles + 1))
					{
						pgmList.resize((numFiles * 2) + 1);
					}

					//Add the name of this pgm file to a list of found pgm's
					pgmList[numFiles] = fName;
					numFiles++;
				}
			}
		}
		closedir(dir);
	}
	else
	{
		//Could not open the current directory. 
		return EXIT_FAILURE;
	}

	if (numFiles == 0)
	{
		cout << "No '*.pgm' files found in current directory. Exiting..." << endl;
		return EXIT_FAILURE;
	}

	//Get the size of the first file in the list. 
	inputFile.open(pgmList[0], ios::in | ios::binary);
	int fileSize = 0;
	inputFile.seekg(0, ifstream::end);
	fileSize = inputFile.tellg();
	inputFile.close();

	vector<int> outputBuffer;
	outputBuffer.resize(fileSize + 1);

	//Go through each found 'pgm' file
	for (int i = 0; i < numFiles; i++)
	{
		vector<char> inputBuffer;
		inputBuffer.resize(fileSize + 1);

		//Open a 'pgm' file from the pgmList
		inputFile.open(pgmList[i], ios::in | ios::binary);

		//Read the entire file into the input buffer. Each element is a byte of data.
		inputFile.read(&inputBuffer[0], fileSize);	

		//Add each element of the input buffer to the cummulative output buffer elements.
		for (int i = 0; i < outputBuffer.size(); i++)
		{
			outputBuffer[i] += inputBuffer[i];
		}

		inputFile.close();
	}

	//Divide each element of the output buffer by the number of files, averaging each byte of each file.
	for (int i = 0; i < outputBuffer.size(); i++)
	{
		outputBuffer[i] /= numFiles;
	}

	vector<char>outputCharBuffer;
	outputCharBuffer.resize(outputBuffer.size());

	for (int i = 0; i < outputBuffer.size(); i++)
	{
		outputCharBuffer[i] = (char)(outputBuffer[i]);
	}

	//Write the output buffer to the output file.
	outputFile.write(&outputCharBuffer[0], fileSize);

	outputFile.close();

	return 0;
}