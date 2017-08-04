#define _CRT_SECURE_NO_WARNINGS
#ifndef __CTCPSOCKETMANAGE__H
#define __CTCPSOCKETMANEGE__H
#include "TCPClientSocket.h"
#include "BalanceTree.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "ParseStruct.h"
#include "Log.h"

// TCP������
// һ��useridһ��socket

struct ClientUserInfo // ǳ�������� ����һ��CTCPClientSocket
{
	int nUserID; // �û�ID
	char szUserName[20]; // �û���
	CTCPClientSocket* socketHandle; // ���û�ID��Ӧ��socket

	ClientUserInfo()
	{
		nUserID = -1;
		socketHandle = nullptr;
	}

	bool operator ==(ClientUserInfo& rhs)
	{
		return nUserID == rhs.nUserID;
	}

	bool operator <(ClientUserInfo& rhs)
	{
		return nUserID < rhs.nUserID;
	}

	bool operator >(ClientUserInfo& rhs)
	{
		return nUserID > rhs.nUserID;
	}
};

class CTCPSocketManage
{
public:
	CTCPSocketManage();
	~CTCPSocketManage();

public:
	bool UserLogin(int nUserID, const char *szUserName, const char* szPassWord, LoginMG login, int nNameID = 0); // �û���¼
	bool UserLogOut(int nUserID); // �û��˳�
	void SetSecretKey(int nSecretKey); // ������Կ
	void SetEncrypt(bool bEncrypt); // �Ƿ����
	void SetConnectParam(std::string strIP, int nPort); //�������ӵķ�����������
	bool UserSendMessage(int nUserID, int nMainID, int nAssistID); // �û�������Ϣ
	void SendData(int nUserID, int nMainID, int nAssistID, int nLoopCount, LoginMG login); // ������Ϣ nLoopCount���Ͷ��ٴ�
	void Reload(LoginMG login); // ���¼���xml�ļ�
	void SendDataBatch(int nMainID, int nAssistID, int nLoopCount, LoginMG login);
	void SetStart(bool bStart);
	void Quit(); // �˳�
private:
	void HandleData(ClientUserInfo user);
	void UserLogin(ClientUserInfo* userInfo, const char* szUserName, const char* szPassWord); // ��¼M������
	void UserLogin(ClientUserInfo* userInfo, const char* szPassWord, int nNameID); // ��¼G������
	void CloseAll(); // �ر�����socket
private:
	CBalanceTree<struct ClientUserInfo> m_treeClientInfo;
	std::mutex       m_mutxUsers;

	int m_nSecretKey;
	bool m_bEncrypt;
	std::string m_strIP;
	int m_nPort;

	bool m_bExit; // ������־
	std::thread m_threadHandleData; // �����������ݵ��߳�
	bool m_bStart; // �Ƿ�ʼ��������

	// ����xml�ļ�
	ParseStruct     m_mXmlMParse; // M��������ϢID
	ParseStruct     m_mXmlGParse; // G��������ϢID
};

#endif