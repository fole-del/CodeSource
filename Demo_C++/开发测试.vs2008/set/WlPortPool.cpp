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

std::set<STU_PORT_POOL> g_stuPortPool;

#if 0 /* �ӿ����� */
#endif
/************************************************************************/
/* ������������ 2022-1-24 14:33:28                                    */
/************************************************************************/
// ��ȡ��̬�˿�
int ExecuteCMD(__out char *pChRet);
int getDynamicPortFromstr(__out STU_PORT_POOL& stuDynamicPort);

// ��ȡ��ǰ��ռ�ö˿�
std::set<int> GetAllTcpConnectionsPort();

// ��ȡʣ�ද̬�˿�
int getFreeDynamicPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<UINT> CurSysUsedPort, __out std::vector<STU_PORT_POOL>& stuFreeDynamicPort);
int getFreeStaticPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<int> CurSysUsedPort, __out std::set<STU_PORT_POOL>& stuFreeStaticPort);

// ���¶˿�
void updatePort();

// �����˿ڳ�
int CreatPortPool();

#if 0 /* ����ӿڶ��� */
#endif
/*
* @fn			getPort
* @brief		����һ�����ö˿ں��䷶Χ
* @param[in]	
* @param[out]   STU_PORT_POOL: ����һ���˿�
* @return		
*               
* @detail      	���η��ض˿ڳ��еĶ˿ںͷ�Χ
* @author		mingming.shi
* @date			2022-1-24
*/
void getPort(__out STU_PORT_POOL& stuPort)
{
	if ( ! g_stuPortPool.size() ) // ��ǰ�˿ڳ�Ϊ��
	{
		CreatPortPool();
	}

	stuPort = *g_stuPortPool.begin();
}

/*
* @fn			SetPortIsAvailable
* @brief		���õ�ǰ�˿ڵĿ���״̬
* @param[in]	STU_PORT_POOL: ��Ҫɾ���Ķ˿�
* @param[out]   
* @return		
*               
* @detail      	����ݵ�ǰ�������˿ںͷ�Χ�ڶ˿ڳ��в���һ�µĲ�ɾ��
* @author		mingming.shi
* @date			2022-1-24
*/
void SetPortIsAvailable(__in STU_PORT_POOL stuPort)
{
	std::set<STU_PORT_POOL>::iterator it = g_stuPortPool.begin();
	for (; it != g_stuPortPool.end(); it++)
	{
		if (it->nStartPort == stuPort.nStartPort &&
			it->nRange == stuPort.nRange && 
			stuPort.bEnabl == false) // ��ǰ�˿ڲ�����
		{
			g_stuPortPool.erase(it);
			it--;
		}
	}

	updatePort();
}

