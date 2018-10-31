/*Purpose of this code is to traverse through an N x N matrix that
is color coded and has a direction to find the Bullseye using a Depth 
First search. The Bullseye is marked by 'O' and the nodes have directions
N, S, E, W with a color. The color must alternate for you to land 
on it. I.e., if you are on N-red, you must skip every red in the 
north direction before you can land on a blue node. You may skip that 
blue node if that leads to loop onto another blue as long as there
are more blues to go to.*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <stack>

using namespace std;
std::stack<std::string> answer; 

struct component
{
  char color;  // R = Red, B = Blue
  string direction;  // N = North, NW = NorthWest, etc.
  bool visited;  // False if not visited, true if visited
};

bool DFS(int x, int y, component** listOfComponents, int numRows, int numColumns);
void removeMovements(int movements);
component getComponent(int col, int row, ifstream &infile, int origin);
void printThis(component **myCom, int a, int b);

main(int argc, char *argv[])
{
  int ROW, COLUMN;
  ifstream inFile;
  inFile.open(argv[1]);
  
  if (!inFile) {
      cerr << "Unable to open file "<< argv[1];
      return(1);   // call system to stop
      } 
      else { cout <<"opened " << argv[1] << " file successfully\n";}
      
  ofstream outFile;
  outFile.open (argv[2]);
  if (!outFile) {
      cerr << "Unable to open file "<< argv[2];
      return(1);   // call system to stop
      } 
      else { cout <<"opened "<< argv[2] <<" file successfully\n";}
  
  inFile >> ROW >> COLUMN;// set row and column
  inFile.ignore(256, '\n'); //eat up rest of line
  int origin = inFile.tellg();//setting up origin
  
  // initializes listOfComponents
  component **listOfComponents = new component*[COLUMN]; 
  
  for(int i = 0; i < COLUMN; i++)  
    listOfComponents[i] = new component[ROW];
  
  // NOW INITALIZE THE ARAY
  for(int i =0; i < COLUMN; i++)
  {
    for(int j=0; j < ROW; j++)
    {
      if(COLUMN-1 == i && ROW-1 == j)
        break;
      listOfComponents[i][j] = getComponent(i, j, inFile, origin);
    }
  }
  inFile.close();
  
  listOfComponents[COLUMN-1][ROW-1].color = 'G';
  listOfComponents[COLUMN-1][ROW-1].direction = "BullseyeDirection";
  listOfComponents[COLUMN-1][ROW-1].visited = false;
  

  listOfComponents[0][0].visited = true;
  
  bool doesWork = DFS(0, 0, listOfComponents, ROW, COLUMN);

  if(doesWork != true){
    cout << "\nERROR: BULLSEYE COULD NOT BE FOUND";
  }
  else
  {
    std::stack<std::string> realAnswer;
  
    while(answer.empty() == false)
    {
      realAnswer.push(answer.top()); 
      answer.pop();
    }
  
    cout << "\n";
    string prevDirection = realAnswer.top();
    int n = 0;
    while(realAnswer.empty() == false)
    {
      if (realAnswer.top().compare(prevDirection) == 0)
      { 
        n++;
        realAnswer.pop();
      }
      else
      {
        cout << n << prevDirection <<" ";
        outFile << n << prevDirection <<" ";
        prevDirection = realAnswer.top();
        n=1;
        realAnswer.pop();
      }
    }
  cout << n << prevDirection <<" ";
  outFile << n << prevDirection <<" ";
  }

  outFile.close();
  return 0;
}
  


//fetches one component given x and y, returns the color direction, 
component getComponent(int col, int row, ifstream &inFile, int origin)
{
  component newStruct;
  newStruct.color = 'G';
  newStruct.direction = 9;
  newStruct.visited = false;
  
  string dummyLine, storage;

  int j;
  inFile.seekg(origin); // return to origin position

  // get to beginning of entry
  for(j=0; j<row; j++) //skips to row we want
	{getline(inFile, dummyLine);}
  for(j=0; j<col; j++) // skips to the word we want
	{inFile >> dummyLine;}
  
  //simple method
  inFile >> storage;
  newStruct.color = storage[0]; 
  newStruct.direction = storage.substr(2);
  

	return newStruct;
}

bool DFS(int x, int y, component** listOfComponents, int numRows, int numColumns)
{
  // does set values
  int movementsUsed = 0;
  char setColor = listOfComponents[x][y].color;
  std::string setDirection = listOfComponents[x][y].direction;
 
  while(x < numColumns && y < numRows && x >=0 && y >= 0)
  {
    if(x == numColumns-1 && y == numRows-1)
    {
      return true;
    }
    
    answer.push(setDirection);
    movementsUsed++;
    
    if (setDirection.compare("N") == 0)
      {y--;}
    else if (setDirection.compare("S") == 0)
      {y++;}
    else if (setDirection.compare("E") == 0)
      {x++;}
    else if (setDirection.compare("W") == 0)
      {x--;}
    else if (setDirection.compare("NW") == 0)
      {x--; y--;}
    else if (setDirection.compare("NE") == 0)
      {x++; y--;}
    else if (setDirection.compare("SW") == 0)
      {x--; y++;}
    else if (setDirection.compare("SE") == 0)
      {x++; y++;}
      
    if(!(x < numColumns && y < numRows && x >=0 && y >= 0))
      break;
    
    if(listOfComponents[x][y].color != setColor)
    {
      if(listOfComponents[x][y].visited != true)
      {
        listOfComponents[x][y].visited = true;
        if(DFS(x, y, listOfComponents, numRows, numColumns) == true)
        {
          return true;
        }
      }
    }
  }
  removeMovements(movementsUsed);
  return false;
}

// Removes movements used in this level of recurrence
void removeMovements(int movements)
{
  for(int i = 1; i <= movements; i++)
    answer.pop();
}

void printThis(component **myCom, int a, int b)
{
  for(int i = 0; i < a; i++)
  {
    for(int j = 0; j < b; j++)
      cout << "\nChecking component" << i << "x" << j << ": " << myCom[i][j].color << "|" << myCom[i][j].direction << "|"  << myCom[i][j].visited;
  }
}