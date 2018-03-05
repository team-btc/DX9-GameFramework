#pragma once
#include <oledb.h>
#include <stdio.h>
#include <conio.h>
#include "icrsint.h"

class CAdo
{
private:
    BOOL m_bIsConnected;
    HRESULT m_hresult;
    _ConnectionPtr m_pConn;
    _RecordsetPtr m_pRset;

public:
    CAdo();
    virtual ~CAdo();

#pragma region CONNECT
    BOOL        Connect(char* szDBServerIP, char* szDBName, char* szUser);
    BOOL        Connect(string szDBServerIP, string szDBName, string szUserID, string szUserPW);
    BOOL        Disconnect();
#pragma endregion

#pragma region CRUD
    void*       SQLExecute(char* szSQL);
    BOOL        SQLInsert(char* szTableName, char* szValues);
    _RecordsetPtr  SQLSelect(string szTableName, int id);
    _RecordsetPtr SQLSelect(string szTableName, string szCondition = "");
    BOOL        SQLUpdate(char* szTableName, char* szSet, char* szWhere);
    BOOL        SQLDelete(char *szTableName, char *szWhere);
#pragma endregion
};