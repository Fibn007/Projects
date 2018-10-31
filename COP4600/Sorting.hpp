#include <iostream>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <stdlib.h>

using namespace std;
template <class T> void selectionsort(T* data, int size);
template <class T> void insertionsort(T* data, int size);
template <class T>void quicksort(T* data, int low, int high);

int main(void)
{
    int array[9000];
    int start_s = clock();
    selectionsort(array, 9000);
    int stop_s = clock();
    for(int i = 0; i < 9000; i++)
    {
        cout << array[i] << " ";
    }
    cout << "\nTime is: ";
    if(((stop_s - start_s)/double (CLOCKS_PER_SEC) * 1000) >=  3600000)
        cout << ((stop_s - start_s)/double (CLOCKS_PER_SEC) * 1000) / 3600000 << " hours";
    else if(((stop_s - start_s)/double (CLOCKS_PER_SEC) * 1000) >=  60000)
        cout << ((stop_s - start_s)/double (CLOCKS_PER_SEC) * 1000) / 60000 << " minutes";
    else if(((stop_s - start_s)/double (CLOCKS_PER_SEC) * 1000) >= 1000)
        cout << ((stop_s - start_s)/double (CLOCKS_PER_SEC) * 1000) / 1000 << " seconds";
    else
        cout << (stop_s - start_s)/double (CLOCKS_PER_SEC) * 1000 << " milliseconds" << endl;
        
    return 0;
}





// SelectionSort Function
template <class T> void selectionsort(T* data, int size)
{
	// Checks if size is 1, making this function unneeded
	if(size == 1)
	{
		return;
		cout << "This array is size 1, why are you using this?";
	}
	
	// Sorts
	int largestLocation; // location of the largest value in the array
	int temp;
	int j; // scans through the array for the largest value
	for(int i = 0; i < size; i++)
	{
		largestLocation = i;
		j = i;
		while(j < size)
		{
			if(data[j] < data[largestLocation])
				largestLocation = j;
			j++;
		}
		temp = data[largestLocation];
		data[largestLocation] = data[i];
		data[i] = temp;
	}
}

// InsertSort Function
template <class T> void insertionsort(T* data, int size)
{
	if(size == 1)
	{
		return;
		cout << "This array is size 1, why are you using this?";
	}

	int j;
	int temp; 
	for(int i = 1; i < size; i++)
	{
		j = i;
		while(j >= 0)
		{
			if(data[j - 1] > data[j])
			{
				temp = data[j];
				data[j] = data[j - 1];
				data[j - 1] = temp;
			}
			j--;
		}
	}
}

// QuickSort Function
template <class T>void quicksort(T* data, int low, int high)
{
	int i = low, j = high;
    int temp;
    int pivot = data[(low+high)/2];
 
    while (i <= j) 
    {
        while (data[i] < pivot)
                  i++;
        while (data[j] > pivot)
                  j--;
        if (i <= j) 
        {
            temp = data[i];
            data[i] = data[j];
            data[j] = temp;
            i++;
            j--;
        }
    }
 
    if (low < j)
    {
        quicksort(data, low, j);
    }
    
    if (i < high)
    {
        quicksort(data, i, high);
    }
}

template <class T> bool SortChecker(T* data, int size) //Function to determine if data is sorted or not
{
    
    for (int i = 0; i < size; i++)
    {
      if (data[i - 1] > data[i])
      {
          return false; //Data not sorted
      }      
  }
  return true; //Data sorted
}

template <class T> void ArrayOfZero(T* data, int size) //Function used to declare a data structure filled with values of 0
{
    memset(data, 0, size);
}

template <class T> void SortedArray(T* data, int size)//Function used to declare a data structure filled with values from 0 to size
{
    for(int i = 0; i < size; i++)
    {
       data[i] = i;
    }
}

template <class T> void RandomArray(T* data, int size) //Function used to create a data structure filled with random values from 0 to 99999
{
    for(int i = 0; i < size; i++)
    {
        data[i] = (rand() % 100000);
    }
}
