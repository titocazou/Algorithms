#include <cmath>
#include <cstdio>

#include <algorithm>
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include <string>
using namespace std;

void FindSellingDays(vector<bool>& sellDays, const int& testSize, const vector<int>& testData)
{
	int LocalMax=-100;
	for (int j=0; j < testSize; ++j)
	{
		sellDays.push_back(false);
	}

	for (int i= testSize-1; i>=0;--i)
	{
		if (testData[i] > LocalMax)
		{
			LocalMax=testData[i];
			sellDays[i]=true;
		}
	}

}

void ComputeCost(vector<bool>& sellDays, const int& testSize, const vector<int>& testData)
{
	int totalProfit=0;
	int numStocks=0, currentCost=0;

	for (int i=0; i<testSize; ++i)
	{
		if (!sellDays[i])
		{
			++numStocks;
			currentCost += testData[i];
		}
		else
		{
			totalProfit+=numStocks* testData[i]-currentCost;
			numStocks=0;
			currentCost=0;
		}
	}

	cout << totalProfit << "\n";
}
void SolveCase(int testSize, vector<int> testData)
{
	vector<bool> sellDays;
	FindSellingDays (sellDays,testSize,testData);
	ComputeCost (sellDays,testSize,testData);
}
vector<int> ParseDataString(const string& dataString)
{
	vector<int> data;
	string temp="";
	for (int j =0; j<dataString.length();++j)
	{
		if (dataString[j] != ' ')
		{
			temp+=dataString[j];
		}
		else
		{
			data.push_back(atoi(temp.c_str()));
			temp="";
		}
	}
	if (temp != "") {data.push_back(atoi(temp.c_str()));} 
	return data;
}
void SolveAll(const vector<int>& testSizeSet,const vector<string>& dataSetString)
{
	for (int i=0; i < testSizeSet.size(); ++i )
	{
		vector<int> testData= ParseDataString(dataSetString[i]);
		SolveCase(testSizeSet[i],testData);
	}
}
void Initialize(vector<int>& testSizeSet,vector<string>& dataSetString)
{
	int numberTest;
	int testSize;
	string dataString;
	cin >> numberTest;

	for (int i=0; i<numberTest; ++i)
	{
		cin >> testSize;
		testSizeSet.push_back(testSize);
		cin.ignore();
		std::getline(std::cin,dataString);
		dataSetString.push_back(dataString);
	}
}

int main() {
	/* Enter your code here. Read input from STDIN. Print output to STDOUT */   

	vector <string> dataSetString;
	vector <int> testSizeSet;

	Initialize(testSizeSet,dataSetString);

	SolveAll(testSizeSet,dataSetString);

	return 0;
}


