#ifndef __CPARSECOMMAND__H
#define __CPARSECOMMAND__H
#include "Tokens.h"
#include <iostream>
#include "TCPSocketManage.h"

// ��������ģ��

class CParseCommand
{
public:
	CParseCommand();
	~CParseCommand();
public:
	bool ParseInput(const char* pInput);
	const char* GetErrorMsg()
	{
		return m_strErrorMsg.c_str();
	}

private:
	bool ParseCmd(const char* pCmd);
private:
	void PrintErrorMsg(const char* p, ...);
private:
	char GetCh()
	{
		return m_szInput[m_nIndex++];
	}
	void UnGetCh()
	{
		m_nIndex--;
	}
	bool GetString(std::string& str);
	void FilterSpace() // ���˿ո��tab
	{
		char ch;
		while ((ch = GetCh()) == ' ' || ch == '\t')
		{
			;
		}
		UnGetCh();
	}
	bool GetNumber(int& nNum); // �������
	bool GetIp(std::string& strIP);
	bool GetUserName(std::string& userName);
	bool GetPassWord(std::string& passWord);
private:
// ��������
	bool ParseHelp();
	bool ParseSet();
	bool ParseEncrypt();
	bool ParseLoginM();
	bool ParseLoginG();
	bool ParseQuit();
	bool ParseSendM();
	bool ParseSendG();
	bool ParseReload();
	bool ParseLoginMBatch();
	bool ParseLoginGBatch();
	bool ParseSendBatch();
public:
	bool m_bQuit; // �˳�
	bool m_bStart; // �Ƿ������˷�����
private:
	std::string m_strErrorMsg;
	char m_szInput[512];
	int  m_nIndex; // �������ڼ����ַ�
private:
	CTCPSocketManage m_tcpSocketManage; // socket����ģ��
};


#endif