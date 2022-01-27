/**************************************************************************
 *
 *        Copyright (C) 2022  Beijing Winicssec Technologies Co.,Ltd
 *        All rights reserved
 *
 *        Filename    :	WLPortPool.cpp
 *        Description : ��̬�˿���ռ����ϵ�����£���ȡ��̬�˿ڵĺ����ӿ�ʵ���ļ�
 *
 *        Created by  mingming.shi on Jan. 24th, 2022
 *
 *************************************************************************/
#include "StdAfx.h"
#include "WLPortPool.h"


#define STU_MEMBER_NUM		2
#define CMD_RESULT_BUF_SIZE 1024

#define CMD_QUERY_DYNAMIC_TCP "netsh int ipv4 show dynamicport tcp"

std::vector<STU_PORT_POOL> g_stuPortPool;

#if 0 /* �ӿ����� */
#endif
/************************************************************************/
/* ������������ 2022-1-24 14:33:28                                    */
/************************************************************************/
// ��ȡ��̬�˿�
int executeCMD(__out char *pChRet);
int getDynamicPortFromstr(__out STU_PORT_POOL& stuDynamicPort);

// ��ȡ��ǰ��ռ�ö˿�
int getAllTcpConnectionsPort(__out std::set<UINT>& setRet);

// ��ȡʣ�ද̬�˿�
int getFreeDynamicPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeDynamicPort);
int getFreeStaticPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeStaticPort);

// ���¶˿�
void updatePort();

// �����˿ڳ�
int creatPortPool();

#if 0 /* ����ӿڶ��� */
#endif
/*
* @fn			getPort
* @brief		����һ�����ö˿ں��䷶Χ
* @param[in]	
* @param[out]   STU_PORT_POOL: ����һ�����ö˿ڣ��������ʧ�ܣ����޸�stuPort�е�BEnableΪfalse
* @return		
*               
* @detail      	���η��ض˿ڳ��еĶ˿ںͷ�Χ
* @author		mingming.shi
* @date			2022-1-24
*/
void WNT::getPort(__out STU_PORT_POOL& stuPort)
{
	int iRet = -1;
	if ( 0 == g_stuPortPool.size() ) // ��ǰ�˿ڳ�Ϊ��
	{
		iRet = creatPortPool();
	}
	else
	{
		stuPort = *g_stuPortPool.begin();
	}

	if (NO_ERROR == iRet)
	{
		stuPort = *g_stuPortPool.begin();
	}
	else
	{
		stuPort.bEnabl = false;
	}
}

/*
* @fn			SetPortIsAvailable
* @brief		���õ�ǰ�˿ڵĿ���״̬
* @param[in]	STU_PORT_POOL: ��Ҫɾ���Ķ˿�
* @param[out]   
* @return		
*               
* @detail      	����ݵ�ǰ�������˿ںͷ�Χ�ڶ˿ڳ��в���һ�µĲ�ɾ��,Ȼ����������¶˿ڳ�
* @author		mingming.shi
* @date			2022-1-24
*/
void WNT::SetPortIsAvailable(__in STU_PORT_POOL stuPort)
{
	std::vector<STU_PORT_POOL>::iterator it = g_stuPortPool.begin();
	for (; it != g_stuPortPool.end(); it++)
	{
		if (it->nStartPort == stuPort.nStartPort &&
			it->nRange == stuPort.nRange && 
			stuPort.bEnabl == false) // ��ǰ�˿ڲ�����
		{
			g_stuPortPool.erase(it);
		}
	}

	updatePort();
}

