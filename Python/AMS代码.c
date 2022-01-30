
#pragma once

#pragma pack(1) // 内存对齐

#define HEADER_DAC_MAGIC ((unsigned int)'&$^#')
#define HEADER_DAC_VERSION 0x00010000
#define MAX_PATH_LEN (512)
#define RESERVED_LENGTH 64

/* 应用列表结构体 */
/* 配置 - 应用保护日志 */
typedef struct _AMS_APPGUARD_LOG_STRUCT
{
	WCHAR szBeGuardFileName[MAX_PATH_LENGTH];	// 被保护的文件名
	WCHAR szAppName[MAX_PATH_LEN];		// 应用名
	WCHAR szFilePath[MAX_PATH_LEN];		// 绝对路径
	LONGLONG llTime;					// 处理时间
	int nResult;						// 处理结果
} AMS_APPGUARD_LOG_STRUCT, *PAMS_APPGUARD_LOG_STRUCT;

public: /* 勒索病毒防护 - 应用保护 */
	BOOL Ams_AppGuard_Insert(__in const ST_AMS_PAC_APP_ITEM& pstDocGuardLog);
	BOOL Ams_AppGuard_InsertLogs(__in const std::vector<ST_AMS_PAC_APP_ITEM> &vecDocGuardLog);
	BOOL Ams_AppGuard_GetTotalCountLog(__out int& nTotalCount);
	BOOL Ams_AppGuard_GetOnePageLog(__in wstring wsSQL, __out wstring wsLogBuf, __in int nLogBuf); // 查询条件过于复杂，SQL在外面组合
	BOOL Ams_AppGuard_GetAndDeleteFixOneLog(__out ST_AMS_PAC_APP_ITEM pstDocGuardLog);
	BOOL Ams_AppGuard_DeleteLog(LONGLONG delID);


public: /* 勒索病毒防护 - 应用保护 */
	BOOL Ams_AppGuard_Insert(__in const ST_AMS_PAC_APP_ITEM& pstDocGuardLog) = 0 ;
	BOOL Ams_AppGuard_InsertLogs(__in const std::vector<ST_AMS_PAC_APP_ITEM> &vecDocGuardLog) = 0 ;
	BOOL Ams_AppGuard_GetTotalCountLog(__out int& nTotalCount) = 0 ;
	BOOL Ams_AppGuard_GetOnePageLog(__in wstring wsSQL, __out wstring wsLogBuf, __in int nLogBuf) = 0 ; // 查询条件过于复杂，SQL在外面组合
	BOOL Ams_AppGuard_GetAndDeleteFixOneLog(__out ST_AMS_PAC_APP_ITEM pstDocGuardLog) = 0 ;
	BOOL Ams_AppGuard_DeleteLog(LONGLONG delID) = 0 ;

/* 配置文件头 */
typedef struct __AMS_PAC_VERSION_HEADER
{
	unsigned int nMagic;
	unsigned int nVersion;
	unsigned char ucReserved[RESERVED_LENGTH];
}AMS_PAC_VERSION_HEADER, *PAMS_PAC_VERSION_HEADER;


/* 配置 - 进程 */
typedef struct _ST_AMS_PAC_APP_ITEM
{
	WCHAR szAppName[MAX_PATH_LEN]; // 主体进程名
} ST_AMS_PAC_APP_ITEM, *PST_AMS_PAC_APP_ITEM;


