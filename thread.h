#ifndef _THREAD_STUFF
#define _THREAD_STUFF
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "ThreadClass.h"


extern int A[212][212];
extern int B[212][212];
extern int C[212][212];
extern SynOneToOneChannel *channelR[212][212];
extern SynOneToOneChannel *channelD[212][212];


class Columns : public Thread
{
	public:
		Columns(int lastRow, int c);
	private:
		void ThreadFunc();
		int lastRow;
		int c;
		char buf[212];
};

class Rows : public Thread
{
	public:
		Rows(int lastColumn, int r);
	private:
		void ThreadFunc();
		int lastColumn;
		int r;
		char buf[212];
};

class Computer : public Thread
{
	public:
		Computer(int r, int c, int lastRow, int lastColumn);
	private:
		void ThreadFunc();
		int r;
		int c;
		int lastRow;
		int lastColumn;
		char buf[212];
};

#endif
