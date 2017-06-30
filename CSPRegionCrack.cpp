// CSPRegionCrack.cpp : Entry Point
//

#include <string>
#include <iostream>
#include <fstream>

#define BYTE unsigned char

using namespace std;

const BYTE checkBytes[] = { 0x75, 0x11, 0x3B, 0xF3, 0x74, 0x04, 0x3B, 0xC3, 0x75 };

const BYTE replaceBytes[] = { 0xEB, 0x11, 0x3B, 0xF3, 0x74, 0x04, 0x3B, 0xC3, 0x75 };

const BYTE checkBytes32[] = { 0x75, 0x1B, 0x3B, 0xFE, 0x74, 0x04, 0x3B, 0xC6, 0x75 };

const BYTE replaceBytes32[] = { 0xEB, 0x1B, 0x3B, 0xFE, 0x74, 0x04, 0x3B, 0xC6, 0x75 };

int GetIndexOfSubArray(const BYTE* mainArr, int mainArrSize,const BYTE* subArr, int subArrSize)
{
	for (int i = 0; i < mainArrSize - (subArrSize - 1); ++i)
	{
		if (mainArr[i] == subArr[0])
		{
			bool matches = true;
			for (unsigned int j = 1; j < subArrSize; ++j)
			{
				if (mainArr[i + j] != subArr[j])
				{
					matches = false;
					break;
				}
			}
			if (matches)
			{
				return i;
			}
		}
	}
	return -1;
}


void ReplaceSubArray(BYTE* mainArr, int startIndex, const BYTE* subArr, int subArrSize)
{
	for (int i = 0; i < subArrSize; ++i)
	{
		mainArr[startIndex + i] = subArr[i];
	}		
}

/*
Usage: 
64bit no para
32bit -32
*/
int main() {
	ifstream ifs("CLIPStudioPaint.exe", ios::binary);

	if (!ifs) {
		wcout << "Cannot find file -- CLIPStudioPaint.exe\nPress Any key to exit..." << endl;
		getchar();
		return -1;
	}

	// get length of file:
	ifs.seekg(0, ifs.end);
	int length = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	wcout << "Reading file... " << length << " bytes in total" << endl;

	BYTE* buffer = new BYTE[length];

	ifs.read((char *)buffer, length);

	if (ifs)
	{
		wcout << "Read success!" << endl;
	}
	else
	{
		wcout << "Error: Only " << ifs.gcount() << " bytes of data has been read." << endl;
		length = ifs.gcount();
	}
	ifs.close();

	int matchIndex = GetIndexOfSubArray(buffer, length, checkBytes, sizeof(checkBytes) / sizeof(BYTE));

	if (matchIndex != -1)
	{
		//backup
		ofstream bak_ofs("CLIPStudioPaint.exe.bak", ios::binary | ios::trunc);
		bak_ofs.write((char *)buffer, length);
		bak_ofs.close();
		wcout << "Cracking..." << endl;
		ReplaceSubArray(buffer, matchIndex, replaceBytes, sizeof(replaceBytes) / sizeof(BYTE));
		wcout << "Replace success..." << endl;
		ofstream ofs("CLIPStudioPaint.exe", ios::binary | ios::trunc);
		ofs.write((char *)buffer, length);
		ofs.close();
		wcout << "Crack success!" << endl;
	}
	else
	{
		wcout << "Cannot find matched data... Check if the file has already been cracked..." << endl;
		int matchCrackedIndex = GetIndexOfSubArray(buffer, length, replaceBytes, sizeof(replaceBytes) / sizeof(BYTE));
		if (matchCrackedIndex > 0)
		{
			wcout << "The file has already been cracked!" << endl;
		}
		else
		{
			wcout << "Cannot find cracked data block...Crack failed..." << endl;
		}
	}

	delete buffer;


	wcout << "按任意键退出..." << endl;
	getchar();
	return 0;
}