/* 配置 - 应用保护日志 */
typedef struct _ST_AMS_PAC_APP_LOG
{
	int nID;						// 主键
	WCHAR szAppName[MAX_PATH_LEN];	// 应用名
	WCHAR szFilePath[MAX_PATH_LEN];	// 绝对路径
	LONGLONG llTime;				// 处理时间
	int nResult;					// 处理结果
} ST_AMS_PAC_APP_LOG, *PST_AMS_PAC_APP_LOG;

		str_OperateTime.Format(_T("%I64d"), (LONGLONG)pstItem.llTime);
        str_AppName.Format(_T("%s"),        pstItem.szAppName);
        str_AppPath.Format(_T("%s"),        pstItem.szFilePath);
        str_Result.Format(_T("%d"),         pstItem.nResult);
		str_BeGuardedApp.Format(_T("%d"),   pstItem.szBeGuardFileName);
	/**
	tb_ams_AmsGuard_Log 勒索病毒防护-应用防护日志表 数据库 数据表 

    ID			-   主键
	GuardedFile	-	被保护文件名
    App     	-   应用进程名
    File    	-   进程路径（绝对路径）
    OP_Time	    -   处理时间
    Result		-   处理结果
    llRevert1   -   保留字段
    llRevert1	-   保留字段
    llRevert2	-   保留字段
    szRevert1	-   保留字段
    szRevert2	-   保留字段
	*/
    tstring wsSql_tb_ams_AmsGuard_Log = _T("CREATE TABLE IF NOT EXISTS [tb_ams_AppGuard_Log] ")
    _T(" ( ")
    _T("ID INTEGER PRIMARY KEY, ")
	_T("GuardedFile VARCHAR(256) COLLATE NOCASE, ")
	_T("App VARCHAR(256) COLLATE NOCASE, ")
	_T("File VARCHAR(256) COLLATE NOCASE, ")
    _T("OP_Time INTEGER, ")
	_T("Result INTEGER, ")
    _T("llRevert1  INTEGER, ")
    _T("llRevert1  INTEGER, ")
    _T("llRevert2  INTEGER, ")
    _T("szRevert1  VARCHAR(512), ")
    _T("szRevert2  VARCHAR(512)")
    _T(" ) ;");
    
/************************************************************************/
/* 应用保护数据库日志读写  start add by mingming.shi */
/************************************************************************/
/*
* @fn			Ams_AppGuard_Insert
* @brief		应用保护日志
* @param[in]    
* @param[out]	
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2021-12-9
*/
BOOL CManager::Ams_AppGuard_Insert(__in const ST_AMS_PAC_APP_LOG& pstAppGuardLog)
{
	BOOL bRet = TRUE;
	bRet = m_db.Ams_AppGuard_Insert(pstAppGuardLog);

	if( !bRet)
	{
		::Sleep(SLEEP_TIME);
		bRet = m_db.Ams_AppGuard_Insert(pstAppGuardLog);
	}
	return bRet;
}

BOOL CManager::Ams_AppGuard_InsertLogs(__in const std::vector<ST_AMS_PAC_APP_LOG> &vecAppGuardLog)
{
	BOOL bRet = TRUE;
	bRet = m_db.Ams_AppGuard_InsertLogs(AmsDocGuardLog);

	if( !bRet)
	{
		::Sleep(SLEEP_TIME);
		bRet = m_db.Ams_AppGuard_InsertLogs(AmsDocGuardLog);
	}
	return bRet;
}

BOOL CManager::Ams_AppGuard_GetTotalCountLog(__in LPCTSTR lpSQL, __out int& nTotalCount)
{
	BOOL bRet = FALSE;
	bRet = m_db.Ams_AppGuard_GetTotalCountLog(nTotalCount);
	return bRet;
}

BOOL CManager::Ams_AppGuard_GetOnePageLog(__in LPCTSTR lpSQL, __out LPTSTR lpLogBuf, __in int nLogBuf)
{
	BOOL bRet = FALSE;
	bRet = m_db.Ams_AppGuard_GetOnePageLog(lpSQL, lpLogBuf, nLogBuf);
	return bRet;
}

