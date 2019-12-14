// ----------------------------------------------------------- 
// NAME : Hunter Chambers                         User ID: 81276171 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//      Compute the dot product of two matrices
// ----------------------------------------------------------- 
#include "thread.h"
#include "ThreadClass.h"

int A[212][212], B[212][212], C[212][212];
SynOneToOneChannel *channelR[212][212];
SynOneToOneChannel *channelD[212][212];

using namespace std;
// ----------------------------------------------------------- 
// FUNCTION  main                         
//     the driver for the program                            
// PARAMETER USAGE :                                           
//    NONE          
// FUNCTION CALLED :                                           
//    NONE         
// ----------------------------------------------------------- 
int main(int argc, char *argv[])
{
	// For ease, the variables are named the same as in the instructions
	// 
	int l, m, u, v, i, j;
	ifstream myFile("data.in1");
	char buffer[212], buf[212];

	myFile >> l; // number of rows in a
	myFile >> m; // number of columns in a
	for(int i=0; i<l; i++)
	{
		for(int j=0; j<m; j++)
		{
			myFile >> A[i][j];
		}
	}

	myFile >> u; // number of rows in b
	myFile >> v; // number of columns in b
	for(i=0; i<u; i++)
	{
		for(int j=0; j<v; j++)
		{
			myFile >> B[i][j];
		}
	}
	

	int id = 0;

	for(i=0; i<=l; i++)
	{	
		for(j=0; j<=v; j++)
		{
			channelR[i][j] = new SynOneToOneChannel("ChannelR", id, id);
			channelD[i][j] = new SynOneToOneChannel("ChannelD", id, id);
		}

	}

	RowThread *rowsA[l];
	ColumnThread *columnsB[v];
	ComputerThread *gridC[l][v];

	for(i=0; i<l; i++)
	{
		rowsA[i] = new RowThread(m, i+1);
		rowsA[i]->Begin();
	}

	for(i=0; i<v; i++)
	{
		columnsB[i] = new ColumnThread(u, i+1);
		columnsB[i]->Begin();
	}
	for(i=0; i<l; i++)
	{
		for(j=0; j<v; j++)
		{
			gridC[i][j] = new ComputerThread(i+1, j+1, l, v);
			gridC[i][j]->Begin();
		}
	}

	for(i=0; i<l; i++)
	{
		rowsA[i]->Join();
		for(j=0; j<v; j++)
		{
			gridC[i][j]->Join();
		}

	}
	for(i=0; i<v; i++)
	{
		columnsB[i]->Join();
	}

	sprintf(buf, "*** From main***\n");
	write(1, buf, strlen(buf));

	sprintf(buf, "Matrix A: %d rows and %d columns\n", l, m);
	write(1, buf, strlen(buf));
	for(i=0; i<l; i++)
	{
		for(j=0; j<m; j++)
		{
			sprintf(buf, "%d ", A[i][j]);
			write(1, buf, strlen(buf));
		}
		sprintf(buf, "\n");
		write(1, buf, strlen(buf));
	}

	sprintf(buf, "\n");
	write(1, buf, strlen(buf));

	sprintf(buf, "Matrix B: %d rows and %d columns\n", l, m);
	write(1, buf, strlen(buf));
	for(i=0; i<u; i++)
	{
		for(j=0; j<v; j++)
		{
			sprintf(buf, "%d ", B[i][j]);
			write(1, buf, strlen(buf));
		}
		sprintf(buf, "\n");
		write(1, buf, strlen(buf));
	}

	sprintf(buf, "\n");
	write(1, buf, strlen(buf));

	sprintf(buf, "Matrix C = A*B: %d rows and %d columns\n", l, v);
	write(1, buf, strlen(buf));
	for(i=0; i<l; i++)
	{
		for(j=0; j<v; j++)
		{
			sprintf(buf, "%d ", C[i][j]);
			write(1, buf, strlen(buf));
		}
		sprintf(buf, "\n");
		write(1, buf, strlen(buf));
	}



	return 0;
}