/*
* @fn			getFreeDynamicPortsNum
* @brief		��ȡʣ�ද̬�˿�����
* @param[in]    
* @param[out]	nNum��ϵͳ���ж�̬�˿�����
* @return		0 ��ʾ�ɹ���������ʾʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int WNT::getFreeDynamicPortsNum(__out int& nNum)
{
	int iRet = 0;
	int nPortsNum = 0;

	STU_PORT_POOL stuDynamicPort;
	std::set<UINT> CurSysUsedPort;
	std::set<UINT>::iterator it;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	iRet = getDynamicPortFromstr(stuDynamicPort);
	if ( NO_ERROR != iRet ) 
	{
		iRet = -1;
		goto _END_;
	}

	printf("��̬�˿�������ַ��%d\n", stuDynamicPort.nStartPort);
	printf("��̬�˿ڷ�Χ��%d\n", stuDynamicPort.nRange);

	// 2 ��ȡ��ǰϵͳ�˿�ռ�����
	iRet = getAllTcpConnectionsPort(CurSysUsedPort);
	if ( NO_ERROR != iRet ) 
	{
		iRet = -2;
		goto _END_;
	}
	
	// 3 ͳ�ƶ�̬�˿ڸ���
	for (it = CurSysUsedPort.begin(); it != CurSysUsedPort.end(); it++)
	{
		if(true == stuDynamicPort.isContain(*it))
		{
			nPortsNum++;
			printf("%d:%d ",nPortsNum, *it);
			if (nPortsNum % 10 == 0)
			{
				printf("\n");
			}
			
		}
	}
	printf("��ռ��\n��̬�˿�ռ�ø�����%d\n", nPortsNum);

	nNum = stuDynamicPort.nRange - nPortsNum;
	iRet = 0;
_END_:
	if (CurSysUsedPort.size())
	{
		CurSysUsedPort.clear();
	}
	return iRet;
}

#if 0 /* �ӿڶ��� */
#endif
/************************************************************************/
/* �ӿ�ʵ��                                                     */
/************************************************************************/



/*
* @fn			CreatPortPool
* @brief		�����˿���Դ��
* @param[in]    
* @param[out]	
* @return		0 �����ɹ���������ʾʧ��
*               
* @detail      	��ϵͳ��̬�˿�ʹ�����������IEGʱ���ô˽ӿڻ�ȡ��̬�˿���Դ�أ����浽 g_stuPortPool ������
* @author		mingming.shi
* @date			2022-1-24
*/
int creatPortPool()
{
	int iRet = 0;
	std::vector<STU_PORT_POOL> stuPortPool;			// ����ӿڷ��صĶ˿ڷ�Χ
	std::vector<STU_PORT_POOL> stuFreeDynamicPort;	// ����δ��ʹ�ö˿ڷ�Χ
	STU_PORT_POOL	stuDynamicPort;
	std::set<UINT>	CurSysUsedPort;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	iRet = getDynamicPortFromstr(stuDynamicPort);
	if(NO_ERROR != iRet)
	{
		iRet = -1;
		goto _END_;
	}

	// 2 ��ȡ�˿�ռ�����
	iRet = getAllTcpConnectionsPort(CurSysUsedPort);
	if(NO_ERROR != iRet)
	{
		iRet = -2;
		goto _END_;
	}

	// 3 ��Ĭ�ϵ�ַ��ʼ���ҿ��ö˿ڷ�Χ
	iRet = getFreeStaticPort(stuDynamicPort, CurSysUsedPort, stuFreeDynamicPort);
	if(NO_ERROR == iRet)
	{
		g_stuPortPool = stuFreeDynamicPort;
	}

	iRet = 0;
_END_:
	return iRet;
}

/************************************************************************/
/* ����ʵ��  2022-1-24 14:33:41                                        */
/************************************************************************/
/*
* @fn			ExecuteCMD
* @brief		ִ�в�ѯTCP��̬�˿������ȡϵͳ��̬�˿ڷ�Χ
* @param[in]    
* @param[out]	pChRet: ������ִ�к������ַ���
* @return		0 ��ʾ�ɹ������� ʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
static int executeCMD(__out char *pChRet)
{
	int iRet = -1;
	char buf_ps[CMD_RESULT_BUF_SIZE];
	char pChBuf[CMD_RESULT_BUF_SIZE] = { 0 };
	FILE *ptr;

	strcpy_s(pChBuf, _countof(CMD_QUERY_DYNAMIC_TCP) ,CMD_QUERY_DYNAMIC_TCP);

	if ( NULL != (ptr = _popen(pChBuf, "r")) )
	{
		while (NULL != fgets(buf_ps, sizeof(buf_ps), ptr) )
		{
			strcat_s(pChRet, _countof(buf_ps), buf_ps);
			if (strlen(pChRet) > CMD_RESULT_BUF_SIZE)
			{
				iRet = -2;
				break;
			}
		}
		_pclose(ptr);
		ptr = NULL;
		iRet = 0;  // ����ɹ�
	}
	else
	{
	
		iRet = -1; // ����ʧ��
	}

	return iRet;
}

/*
* @fn			getDynamicPortFromstr
* @brief		��cmd���������л�ȡ��̬�˿ڵ�������ַ�ͷ�Χ
* @param[in]    
* @param[out]	stuDynamicPort: ��̬�˿ڵ���ʼ��ַ�ͷ�Χ
* @return		0 ��ʾ�ɹ���������ʾʧ��
*               
* @detail      	�ö�̬�˿�ΪTCP�˿�
* @author		mingming.shi
* @date			2022-1-24
*/
int getDynamicPortFromstr(__out STU_PORT_POOL& stuDynamicPort)
{
	int iRet = 0;
	std::string str = "";
	std::vector<unsigned int> vecNumFromStr;

	// 1 ִ��CMD��ȡ���������ʽ����
	/*
		Э�� tcp ��̬�˿ڷ�Χ
		---------------------------------
		�����˿�        : 49152
		�˿���          : 16384
	*/
	char chRet[CMD_RESULT_BUF_SIZE] = { 0 };
	if ( 0 != executeCMD(chRet) )
	{
		iRet = -1;
		goto _END_;
	}

	str = chRet;

	if(str.empty())
	{
		iRet = -2;
		goto _END_;
	}

	// 2 ��ȡCMD����е�����

	size_t nLen = str.length();

	for (size_t i = 0; i < nLen; i++) {
		int CurNum = 0;
		bool flag = false;
		while ( !(str[i] >= '0' && str[i] <= '9') && i < nLen ) 
		{
			i++;
		}

		while ( (str[i] >= '0' && str[i] <= '9') && i < nLen ) 
		{
			flag = true;
			CurNum = CurNum * 10 + (str[i] - '0');
			i++;
		}

		if (flag)
		{
			vecNumFromStr.push_back(CurNum);
		}
	}
	
	if (STU_MEMBER_NUM != vecNumFromStr.size())
	{
		iRet = -3;
		goto _END_;
	}
	stuDynamicPort.nStartPort	= vecNumFromStr[0];
	stuDynamicPort.nRange		= vecNumFromStr[1];
	
	iRet = 0;
_END_:
	return iRet; //���ϴ�С���ǲ�ͬ��������
}