/*
* @fn			getFreeDynamicPortsNum
* @brief		��ȡʣ�ද̬�˿�����
* @param[in]    
* @param[out]	
* @return		if success return true
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
bool getFreeDynamicPortsNum(__out int& nNum)
{
	bool bRet = false;
	int nPortsNum = 0;

	STU_PORT_POOL stuDynamicPort;
	std::set<int> CurSysUsedPort;
	std::set<int>::iterator it;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	if ( 0 != getDynamicPortFromstr(stuDynamicPort) ) 
	{
		goto _END_;
	}

	// 2 ��ȡ��ǰϵͳ�˿�ռ�����
	CurSysUsedPort = GetAllTcpConnectionsPort();
	
	// 3 ͳ�ƶ�̬�˿ڸ���
	for (it = CurSysUsedPort.begin(); it != CurSysUsedPort.end(); it++)
	{
		if(stuDynamicPort.isContain(*it))
		{
			nPortsNum++;
		}
	}

	nNum = stuDynamicPort.nRange - nPortsNum;
	bRet = true;
_END_:
	if (CurSysUsedPort.size())
	{
		CurSysUsedPort.clear();
		/*
		it = CurSysUsedPort.begin();
		for (; it != CurSysUsedPort.end(); it++)
		{
			it = CurSysUsedPort.erase(it);
			it--;
		}*/
	}
	return bRet;
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
* @return		vector<STU_PORT_POOL>: ���ض˿ڳ�(STU_PORT_POOL)������
*               
* @detail      	��ϵͳ��̬�˿�ʹ�����������IEGʱ���ô˽ӿڻ�ȡ��̬�˿���Դ�أ����浽 g_stuPortPool ������
* @author		mingming.shi
* @date			2022-1-24
*/
int CreatPortPool()
{
	std::vector<STU_PORT_POOL> stuPortPool;			// ����ӿڷ��صĶ˿ڷ�Χ
	std::set<STU_PORT_POOL> stuFreeDynamicPort;	// ����δ��ʹ�ö˿ڷ�Χ
	STU_PORT_POOL	stuDynamicPort;
	std::set<int>	CurSysUsedPort;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	getDynamicPortFromstr(stuDynamicPort);

	// 2 ��ȡ�˿�ռ�����
	CurSysUsedPort = GetAllTcpConnectionsPort();

	// 3 ��Ĭ�ϵ�ַ��ʼ���ҿ��ö˿ڷ�Χ
	if ( 0 == getFreeStaticPort(stuDynamicPort, CurSysUsedPort, stuFreeDynamicPort) )
	{
		g_stuPortPool = stuFreeDynamicPort;
	}

	return 0;
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
static int ExecuteCMD(__out char *pChRet)
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
* @param[in]    str: ���������������ַ���
* @param[out]	stuDynamicPort: ��̬�˿ڵ���ʼ��ַ�ͷ�Χ
* @return		
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
	if ( 0 != ExecuteCMD(chRet) )
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
		iRet = -2;
		goto _END_;
	}
	stuDynamicPort.nStartPort	= vecNumFromStr[0];
	stuDynamicPort.nRange		= vecNumFromStr[1];
	
	iRet = 0;
_END_:
	return iRet; //���ϴ�С���ǲ�ͬ��������
}

/*
* @fn			GetAllTcpConnectionsPort
* @brief		��ȡ�����Ӷ˿���
* @param[in]    
* @param[out]	ret�������Ӷ˿ڵ�set����
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
std::set<int> GetAllTcpConnectionsPort()
{
	ULONG size = 0;
	PMIB_TCPTABLE pTcpTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	std::set<int> ret;
	GetTcpTable(NULL, &size, TRUE);

	if ( ERROR_INSUFFICIENT_BUFFER == (dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) ) 
	{
			free(pTcpTable);
			pTcpTable = (MIB_TCPTABLE *)malloc(dwSize);
			if (pTcpTable == NULL) 
			{
				printf("Error allocating memory\n");
				// return 1;
			}
	}

	if ((dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) == NO_ERROR) 
	{

		for (int i = 0; i < (int)pTcpTable->dwNumEntries; i++) 
		{
			ret.insert(pTcpTable->table[i].dwLocalPort);
		}
	}
	else 
	{
		free(pTcpTable);
		// return 1;
	}

	if (pTcpTable != NULL) 
	{
		free(pTcpTable);
		pTcpTable = NULL;
	}

	return ret;
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
* @param[out]	
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2022-1-24
*/
int getFreeStaticPort(__in STU_PORT_POOL stuDynamicPort, __in std::set<int> CurSysUsedPort, __out std::set<STU_PORT_POOL>& stuFreeStaticPort)
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

		stuFreeStaticPort.insert(stuTempPort);
		if (stuFreeStaticPort.size() > MAX_PORT_NUM)
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
	std::set<int>	CurSysUsedPort;

	// 1 ��ȡ��̬�˿�״̬����ʼ��ַ�ͷ�Χ��
	getDynamicPortFromstr(stuDynamicPort);

	// 2 ��ȡ�˿�ռ�����
	CurSysUsedPort = GetAllTcpConnectionsPort();

	// 3 ���������˿ڣ��Ӷ˿ڳ����һ���˿ڿ�ʼ
	std::set<STU_PORT_POOL>::iterator it = g_stuPortPool.end();
	nStart = it->nStartPort + it->nRange;

	for (UINT nPort = nStart; nPort != nEnd; ++nPort)
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

		g_stuPortPool.insert(stuTempPort);
		if (g_stuPortPool.size() > MAX_PORT_NUM)
		{
			break;
		}
	}
}