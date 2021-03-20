#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int main();
bool isNumeric(std::string checkString);
int usrInput(std::string outMessage, int lowerLength, int upperLength);
std::vector<std::string> splitString(const std::string &sentence, char delim);
int grayIndex(std::string grayCode);
int grayCode(int grayIndex);
void display(int** thisArray, int rows, int cols);
bool findIntGrid(int xLim, int yLim, int hor, int ver, int xstart, int ystart, int** kMap);
int wrapBack(int index, int maxVal);
std::string strLeft(std::string strIn, int count);
std::string strRight(std::string strIn, int count);

int main()
{
	std::string tmpInputs = "";
	int intInputs = usrInput("inputs", 1, 5);
	std::cout << "Enter the unsimplified boolean expression as sums of products" << std::endl;
	std::cin >> tmpInputs;
	std::vector<std::string> sumoprod = splitString(tmpInputs, '+');
	
	int iLength =0 ; int jLength= 0;
	int xLen; int yLen;
	switch (intInputs)
	{
		case 2: 
			iLength = 2; jLength = 2; xLen = 1; yLen = 1; break;
		case 3: 
			iLength = 4; jLength = 2; xLen = 2; yLen = 1; break;
		case 4: 
			iLength = 4; jLength = 4; xLen = 2; yLen = 2; break;
	}
	int** kMap;
	kMap = new int*[iLength];
	for (int l = 0; l < iLength; l++)
	{
		kMap[l] = new int[jLength];
		for (int h = 0; h < jLength; h++)
		{
			kMap[l][h] = 0;
		}
	}
	
	int grayX = 0; int grayY = 0;
	for (int k = 0; k < sumoprod.size(); k++)
	{
		tmpInputs = sumoprod[k];
		grayX = grayIndex(strLeft(tmpInputs, xLen));
		grayY = grayIndex(strRight(tmpInputs, yLen));
		kMap[grayX][grayY] = 1;
	}
	display(kMap, iLength, jLength);
	return 0;
}

bool isNumeric(std::string checkString)
{
	for (int i = 0; i < checkString.length(); i++)
	{
		if (isdigit(checkString[i]) == false)
		{ 
			return false;
		}
	}
	return true;	
}

int usrInput(std::string outMessage, int lowerLength, int upperLength)
{
	std::cout << "Enter the number of " << outMessage << std::endl;
	std::string tmpInput;
	bool isValid = false;
	while (!isValid)
	{
		std::cin >> tmpInput;
		if (isNumeric(tmpInput))
		{
			if ((stoi(tmpInput) < upperLength) && (stoi(tmpInput) > lowerLength))
			{
				std::cout << "Input for " << outMessage << " is valid." << std::endl;
				isValid = true;
			} else {
				std::cout << "Value is out of bounds." << std::endl; 
			}
		} else {
			std::cout << "Value is not a positive integer." << std::endl;
		}
	}	
	return stoi(tmpInput);
}

int grayIndex(std::string grayCode)
{
	switch (stoi(grayCode))
	{
		case 0: return 0;
		case 1: return 1;
		case 11: return 2;
		case 10: return 3; 
		default: return 0;
	}
}

int grayCode(int grayIndex)
{
	switch (grayIndex)
	{
		case 0: return 0;
		case 1: return 1;
		case 2: return 11;
		case 3: return 10;
	}
}

std::vector<std::string> splitString(const std::string &sentence, char delim)
{
	std::vector<std::string> elements;
	std::string word;
	std::stringstream strStream(sentence);
	while (std::getline(strStream, word, delim))
	{
		elements.push_back(word);
	}
	return elements;
}

void display(int** thisArray, int rows, int cols) 
{
	int x = rows  - 1; int y = cols - 1;
	std::string thisRow; std::string thisCol;
	std::vector<std::string> expression;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << thisArray[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	//simplified expression:	
	// check right bottom then continue decrementing until reach 0. only search right / down and wrap back if needed.
	// when getting the collection of grayCode that had 1s, OR the cols with each other and OR the rows with each other together to get a simplified expression
	// if found then insta leave the if loops - return to the for loop
	int xSearch = 0; int ySearch = 0;
	bool foundInCycle = false;
	for (int i = rows - 1; i == 0; i--)
	{
		for (int j = cols - 1; j == 0; j--)
		{
			// by design rows >= cols so no need for 1 or 2x4
			// if rows = 4 then search for 4x
			// if cols = 4 then allow for 4x4
			// else allow for 4x2
			// else allow for 4x1
			thisRow = ""; thisCol  = "";
			foundInCycle = false;
			std::vector<std::string> tmpRow;
			std::vector<std::string> tmpCol;
			xSearch = rows; ySearch = cols;
			while ((!foundInCycle) && (xSearch > 0) && (ySearch > 0))
			{
				if (findIntGrid(xSearch, ySearch, rows, cols, i, j,thisArray))
				{
					foundInCycle = true;
					
				}
				
				// reduce ySearch every other, and reduce 
				xSearch -= 1; ySearch = -1;
			}
		}
	}
	
}

bool findIntGrid(int xLim, int yLim, int hor, int ver, int xstart, int ystart, int** kMap)
{
	for (int i = xstart; i < hor; i++)
	{
		i = wrapBack(i, xLim);
		for (int j = ystart; j < ver; j++)
		{
			j = wrapBack(j, yLim);
			if (kMap[i][j] == 0) 
			{
				return false;
			}
		}
	}
	return true;
}

int wrapBack(int index, int maxVal)
{
	return (index * (!(index > maxVal)));
}

std::string strLeft(std::string strIn, int count)
{
	std::string tmpStr(1, strIn[0]);
	if (count == 2)
	{
		tmpStr.push_back(strIn[1]);
	}
	return tmpStr;
}

std::string strRight(std::string strIn, int count)
{
	std::string tmpStr(1,strIn[strIn.size() - count]);
	if (count == 2) 
	{
		tmpStr.push_back(strIn[strIn.size() - 1]);
	}
	return tmpStr;
}