/*
* @fn			getAllTcpConnectionsPort
* @brief		��ȡ�����Ӷ˿���
* @param[in]    
* @param[out]	ret�������Ӷ˿ڵ�set����
* @return		0 ��ʾִ�гɹ���������ʾʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getAllTcpConnectionsPort(__out std::set<UINT>& setRet)
{
	int iRet                = 0;
	ULONG size              = 0;
	PMIB_TCPTABLE pTcpTable = NULL;
	DWORD dwSize            = 0;
	DWORD dwRetVal			= 0;

	GetTcpTable(NULL, &size, TRUE);

	if ( ERROR_INSUFFICIENT_BUFFER == (dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) ) 
	{
		free(pTcpTable);
		pTcpTable = (MIB_TCPTABLE *)malloc(dwSize);
		if (pTcpTable == NULL) 
		{	
			iRet = -1;
			goto _END_;
		}
	}

	if ( NO_ERROR == (dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) ) 
	{
		for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) 
		{
			setRet.insert(pTcpTable->table[i].dwLocalPort);
		}
	}
	else 
	{
		free(pTcpTable);
		iRet = -2;
		goto _END_;
	}

_END_:
	if (pTcpTable != NULL) 
	{
		free(pTcpTable);
		pTcpTable = NULL;
	}

	return iRet;
}

/*
* @fn			getFreeDynamicPort
* @brief		��ȡδ��ռ�õĶ�̬�˿ڶ�
* @param[in]    stuDynamicPort��ϵͳ��ǰ���ŵĶ�̬�˿ڷ�Χ
				CurSysUsedPort��ϵͳ��ǰ��ʹ�ö˿�
* @param[out]	stuFreeStaticPort: ��Ĭ�϶˿ڿ�ʼ��ȡ�Ķ˿ڷ�Χ

* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getFreeDynamicPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeDynamicPort)
{
	int iRet = 0;
	UINT nStart = stuDynamicPort.nStartPort;
	UINT nEnd	= stuDynamicPort.nStartPort + stuDynamicPort.nRange;

	for (UINT nPort = nStart; nPort != nEnd; ++nPort)
	{
		STU_PORT_POOL stuTempPort;
		if ( CurSysUsedPort.end() !=  CurSysUsedPort.find(nPort) ) // �ҵ���δʹ�ö˿�
		{
			int nRange = 0;

			// ������ǰ�����˿ڵĶ˿ڷ�Χ
			while ( CurSysUsedPort.end() !=  CurSysUsedPort.find( nPort + nRange) )
			{
				nRange++;
			}

			stuTempPort.nStartPort	= nPort;
			stuTempPort.nRange		= nRange;
		}

		stuFreeDynamicPort.push_back(stuTempPort);
		if (stuFreeDynamicPort.size() > MAX_PORT_NUM)
		{
			iRet = -1;
			goto _END_;
		}
	}
	
_END_:
	return iRet;
}

/*
* @fn			getFreeStaticPort
* @brief		��Ĭ�϶˿ڿ�ʼ��ȡδ��ϵͳʹ�õľ�̬�˿ںͷ�Χ
* @param[in]    stuDynamicPort��ϵͳ��ǰ���ŵĶ�̬�˿ڷ�Χ
				CurSysUsedPort��ϵͳ��ǰ��ʹ�ö˿�
* @param[out]	stuFreeStaticPort: ����
* @return		0 ��ʾ�ɹ���������ʾʧ��
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getFreeStaticPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeStaticPort)
{
	int iRet = -1;
	UINT nStart = DEFAULT_START_PORT;
	UINT nEnd	= DEFAULT_STOP_PORT;

	for (UINT nPort = nStart; nPort != nEnd; ++nPort)
	{
		STU_PORT_POOL stuTempPort;

		if ( CurSysUsedPort.end() == CurSysUsedPort.find(nPort) && false == stuDynamicPort.isContain(nPort) ) // �ҵ���δʹ�ö˿ڣ����Ҳ��ٶ�̬�˿ڷ�Χ��
		{
			int nRange = 0;

			// ������ǰ�����˿ڵĶ˿ڷ�Χ
			do
			{
				nRange++;
			}while ( 
				CurSysUsedPort.end() ==  CurSysUsedPort.find(nPort + nRange) && // ϵͳδʹ��
				false == stuDynamicPort.isContain(nPort + nRange) && // ���ڶ�̬�˿ڷ�Χ
				nRange <= DEFAULT_PORT_RANGE); // ��ΧС��Ĭ�϶˿ڷ�Χ

			stuTempPort.nStartPort	= nPort;
			stuTempPort.nRange		= nRange;
			stuTempPort.bEnabl		= true;

			// ���������˿ڲ����Ϸ���
			nPort += nRange; 
			nPort = nPort >= DEFAULT_STOP_PORT ? DEFAULT_START_PORT : nPort;
		}

		stuFreeStaticPort.push_back(stuTempPort);
		if (stuFreeStaticPort.size() > MAX_PORT_NUM) // ֻ���������˶��ѣ���Ȼ���سɹ�
		{
			iRet = 0; 
			goto _END_;
		}
	}

	iRet = 0;
_END_:
	return iRet;
}

void updatePort()
{
	int iRet = -1;
	UINT nStart = DEFAULT_START_PORT;
	UINT nEnd	= DEFAULT_STOP_PORT;
	STU_PORT_POOL	stuDynamicPort;
	std::set<UINT>	CurSysUsedPort;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	getDynamicPortFromstr(stuDynamicPort);

	// 2 ��ȡ�˿�ռ�����
	getAllTcpConnectionsPort(CurSysUsedPort);

	// 3 ���������˿ڣ��Ӷ˿ڳ����һ���˿ڿ�ʼ
	STU_PORT_POOL stuTempPort = g_stuPortPool[g_stuPortPool.size() - 1];
	nStart = stuTempPort.nStartPort + stuTempPort.nRange;

	for (UINT nPort = nStart; nPort <= DEFAULT_STOP_PORT; ++nPort)
	{
		STU_PORT_POOL stuTempPort;

		if ( CurSysUsedPort.end() != CurSysUsedPort.find(nPort) && false == stuDynamicPort.isContain(nPort) ) // �ҵ���δʹ�ö˿ڣ����Ҳ��ٶ�̬�˿ڷ�Χ��
		{
			int nRange = 0;

			// ������ǰ�����˿ڵĶ˿ڷ�Χ
			while ( CurSysUsedPort.end() !=  CurSysUsedPort.find(nPort + nRange) && false == stuDynamicPort.isContain(nPort + nRange) )
			{
				nRange++;
				if (nRange >= DEFAULT_PORT_RANGE)
				{
					break;
				}
			}

		
			stuTempPort.nStartPort	= nPort;
			stuTempPort.nRange		= nRange;
			stuTempPort.bEnabl		= true;

			// ���������˿ڲ����Ϸ���
			nPort += nRange; 
			nPort = nPort >= DEFAULT_STOP_PORT ? DEFAULT_START_PORT : nPort;
		}

		if (true == stuTempPort.isLegal())
		{
			g_stuPortPool.push_back(stuTempPort);
		}
		
		if (g_stuPortPool.size() > MAX_PORT_NUM)
		{
			break;
		}
	}
}
