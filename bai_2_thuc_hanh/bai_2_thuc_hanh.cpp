// bai_2_thuc_hanh.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"

int main()
{
	WIN32_FIND_DATAA DATA;
	HANDLE h = FindFirstFileA("C:\\*.*", &DATA);
	do
	{
		if (DATA.dwFileAttributes &
			FILE_ATTRIBUTE_DIRECTORY) {
			printf("name folder: %s \n",DATA.cFileName);
		}
		else
		{
			INT64 size = (DATA.nFileSizeHigh*(MAXDWORD + 1)) +  DATA.nFileSizeLow;
			printf("name file: %s size: %lld bytes\n", DATA.cFileName, size);
		}
	} while (FindNextFileA(h, &DATA));
	system("PAUSE");
    return 0;
}

