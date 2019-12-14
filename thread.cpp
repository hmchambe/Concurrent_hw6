// ----------------------------------------------------------- 
// NAME : Hunter Chambers                         User ID: 81276171 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//      Compute the dot product of two matrices
// ----------------------------------------------------------- 
#include "thread.h"
#include "ThreadClass.h"

// ----------------------------------------------------------- 
// FUNCTION  ColumnThread                         
//     constructor                            
// PARAMETER USAGE :                                           
//    lastRow -> upperbound for when to terminate loops
//    c -> column index  
// FUNCTION CALLED :                                           
//    NONE         
// ----------------------------------------------------------- 
ColumnThread::ColumnThread(int lastRow, int c)
		:lastRow(lastRow), c(c)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "ColumnThread: " << c << '\0';
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc                         
//     main function for ColumnThread
// PARAMETER USAGE :                                           
//    NONE          
// FUNCTION CALLED :                                           
//    NONE         
// ----------------------------------------------------------- 
void ColumnThread::ThreadFunc()
{
	int informationToSend = -1;
	sprintf(buf, "   Column thread c[%d] started\n", c);
	write(1, buf, strlen(buf));

	for(int i = 0; i<lastRow; i++)
	{
		informationToSend = B[i][c-1];
		channelD[1][c]->Send(&informationToSend, sizeof(int));
		sprintf(buf, "    Column thread [%d] sent %d to P[%d][%d]\n", c, informationToSend, 1, c);
		write(1, buf, strlen(buf));
		informationToSend = -1;
	}

	channelD[1][c]->Send(&informationToSend, sizeof(int));
	sprintf(buf, "    Column thread c[%d] sent EOD to P[%d][%d] and terminated\n", c, c, 1);
	write(1, buf, strlen(buf));

	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  RowThread                         
//     constructor                            
// PARAMETER USAGE :                                           
//    lastColumn -> upperbound for when to terminate loops
//    r -> row index  
// FUNCTION CALLED :                                           
//    NONE         
// ----------------------------------------------------------- 
RowThread::RowThread(int lastColumn, int r)
	:lastColumn(lastColumn), r(r)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "RowThread: " << r << '\0';

}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc                         
//     main function for RowThread
// PARAMETER USAGE :                                           
//    NONE          
// FUNCTION CALLED :                                           
//    NONE         
// ----------------------------------------------------------- 
void RowThread::ThreadFunc()
{
	int informationToSend = -1;
	sprintf(buf, "Row thread r[%d] started\n", r);
	write(1, buf, strlen(buf));
	
	for(int i = 0; i<lastColumn; i++)
	{
		informationToSend = A[r-1][i];
		channelR[r][1]->Send(&informationToSend, sizeof(int));
		sprintf(buf, "Row thread r[%d] sent %d to P[%d][%d]\n", r, informationToSend, r, 1);
		write(1, buf, strlen(buf));
		informationToSend = -1;
	}

	channelR[r][1]->Send(&informationToSend, sizeof(int));
	sprintf(buf, "Row thread r[%d] sent EOD to P[%d][%d] and terminated\n", r, r, 1);
	write(1, buf, strlen(buf));

	Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  ComputerThread                         
//     constructor                            
// PARAMETER USAGE : 
//    lastRow -> upperbound index used for terminating loops                                          
//    lastColumn -> upperbound for when to terminate loops
//    r -> row index
//    c -> column index  
// FUNCTION CALLED :                                           
//    NONE         
// ----------------------------------------------------------- 
ComputerThread::ComputerThread(int r, int c, int lastRow, int lastColumn)
		:r(r), c(c), lastRow(lastRow), lastColumn(lastColumn)
{	
	ThreadName.seekp(0, ios::beg);
	ThreadName << "Computer[" << r << "][" << c << "]" << '\0';

}


// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc                         
//     main function for ComputerThread
// PARAMETER USAGE :                                           
//    NONE          
// FUNCTION CALLED :                                           
//    NONE         
// ----------------------------------------------------------- 
void ComputerThread::ThreadFunc()
{
	int receivedColumn = 33, receivedRow = 33, dotProduct = 0;
	size_t sizeOfInt = sizeof(int);
	// Indicator for if EOD was sent from another thread
	bool receivedEOD = false;

	sprintf(buf, "      Thread P[%d,%d] started\n", r, c);
	write(1, buf, strlen(buf));

	while(true)
	{
		channelR[r][c]->Receive(&receivedRow, sizeof(int));
		channelD[r][c]->Receive(&receivedColumn, sizeof(int));
		receivedEOD = ((receivedRow == -1) && (receivedColumn == -1));

		if(!receivedEOD)
		{
			sprintf(buf, "       Thread P[%d][%d] received %d from above and %d from left\n", r, c, receivedRow, receivedColumn);
			write(1, buf, strlen(buf));
			if(r < lastRow)
			{
				channelD[r+1][c]->Send(&receivedColumn, sizeOfInt);
				sprintf(buf, "       Thread P[%d][%d] sent %d down to P[%d][%d]\n", r, c, receivedColumn, r+1, c);
				write(1, buf, strlen(buf));
			}
			if(c < lastColumn)
			{
				channelR[r][c+1]->Send(&receivedRow, sizeOfInt);
				sprintf(buf, "       Thread P[%d][%d] sent %d right to P[%d][%d]\n", r, c, receivedRow, r, c+1);
				write(1, buf, strlen(buf));
			}

			dotProduct = dotProduct + (receivedRow*receivedColumn);

			if(!receivedRow)
				Exit();
		}else
		{


			// Sends info if not at the end of the row
			if(r < lastRow)
				channelD[r+1][c]->Send(&receivedColumn, sizeOfInt);
			// Sends info if not at the end of the column
			if(c < lastColumn)
				channelR[r][c+1]->Send(&receivedRow, sizeOfInt);
			// Saves value into C
			C[r-1][c-1] = dotProduct;  
			
			sprintf(buf, "       Thread P[%d][%d] received EOD, saved result %d and terminated\n", r, c, dotProduct);
			write(1, buf, strlen(buf));
	
			Exit();
		}
		
	}

	Exit();
}
