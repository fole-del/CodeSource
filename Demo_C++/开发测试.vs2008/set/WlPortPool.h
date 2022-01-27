/**************************************************************************
 *
 *        Copyright (C) 2022  Beijing Winicssec Technologies Co.,Ltd
 *        All rights reserved
 *
 *        Filename    :	WLPortPool.h
 *        Description : ��̬�˿���ռ����ϵ�����£���ȡ��̬�˿ڵĺ����ӿ�
 *
 *        Created by  mingming.shi on Jan. 24th, 2022
 *
 *************************************************************************/
#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <WinSock.h>
#include <tcpmib.h>
#include <IPHlpApi.h>
#include <set>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "IPHlpApi.lib")

#define MAX_PORT_NUM		10		// ���ֻ�������10���˿ڳ�
#define DEFAULT_START_PORT	1024	// Ĭ�������˿ڣ�1024
#define DEFAULT_STOP_PORT	65535	// Ĭ�������˿ڣ�65535
#define DEFAULT_PORT_RANGE	100		// Ĭ�϶˿ڷ�Χ��100

/* �˿ڳؽṹ�� */
typedef struct _PORT_POOL {
	UINT nStartPort;	// �����˿�
	UINT nRange;		// �˿ڷ�Χ
	bool bEnabl;

	bool isContain(UINT uPORT)
	{
		if ( uPORT >= nStartPort && uPORT < (nStartPort + nRange) )
		{
			return true;
		}
		return false;
	}

	bool operator<(const _PORT_POOL& b) const
	{
		return this->nStartPort == b.nStartPort ? this->nRange < b.nRange : this->nStartPort < b.nRange;
	}
}STU_PORT_POOL, *PSTU_PORT_POOL;


/* ȫ�ֱ���-�˿����� */
extern std::set<STU_PORT_POOL> g_stuPortPool;


/* ����һ�����ö˿ں��䷶Χ */
void getPort(__out STU_PORT_POOL& stuPort);

/* SetPortIsAvailable */
void SetPortIsAvailable(__in STU_PORT_POOL stuPort);

/* ��ȡʣ�ද̬�˿����� */
bool getFreeDynamicPortsNum(__out int& nNum);