BOOL CManager::Ams_AppGuard_GetAndDeleteFixOneLog(__out ST_AMS_PAC_APP_LOG pIPSec)
{
	tstring wsOnePage = _T("");

	std::wstring wsSQL = _T("SELECT ID, App, File, OP_Time, Result FROM tb_ams_AppGuard_Log ORDER BY OP_Time ASC Limit 1 offset 0; ");
	BOOL bRet =  m_db.Ams_AppGuard_GetOnePageLog(wsSQL.c_str(), wsOnePage);

	if( !bRet)
	{
		::Sleep(SLEEP_TIME);
		bRet =  m_db.Ams_AppGuard_GetOnePageLog(wsSQL.c_str(), wsOnePage);
	}

	if( bRet)
	{
		CParse parse((TCHAR*)wsOnePage.c_str());
		int nRow = parse.GetRowCount();

		if (nRow < 1)
		{
			WriteDebug(_T("%S , parse.GetRowCount() < 1"), __FUNCTION__);
			return FALSE;
		}
		//ID, OP_Time, OP_Type, LogContent
		LONGLONG vecID = _tstoi64(parse.GetData(1, 0));
		pstItem->llTime = _tstoi64(parse.GetData(1, 1));
		_tcsncpy((TCHAR*)pstItem->szProcessName,  parse.GetData(1,2),_countof(pstItem->szProcessName));
		pstItem->nResult = _tstoi(parse.GetData(1, 3));

		if(!m_db.Ams_VirusTrap_DeleteLog(vecID))
		{
			WriteError(_T("%S: Ams_VirusTrap_DeleteLog failed"), __FUNCTION__);
			return FALSE;
		}
	}

	return bRet;
}
/************************************************************************/
/* 应用保护数据库日志读写  end */
/************************************************************************/

/************************************************************************/
/* 应用保护数据库日志读写  start add by mingming.shi */
/************************************************************************/
/*
* @fn			Ams_AppGuard_Insert
* @brief		
* @param[in]    
* @param[out]	
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2021-12-9
*/
BOOL CDataBase::Ams_AppGuard_Insert(__in const ST_AMS_PAC_APP_LOG& pstItem)
{
    if(!CheckOpenDB() && NULL == pstItem)
	{
		return FALSE;
	}

	CString strSQL;
	CString str_AppName;
	CString str_FilePath;
	CString str_OperateTime;
	CString str_Result;

	str_AppName.Format(_T("%s"), (LONGLONG)pstItem->szAppPath);
    str_FilePath.Format(_T("%s"), (LONGLONG)pstItem->szFilePath);
	str_OperateTime.Format(_T("%I64d"), (LONGLONG)pstItem->llTime);
	str_Result.Format(_T("%d"), pstItem->nResult);

	strSQL = _T("INSERT INTO tb_ams_AppGuard(OP_Time, App, File, Result) values ([%1], '[%2]', '[%3]', [%4]);");

	//假如没有值得话
	/*
	 *  删除默认路径，后续跟踪查看何种情况为空，系统完整性检测默认为空是正确的，其他情况待定
	 */
	if( _tcslen(pstItem->szAppPath)== 0) str_AppName = _T("-");
    if( _tcslen(pstItem->szFilePath)== 0) str_FilePath = _T("-");

	strSQL.Replace(_T("[%1]"), str_OperateTime);
    strSQL.Replace(_T("[%2]"), str_AppName);
	strSQL.Replace(_T("[%3]"), str_FilePath);
	strSQL.Replace(_T("[%4]"), str_Result);

	tstring wsSQL = strSQL;
	tstring wsResult = _T("");
	if( EXCUTE(wsSQL, wsResult) == FALSE)
	{
		WriteError(_T("CDataBase::Insert_AppGuard_Log, SQL =%s"), wsSQL.c_str());

		CloseDB();
		return FALSE;
	}
	return TRUE;
}

