// ----------------------------------------------------------- 
// NAME : Hunter Chambers                         User ID: 81276171 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//      Compute the dot product of two matrices
// ----------------------------------------------------------- 
#ifndef _THREAD_STUFF
#define _THREAD_STUFF
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "ThreadClass.h"
#include "thread.h"


extern int A[212][212];
extern int B[212][212];
extern int C[212][212];
extern SynOneToOneChannel *channelR[212][212];
extern SynOneToOneChannel *channelD[212][212];


class ColumnThread : public Thread
{
	public:
		ColumnThread(int lastRow, int c);
	private:
		void ThreadFunc();
		int lastRow;
		int c;
		char buf[212];
};

class RowThread : public Thread
{
	public:
		RowThread(int lastColumn, int r);
	private:
		void ThreadFunc();
		int lastColumn;
		int r;
		char buf[212];
};

class ComputerThread : public Thread
{
	public:
		ComputerThread(int r, int c, int lastRow, int lastColumn);
	private:
		void ThreadFunc();
		int r;
		int c;
		int lastRow;
		int lastColumn;
		char buf[212];
};

#endif
