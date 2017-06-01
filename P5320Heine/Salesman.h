#ifndef _SALESMAN_H_
#define _SALESMAN_H_
#include<iostream>
#include<fstream>
#include<sstream>
#include<bitset>
#include<vector>
#include<string>
#include"matrix.h"

using namespace std;
// Salesman Class Defines all the functionality required to solve the Traveling Salesman Problem
// using Dynamic Programming
class Salesman{
public:
	void runFromFile();
	void parseWS(ifstream& in, int& numVerts);
	void initAdjMat(int numVerts, matrix<float> &adjMat);
	void initVertsAndEdges(ifstream& in, vector<string>& vertNames, matrix<float>& adjMat);
	void initGTable(matrix<pair<float, vector<int>>> &gTable, matrix<float> adjMat);
	void printMat(matrix<float> currMat);
	void printGTable(matrix<pair<float, vector<int>>> currGTab);
	pair<float, vector<int>> gCost(int i, bitset<32> S, const matrix<float>& adjMat, matrix<pair<float, vector<int>>> &gTable);
	pair<float, vector<int>> populateGTable(const matrix<float>& adjMat, matrix<pair<float, vector<int>>>& gTable);
	void testMin(pair<float, vector<int>> minCostPath, vector<string> vertNames);
};

void Salesman::runFromFile()
{
	ifstream infile;
	string infileName;
	int numVerts = 0;

	cout << "Enter Filename: ";
	cin >> infileName;

	infile.open(infileName, fstream::in);

	if (infile.fail())
	{
		cerr << "ERROR: File Not Found! Exiting Program...";
		exit(1);
	}

	parseWS(infile, numVerts);

	vector<string> vertexNames(numVerts);
	matrix<float> adjMat(numVerts, numVerts);

	initAdjMat(numVerts, adjMat);

	initVertsAndEdges(infile,vertexNames,adjMat);

	matrix<pair<float, vector<int>>> gTable(numVerts, pow(2, numVerts));
	initGTable(gTable, adjMat);
	
	pair<float, vector<int>> minCost;
	minCost = populateGTable(adjMat, gTable);

	testMin(minCost, vertexNames);
}

//Parses through the lines counting them to determine number of vertices in the matrix
void Salesman::parseWS(ifstream& in, int& numVerts)
{
	while (in >> ws && !in.eof())
	{
		string cLine;
		getline(in, cLine);
		if (!cLine.empty())
		{
			numVerts++;
		}
	}

	in.clear();
	in.seekg(0, ios::beg);
}

// Initialize the adjacency matrix with base values
void Salesman::initAdjMat(int numVerts, matrix<float> &adjMat)
{
	for (int i = 0; i < numVerts; i++)
	{
		for (int j = 0; j < numVerts; j++)
		{
			adjMat[i][j] = FLT_MAX;
			if (i == j)
			{
				adjMat[i][j] = 0;
			}
		}
	}
}

// Populate the names vector and the adjacency matrix with the values from the file
void Salesman::initVertsAndEdges(ifstream& in, vector<string>& vertNames, matrix<float>& adjMat)
{
	string currLine;
	string vertName;
	int vertId, edgeId, edgeWeight;

	while (!in.eof())
	{
		getline(in, currLine);
		if (!currLine.empty())
		{
			stringstream strstrm(currLine);
			strstrm >> vertId >> vertName;

			vertNames[vertId - 1] = vertName;
			while (!strstrm.eof())
			{
				strstrm >> edgeId >> edgeWeight;
				adjMat[vertId - 1][edgeId - 1] = edgeWeight;
			}
		}
	}
	in.close();
}

void Salesman::initGTable(matrix<pair<float, vector<int>>> &gTable, matrix<float> adjMat)
{
	for (int i = 0; i < gTable.numrows(); i++)
	{
		for (int j = 0; j < gTable.numcols(); j++)
		{
			if (j == 0)
			{
				gTable[i][j].first = adjMat[i][j];   // Fills the current gTable position with a value in the adjacency matrix
			}
			else
				gTable[i][j].first = -1; // Otherwise initialize it to -1
		}
	}
}

void Salesman::printMat(matrix<float> currMat)
{
	for (int i = 0; i < currMat.numrows(); i++)
	{
		for (int j = 0; j < currMat.numcols(); j++)
		{
			cout << currMat[i][j] << " "; // Prints the value contained at the current position in the Adjacency Matrix 
		}
		cout << endl;
	}
}

void Salesman::printGTable(matrix<pair<float, vector<int>>> currGTab)
{
	for (int i = 0; i < currGTab.numrows(); i++)
	{
		for (int j = 0; j < currGTab.numcols(); j++)
		{
			cout << currGTab[i][j].first << " ";  // Prints the value contained at the current position in the gTable
		}
		cout << endl;
	}
}

pair<float, vector<int>> Salesman::gCost(int i, bitset<32> S, const matrix<float>& adjMat, matrix<pair<float, vector<int>>> &gTable)
{
	pair<float, vector<int>> min(FLT_MAX, vector<int>{});
	if (S.none())
	{
		return pair<float, vector<int>>(adjMat[i][0], vector<int>{}); // Returns to the starting node of the matrix
	}
	if (gTable[i][S.to_ulong()].first >= 0)
		return pair<float, vector<int>>(gTable[i][S.to_ulong()]); // Value already stored in gTable
	else
	{
		for (size_t j = 1; j <= gTable.numrows(); j++) // Check if given node is contained in the set
		{
			if (S.test(i)) // If node is already contained in bitset, no need to compute
			{
				break;
			}
			if (S.test(j)) // If j is contained in the set, calculate path from i to j
			{
				bitset<32> S_sub(S);
				S_sub.reset(j);

				pair<float, vector<int>> tmp = gCost(j, S_sub, adjMat, gTable);
				tmp.first += adjMat[i][j];
				tmp.second.push_back(j);
				if (tmp.first < min.first)
					min = tmp;
			}
		}  
			gTable[i][S.to_ulong()] = min; // Store minimum cost or weight in gTable 
		return min;
	}
}

pair<float, vector<int>> Salesman::populateGTable(const matrix<float>& adjMat, matrix<pair<float, vector<int>>>& gTable)
{
	pair<float, vector<int>> tmpPair(FLT_MAX, vector<int>{});

	for (int j = 1; j < gTable.numcols(); j++)
	{
		for (int i = 0; i < gTable.numrows(); i++)
		{
			bitset<32> S(j);
			tmpPair = gCost(i, S, adjMat, gTable); //Grab the min value from the current gCost operation
		}
	}

	return gTable[0][gTable.numcols() - 2]; // Contains the final minimum weight 
}

void Salesman::testMin(pair<float, vector<int>> minCostPath, vector<string> vertNames)
{
	if (minCostPath.first == FLT_MAX)	// If the final min is equivalent to the max value for a float then we have no path
	{
		cout << "No Hamiltonian Cycle" << endl;
	}
	else
	{
		cout << "Optimal Tour Cost = [" << minCostPath.first << ", <" << vertNames[0] << ","; // Otherwise print out the minimum total cost and the final path.
		for (size_t i = minCostPath.second.size() - 1; i > 0; i--)
		{
			cout << vertNames[minCostPath.second[i]] << ",";
		}
		cout << vertNames[minCostPath.second[0]] << ">]" << endl;
	}
}
#endif