BOOL CDataBase::Ams_AppGuard_InsertLogs(__in const std::vector<ST_AMS_PAC_APP_LOG> &vecAppGuardLog)
{
    if(!CheckOpenDB())
    {
        return FALSE;
    }

    if (0 == vecAppGuardLog.size())
    {
        return TRUE;
    }

    CString                   strSQL;
	CString                   str_AppName;
	CString                   str_AppPath;
	CString                   str_OperateTime;
	CString                   str_Result;
    tstring                   wsResult;
    std::vector<std::wstring> vecSqls;

    for (std::size_t i = 0; i < vecAppGuardLog.size(); ++i)
    {
        ST_AMS_PAC_APP_LOG pstItem = vecAppGuardLog[i];
        strSQL = _T("INSERT INTO tb_ams_AppGuard(OP_Time, App, File, Operation, Result) values ([%1], '[%2]', '[%3]', [%4]);");

        str_OperateTime.Format(_T("%I64d"), (LONGLONG)pstItem->llTime);
        str_AppName.Format(_T("%s"), pstItem->szAppPath);
        str_AppPath.Format(_T("%s"), pstItem->szFilePath);
        str_Result.Format(_T("%d"), pstItem->nResult);

        if( _tcslen(pstItem->szAppPath) == 0) str_AppName = _T("-");
        if( _tcslen(pstItem->szFilePath) == 0) str_AppPath = _T("-");

        strSQL.Replace(_T("[%1]"), str_OperateTime);
        strSQL.Replace(_T("[%2]"), str_AppName);
        strSQL.Replace(_T("[%3]"), str_AppPath);
        strSQL.Replace(_T("[%4]"), str_Result);

        vecSqls.push_back(strSQL.GetString());
    }

	if( EXCUTE(vecSqls, wsResult) == FALSE)
	{
        WriteError(_T("CDataBase::Ams_AppGuard_InsertLogs, Error: %s"), wsResult.c_str());

		CloseDB();
		return FALSE;
	}
	return TRUE;
}

BOOL CDataBase::Ams_AppGuard_GetTotalCountLog(__out int& nTotalCount)
{
    if( !CheckOpenDB())
    {
        return FALSE;
    }

    tstring wsResult;
    tstring wsSQL = _T("SELECT COUNT(ID) FROM tb_ams_AppGuard_Log ;");
    
    if( EXCUTE(wsSQL, wsResult) == FALSE)
    {
        WriteError(_T("CDataBase::Ams_AppGuard_GetTotalCountLog, SQL = %s"), wsSQL.c_str());

        CloseDB();
        return FALSE;
    }

    CParse parse((TCHAR*)wsResult.c_str());
    if( parse.GetRowCount() < 2)
    {
        WriteError(_T("%S , parse.GetRowCount() < 2"), __FUNCTION__);
        return FALSE;
    }

    nTotalCount = _tstoi(parse.GetData(1, 0));
    return TRUE;
}

BOOL CDataBase::Ams_AppGuard_GetOnePageLog(__in wstring wsSQL, __out wstring wsLogBuf, __in int nLogBuf)
{
	if( !CheckOpenDB())
		return FALSE;

	tstring ws_SQL = wsSQL;
	tstring wsResult = _T("");

	if( EXCUTE(ws_SQL, wsResult) == FALSE)
	{
		WriteError(_T("CDataBase::Ams_AppGuard_GetOnePageLog, SQL = %s"), ws_SQL.c_str());
		CloseDB();
		return FALSE;
	}

	wsLogBuf = wsResult;

	return TRUE;
}

BOOL CDataBase::Ams_AppGuard_GetAndDeleteFixOneLog(__out ST_AMS_PAC_APP_LOG pstDocGuardLog)
{
    BOOL bRet = TRUE;

	if( !CheckOpenDB())
		return FALSE;

	tstring wsSQL = ws_SQL;
	tstring wsResult = _T("");

	if( EXCUTE(wsSQL, wsResult) == FALSE)
	{
		WriteError(_T("CDataBase::Ams_AppGuard_GetAndDeleteFixOneLog, SQL =%s"), wsSQL.c_str());
		CloseDB();
		return FALSE;
	}

	wsOnePage = wsResult;

    return bRet;
}

