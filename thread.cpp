#include "thread.h"


Columns::Columns(int lastRow, int c)
		:lastRow(lastRow), c(c)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "ColumnThread: " << c << '\0';

}

void Columns::ThreadFunc()
{
	sprintf(buf, "   Column thread c[%d] started\n", c);
	write(1, buf, strlen(buf));


	Exit();
}


Rows::Rows(int lastColumn, int r)
	:lastColumn(lastColumn), r(r)
{
	ThreadName.seekp(0, ios::beg);
	ThreadName << "RowThread: " << r << '\0';

}

void Rows::ThreadFunc()
{
	sprintf(buf, "Row thread r[%d] started\n", r);
	write(1, buf, strlen(buf));


	Exit();
}

Computer::Computer(int r, int c, int lastRow, int lastColumn)
		:r(r), c(c), lastRow(lastRow), lastColumn(lastColumn)
{	
	ThreadName.seekp(0, ios::beg);
	ThreadName << "Computer[" << r << "][" << c << "]" << '\0';

}

void Computer::ThreadFunc()
{
	int receivedColumn, receivedRow;
	size_t sizeOfInt = sizeof(int);
	sprintf(buf, "      Thread P[%d,%d] started\n", r, c);
	write(1, buf, strlen(buf));

	while(true)
	{
		channelR[r][c]->Receive(&receivedRow, sizeOfInt);
		channelC[r][c]->Receive(&receivedColumn, sizeOfInt);
		
		if(!(receivedRow-receivedColumn))
		{
			




		}
		sprintf(buf, "       Thread P[%d][%d] received %d from above and %d from left\n", r, c, receivedRow, receivedColumn);
		write(1, buf, strlen(buf));


	}

	Exit();
}
