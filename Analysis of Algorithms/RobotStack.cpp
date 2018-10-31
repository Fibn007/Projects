#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <cstring>

using namespace std;

int *readFile(int array[], int size);
void writeFile(int tot);
int robotPermutation(int b, int n, int k, int tot);

int main()
{
	int tot = 0, size = 0;
	int array[100];

	readFile(array, size);
	for(int i = 0; i < 3*size; i+3)
	{
		robotPermutation(array[i], array[i+1], array[i+2], tot);
		writeFile(tot);
	}

	return 0;
}

int *readFile(int number[], int size)
{
	ifstream infile;
	infile.open("input.txt");

	//string pch;

	if(!infile)
	{
		cerr << "Unable to open file." << endl;
		exit(1);
	}

	if(infile.is_open())
	{
		infile >> size;
		char c[2];
		for(int i = 0; i < 3 * size; i++)
		{
			if(i % 3 == 0 || i % 3 == 1)
			{
				infile >> number[i];
				infile >> c[0];
				infile >> c[1];
			}

			else
			{
					infile >> number[i];
			}

			cout << number[i] << endl;
		}

	}

	infile.close();
	return number;
}

void writeFile(int total)
{
	ofstream outfile;
	outfile.open("output.txt");

	outfile << total << endl;
	outfile.close();
}

int robotPermutation(int x, int y, int z, int total)
{
	int array[y][x];

	if(z == 1)
	{
			for(int i = 0; i < y + 1; i++)
			{
				for(int j = 0; j <= std::min(j,i); j++)
				{
					if(j == 0 || j == i)
						array[i][j] = 1;
					else
							array[i][j] = array[i - 1][j] + j * array[i - 1][j - 1];
				}
			}
	}

	else if(z == x)
	{
			for(int i = 0; i < y; i++)
			{
				for(int j = 0; j < x; j++)
				{
					if(j == 0|| j == i)
						array[i][j] = 1;
					else
						//array[i][j] = array[i - 1][j];
				}
			}
	}

	else
	{

	}

	total = array[y][x];
	return total;
}