BOOL CDataBase::Ams_AppGuard_DeleteLog(LONGLONG delID)
{
	if( !CheckOpenDB())
	{
		return FALSE;
	}

	CString str_delID;
	str_delID.Format(_T("%I64d"), delID);

	CString strSQL = _T("DELETE FROM tb_ams_AppGuard_Log WHERE ID=[%1] ;" );
	strSQL.Replace(_T("[%1]"), str_delID);

	tstring wsSQL = strSQL;
	tstring wsResult = _T("");

	if( EXCUTE(wsSQL, wsResult) == FALSE)
	{
		WriteError(_T("CDataBase::Ams_AppGurard_DeleteLog, SQL =%s"), wsSQL.c_str());

		CloseDB();
		return FALSE;
	}

	return TRUE;
}
/************************************************************************/
/* 应用保护数据库日志读写  end */
/************************************************************************/

/*
* @fn			SendAppGuardLog
* @brief		发送应用保护日志
* @param[in]    
* @param[out]	
* @return		
*               
* @detail      	
* @author		mingming.shi
* @date			2021-12-9
*/
BOOL CWLSecModLogProcess::SendAppGuardLog(PAUDIT_AMS_APPGUARD pstAudit)
{	
	std::wstring wstrUserName;

	memset(m_LogBuf, 0, LOG_BUF_SIZE);
	IPC_LOG_COMMON* ipclogcomm        = (IPC_LOG_COMMON*)m_LogBuf;
	ipclogcomm->dwLogType             = WL_IPC_LOG_TYPE_ALARM;
	ipclogcomm->dwDetailLogTypeLevel1 = WL_IPC_LOG_TYPE_LEVE_1_APPGUARD;
	ipclogcomm->dwSize                = sizeof(AMS_APPGUARD_LOG_STRUCT);

	PAMS_APPGUARD_LOG_STRUCT pLog = (PST_ASM_PAC_LOG_ITEM)ipclogcomm->data;
	
	if ( 0 == _tcslen(pstAudit->fullPath) && 0 == _tcslen(pstAudit->processName) )
	{
		WriteWarn(_T("AppGuard Log is null!"));        
	}
	else
	{
		// 处理日志
		pLog->szBeGuardFileName = pstAudit->beGuardfileName;
		pLog->szAppName         = pstAudit->processName;
		pLog->szFilePath        = pstAudit->fullPath;
		pLog->nResult           = pstAudit->dwResult;
		
		struct tm _tm;
		_tm.tm_year  = pstAudit->hdr.auditTime.Year - 1900;
		_tm.tm_mon   = pstAudit->hdr.auditTime.Month - 1;
		_tm.tm_mday  = pstAudit->hdr.auditTime.Day;
		_tm.tm_hour  = pstAudit->hdr.auditTime.Hour;
		_tm.tm_min   = pstAudit->hdr.auditTime.Minute;
		_tm.tm_sec   = pstAudit->hdr.auditTime.Second;
		_tm.tm_isdst = -1;
		pLog->llTime = ::mktime(&_tm);
	}

	// 发送日志
	DWORD dwRet = m_WLWarnLogSender.SendLog(ipclogcomm);
	if (ERROR_SUCCESS != dwRet)
	{
		WriteError(_T("SendLog Failed, Error=0x%08x, FullPath=%ws, ProcessName=%ws, OPTResult=%d"), 
			dwRet, pLog->fullPath, pLog->processName, pLog->dwResultcode );

		return FALSE;
	}
	else
	{
		WriteInfo(_T("SendLog Success, FullPath=%s, ProcessName=%s, OPTResult=%d %d-%d-%d_%d:%d:%d llTime %llu"),
			pLog->fullPath,
			pLog->processName, pLog->dwResultcode /*pstAudit->logon*/,
			_tm.tm_year+1900,_tm.tm_mon,_tm.tm_mday,_tm.tm_hour,_tm.tm_min,_tm.tm_sec,pLog->llTime);

		return TRUE;
	}
}
