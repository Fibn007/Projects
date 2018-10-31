#include <iostream>
#include <iomanip>      // std::setprecision
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <assert.h>
#include <cstring>
#include <cstdlib>
using namespace std;

// Split a string into a set of substrings delimited by delim.
void split(const string& str, vector<string>& tokens, const string& delimiters = " ");

void read_mat(string fname, vector<vector<int> >& mat);


int** mat_mult(char* argv1, char* argv2);
int** mat_add(char* argv1, char* argv2);


/* Print the matrix "mat" in binary to ram_data.bin and in hex to ram_data_hex */
void print_mat(const vector<vector<int> >& mat)
{
    ofstream bin_file, hex_file;
    bin_file.open("ram_data.bin", ios::app);
    hex_file.open("ram_data.hex", ios::app);
    
    unsigned rows = mat.size();
    unsigned cols = mat[0].size();
    for(unsigned i = 0; i < rows; i++)
        for(unsigned j = 0; j < cols; j++) {
            unsigned n = mat[i][j];

            char hex_num[256];
            sprintf(hex_num, "%02x\n", n);
            
            string bin_num("00000000");
            for (unsigned h = 0; h < 8; h++) {
                if (n & 1)
                    bin_num[7-h] = '1';
                else
                    bin_num[7-h] = '0';
                n >>= 1;
            }
            
            bin_file << bin_num << endl;
            hex_file << hex_num;
        }
            
    bin_file.close();
    hex_file.close();
}


int main(int argc, char* argv[])
{
    
   // read input files and store the matrices into A and B
   if (argc != 4) {
   	cout << "Correct format: cmd file1 file2 mat-op" << endl;
   	return 1;
   }
   
   if(strcmp(argv[3], "mult") == 0) 
		mat_mult(argv[1], argv[2]);
	else if(strcmp(argv[3], "add") == 0)
		mat_add(argv[1], argv[2]);
	else {
		cout << "Unrecognized matrix operator " << argv[2] << endl;
   		return 1;
	}
	return 0;
}


int** mat_mult(char* argv1, char* argv2)
{
	unsigned int row_A;
	unsigned int col_A;
	unsigned int row_B = 10;
	unsigned int col_B = 10;
   vector<vector<int> > A, B;
   int** C;
    
    
   read_mat(argv1, A);
   read_mat(argv2, B);
   
   /* Generate external RAM data files for xilinx synthesis and simulation */
    print_mat(A);
    print_mat(B);
    
    
   row_A = A.size();
   col_A = A[0].size();
   row_B = B.size();
   col_B = B[0].size();
   
   if (col_A != row_B)  {
   	cout << "# columns of " << argv1 << " is not equal to # rows of " << argv2 << endl; 
   	return NULL;
   }
    
   // allocate memory for matrix C
   C = new int*[row_A];
   for(int i = 0; i < row_A; i++)
   	C[i] = new int[col_B];
   	
   // Start the matrix multiplcation
   clock_t start, end;
   
   cout<<"\nThe new matrix is:\n";
    start = clock();
    for(int i=0; i < row_A; ++i) {
        for(int j=0; j< col_B; ++j) {
            C[i][j]=0;
            for(int k=0; k< col_A; ++k)
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            //cout << std::hex << (C[i][j] & (0xFFFF)) <<" ";
			cout << C[i][j] <<" ";
        }
        cout<<"\n";
    }
	end = clock();
   cout << setprecision(9);
	cout << "Number of seconds: " << (end-start)/(double)CLOCKS_PER_SEC  << endl;
	
   return C;
}

/**
	Matrix addition function
**/
int** mat_add(char* argv1, char* argv2)
{
	unsigned int row_A;
	unsigned int col_A;
	unsigned int row_B = 10;
	unsigned int col_B = 10;
   vector<vector<int> > A, B;
   int** C;
    
    cout << "start " << endl;
   read_mat(argv1, A); cout << "read " << argv1 << endl;
   read_mat(argv2, B); cout << "read " << argv1 << endl;
   
   /* Generate external RAM data files for xilinx synthesis and simulation */
    print_mat(A);
    print_mat(B);
    
    
   row_A = A.size();
   col_A = A[0].size();
   row_B = B.size();
   col_B = B[0].size();
   
   if (row_A != row_B || col_A != col_B)  {
   	cout << "The size of matrix " << argv1 << " is not equal to the size of matrix " << argv2 << endl; 
   	return NULL;
   }
    
   // allocate memory for matrix C
   C = new int*[row_A];
   for(int i = 0; i < row_A; i++)
   	C[i] = new int[col_A];
   	
   // Start the matrix multiplcation
   clock_t start, end;
   
   cout<<"\nThe new matrix is:\n";
   start = clock();
   for(int i=0; i < row_A; ++i) {
        for(int j=0; j< col_B; ++j) {
			C[i][j] = A[i][j] + B[i][j];
            //cout << std::hex << (C[i][j] & (0xFFFF)) <<" ";
            cout << C[i][j] <<" ";
        }
        cout<<"\n";
    }
	end = clock();
   cout << setprecision(9);
	cout << "Number of seconds: " << (end-start)/(double)CLOCKS_PER_SEC  << endl;
	
   return C;
}


void read_mat(string fname, vector<vector<int> >& A)
{
    ifstream file(fname.c_str()); // pass file name as argment
    string linebuffer;
    int col_size = -1;
    /*
     if (file && getline(file, linebuffer)) {
     assert(linebuffer.length() > 0);
     vector<string> dim;
     split(linebuffer, ' ', dim);
     assert(dim.size() == 2);
     row_A = stoi(dim[0]);
     col_A = stoi(dim[1]);
     cout << "Dimention of Matrix A: " << row_A << "  " << col_A << endl;
     }
     */
    while (file && getline(file, linebuffer)) {
        assert(linebuffer.length());
        vector<string> s_row;
        split(linebuffer, s_row);
        if (col_size == -1)
            col_size = s_row.size();
        else
            assert(s_row.size() == col_size);
        vector<int> row;
        for(int j = 0; j < s_row.size(); j++) {
            row.push_back(atoi(s_row[j].c_str())); // stoi(s_row[j]));
        }
        A.push_back(row);
    }
}


void split(const string& str, vector<string>& tokens, const string& delimiters) {
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    
    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

