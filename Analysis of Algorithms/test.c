#include <iostream>
#include <stdio.h>
#define SIZE 128

using namespace std;

int array[SIZE][SIZE];
int robotPermutation(int x, int y, int z, int total);

int main()
{
	int total = 0;
	robotPermutation(10, 10, 5, total);
	printf(total);
	return 0;
}

int robotPermutation(int x, int y, int z, int total)
{

	if(x == 0)
		return 1;
	if(y == 1)
		return 1;
	if(x < 0)
		return 0;
	if(x > y * z)
		return 0;

	else
	{	
		for(int i = 0; i < z; i++)
		{
			array[y][x] += robotPermutation(i - 1, x, z, total);
		}
	}

	total = array[y][x];
	return total;
}