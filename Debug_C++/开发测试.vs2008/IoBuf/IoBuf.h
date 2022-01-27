#pragma once

#ifndef IOBUF_H_
#define IOBUF_H_

#include <windows.h>
#include <iostream>
#include "stdafx.h"

const int IOBUF_SIZE = 4096;

class IoBuf {
private:
	BYTE buffer[IOBUF_SIZE]; // ��������
	PBYTE writePtr; // дָ�롣
	PBYTE readPtr; // ��ָ�롣
	UINT recordCnt; // ��ǰ�������м�¼����
	UINT restCnt; // ��ǰ������δ���ļ�¼����
	FILE* ioFile; // �ļ���

	void writeOutOneDataPage();
	bool readInOneDataPage();

public:
	IoBuf(const char* ioFilePath, const char* ioMode);
	~IoBuf();

	void writeOutInfoRecordInFirstPage(PVOID infoRecord, UINT infoRecordLen);
	void addOneRecord(PVOID record, UINT recordLen);
	PBYTE readInFistPageToGetInfoRecord();
	PBYTE getOneRecord(UINT& recordLen);
};

#endif // !IOBUF_H_

