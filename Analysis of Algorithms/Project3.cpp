#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#define SIZE 100
using namespace std;

char readFile();
char writeFile();
char Traverse();

char **arr;
FILE *fp;



int main(void)
{
	arr = new int*[SIZE];

	for(int i = 0; i < SIZE; i++)
	{
		arr[i] = new int[SIZE];
	}

	fp = fopen("input.txt", "r");
	char readFile();


	exit(0);
}

char readFile()
{
	int row = 0, column = 0;
	fp >> row;
	fp >> column;

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			if(fp == ' ' || fp == '-')
				continue;
			else
				fp >> arr[i][j];
		}
	}

}

char writeFile()
{

}

char Traverse(char **array)
{
	char traverseArray[SIZE] = 0;


	while(array[i][j] != "O" && k++)
	{
		//if(HAS VISITED BEFORE)
		//THEN BACKTRACK;

		if(array[i][j] == "N")
		{
			traverseArray[k] = arr[i][j];
			j--;
		}

		if(array[i][j] == "S")
		{
			traverseArray[k] = arr[i][j];
			j++;
		}

		if(array[i][j] == "E")
		{
			traverseArray[k] = arr[i][j];
			i++;
		}

		if(array[i][j] == "W")
		{
			traverseArray[k] = arr[i][j];
			i--;
		}

		if(array[i][j] == "SE")
		{
			traverseArray[k] = arr[i][j];
			j++;
			i++;
		}

		if(array[i][j] == "SW")
		{
			traverseArray[k] = arr[i][j];
			j++;
			i--;

		}

		if(array[i][j] == "NE")
		{
			traverseArray[k] = arr[i][j];
			j--;
			i++;
		}

		if(array[i][j] == "NW")
		{
			traverseArray[k] = arr[i][j];
			j--;
			i--;
		}
	}

		return traverseArray;
}
