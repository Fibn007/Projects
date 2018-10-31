#include <iostream>
#include <stdio.h>
#include <cstring>
#define SIZE 30

using namespace std;

int **array;

int robotPermutation(int x, int y, int z);

int main(void)
{
	array = new int*[SIZE];
	for(int i = 0; i < SIZE; i++)
	{
		array[i] = new int[SIZE];
	}

	for(int j = 0; j < SIZE; j++)
	{
		for(int k = 0; k < SIZE; k++)
		{
			array[j][k] = -1;
		}
	}


	int total = robotPermutation(10, 10, 5);
	cout << total;
	delete []array;
	return 0;
}

int robotPermutation(int x, int y, int z)
{
	int tot = 0;
	if(x < 0)
		return 0;
	else if(x > (y*z))
		return 0;
	else if(x == 0)
		return 1;
	else if(y == 1)
		return 1;
	//else if(array[x][y] != -1)
	//	return array[x][y];
	else
	{	
		for(int i = 0; i <= z; i++)
		{
			array[x][y] = robotPermutation(x - i , y - 1, z);
			tot += array[x][y];
		}
	}
	return tot;
}
