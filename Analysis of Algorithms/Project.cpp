//Fabian Demiro, Brian Butner, Drew Rooks
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <cstdlib>

#define NORTH 1 //LSB
#define EAST  2
#define SOUTH 4
#define WEST  8 //MSB

#define BLUE 0
#define RED  1

#define MAX_CHILDREN 100


using namespace std;


/*
 *  When reading in the text file, parse everything so that you can use a simple
 *  recursion algorithm(s) through your already known children
 */
typedef struct graph_node {
    int color; //0 - R, 1 - B

    int row, col; //node location in 2D array

    int direction; //bit-wise OR with the cardinal directions to get the

    int visited; //flag for each node to see if you've already visited

    int isGoal; //holds the value of "O"

    struct graph_node* parent; //TO BE POPULATED AS YOU SEARCH! NOT IN PARSING

    // struct graph_node* [MAX_CHILDREN] children; //every possible child before running out of bounds
    //std::vector <struct graph_node*> children; //??????????


    //If every child is known before the parsing, then you don't have to worry about
    //going 'out of bounds'
} GraphNode;

std::vector<GraphNode> _current_path;
//std::vector< std::vector<GraphNode> > _available_nodes; //not necessary if everything's parsed into linked lists

GraphNode *start; //to be filled in during parsing
GraphNode *goal;

std::vector <std::vector <GraphNode> > globalGraph;

void readFile();
void writeFile();
void traverseGraph(GraphNode* cur_node, GraphNode* parent);
void getFinalPath(GraphNode* goal);

void readFile()
{
  int maxRow, maxColumn;
  string temp;

  ifstream infile;
  infile.open("small.txt");

  infile >> maxRow;
  infile >> maxColumn;

  globalGraph.resize(maxRow);

  cout << maxRow << ", " << maxColumn << endl;

  for(int i = 0; i < maxRow; i++)
  {
      globalGraph[i].resize(maxColumn);

      for(int j = 0; j < maxColumn; j++)
      {
            GraphNode tempNode;
            infile >> temp;
             //Reading in graph node

            if(temp.at(0) == 'R')
            {
              globalGraph[i][j].color = 0;
              globalGraph[i][j].visited = 0;
            }

            else if(temp.at(0) == 'B')
            {
              globalGraph[i][j].color = 1;
              globalGraph[i][j].visited = 0;
            }



            if(temp.find("N") != std::string::npos)
            {
              globalGraph[i][j].direction |= 1;
            }

            if(temp.find("S") != std::string::npos)
            {
              globalGraph[i][j].direction |= 4;
            }

            if(temp.find("E") != std::string::npos)
            {
              globalGraph[i][j].direction |= 2;
            }

            if(temp.find("W") != std::string::npos)
            {
              globalGraph[i][j].direction |= 8;
            }

            if(i == start->row && j == start->col)
            {
              start->color = globalGraph[i][j].color;
              start->direction = globalGraph[i][j].direction;
            }

            globalGraph[i][j].row = i;
            globalGraph[i][j].col = j;
            globalGraph[i][j].isGoal = 0;

            //cout << globalGraph[i][j].row << ", " << globalGraph[i][j].col << "\t";
            //cout << globalGraph[i][j].color << "\t";
            //cout << globalGraph[i][j].direction << endl;
        }

        //cout << endl;
      }

      goal->row = maxRow;
      goal->col = maxColumn;
      goal->isGoal = 1;
}

void writeFile()
{
  //ofstream outfile;
  //outfile << GraphNode.direction << " ";
}


void traverseGraph(GraphNode* cur_node, GraphNode* parent)
{
  int nextRow = cur_node->row, nextColumn = cur_node->col;
    //multiple nodes can have goal as a child, so check with the position

    if((cur_node->isGoal == 1) || (cur_node->row == goal->row && cur_node->col == goal->col))
        return;

    if(parent->color != cur_node->color && cur_node->visited != 1)
    {
      cout << "Moving on up in life fam" << endl;
      cur_node->parent = parent; //Here is where we update parent node
      cur_node->visited = 1;
      parent = cur_node;
    }

    else if(parent->color != cur_node->color && cur_node->visited == 1)
    {
        cout << "Already visited, going back to parent" << endl;
        //parent = parent->parent;
        //cur_node = parent;

        //nextRow = cur_node->row; //These two will hold the value of the parent in case the graph needs to backtrack
        //nextColumn = cur_node->col;

    }

    if(cur_node != NULL)
    {
        if(parent->direction & NORTH)
            nextRow--;

        else if(parent->direction & SOUTH)
            nextRow++;

        if(parent->direction & EAST)
            nextColumn++;

        else if(parent->direction & WEST)
            nextColumn--;
    }

    if(nextRow < start->row || nextColumn < start->col || nextRow > goal->row || nextColumn > goal->col)
    {
        cout << "Out of bounds\n";
        parent = parent->parent;
        cur_node = parent;

        nextRow = cur_node->row; //These two will hold the value of the parent in case the graph needs to backtrack
        nextColumn = cur_node->col;
    }

    cout << cur_node->row << ", " << cur_node->col << "\t";
    cout << cur_node->color << "\t";
    cout << cur_node->direction << endl;

    traverseGraph(&globalGraph[nextRow][nextColumn], parent);
}

void getFinalPath(GraphNode* goal)
{
    while(goal->row != start->row && goal->col != start->col)
    {
        //insert the goal into the current path
        //use iterators?
        _current_path.insert(_current_path.begin(), *goal);

        //OR DO YOUR FPRINTF HERE? i.e. 2NE 3S 5E ... etc

        //now get the parent
        goal = goal->parent;
    }

    //the above loop should have gotten everything except the start node.
    // add it.
    _current_path.insert(_current_path.begin(), *start);


    if(_current_path.size() < 2)
    {
        printf("Something's not right! \n");
    }

    else
    {
        printf("Final Path has %d nodes\n", _current_path.size());
    }
}

int main(void)
{

    start = (GraphNode*) malloc(sizeof(GraphNode));
    goal = (GraphNode*) malloc(sizeof(GraphNode));

    start->row = 0;
    start->col = 0;
    start->visited = 1;
    start->isGoal = 0;

    readFile();
    GraphNode cur_node = *start;

    //returns true when path is found...
    //cur_node is also set to the goal node...
    traverseGraph(&cur_node, &cur_node);
    getFinalPath(&cur_node); //... trace your way from goal to start through parents

    //int writeFile();
    delete start;
    delete goal;

    return 0;
}
