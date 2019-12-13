#include "thread.h"
#include "ThreadClass.h"

int A[212][212], B[212][212], C[212][212];
SynOneToOneChannel *channelR[212][212];
SynOneToOneChannel *channelD[212][212];

using namespace std;

int main(int argc, char *argv[])
{
	int l, m, u, v, i, j;
	ifstream myFile("data.in1");

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

	for(i=0; i<=l; i++)
	{	
		for(j=0; j<=v; j++)
		{
			channelR[i][j] = new SynOneToOneChannel("ChannelR", 10, 10);
			channelD[i][j] = new SynOneToOneChannel("ChannelD", 10, 10);
		}

	}

	Rows *rowsA[l];
	Columns *columnsB[v];
	Computer *gridC[l][v];

	for(i=0; i<l; i++)
	{
		rowsA[i] = new Rows(m, i+1);
		rowsA[i]->Begin();
		for(j=0; j<v; j++)
		{
			gridC[i][j] = new Computer(i+1, j+1, l, v);
			gridC[i][j]->Begin();
		}

	}

	for(i=0; i<v; i++)
	{
		columnsB[i] = new Columns(u, i+1);
		columnsB[i]->Begin();
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

	return 0;
}