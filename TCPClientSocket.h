#define _CRT_SECURE_NO_WARNINGS
#ifndef __CTCPCLIENTSOCKET__H
#define __CTCPCLIENTSOCKET__H
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include "NetMessage.h"
#include <vector>
#include "ThreadPoolTask.h"
#include <atomic>

class CTCPSocketManage;

struct PackageSend 
{
	char* pData;
	int   nLen;
	bool  bRet; // ��Ϣ�Ƿ񱻴���
};

struct PackageRecv
{
	char* pData;
	int   nLen;
	bool  bRet; // �Ƿ��ǿ��е�
};

enum class LoginMG // ��¼M����G
{
	LOGIN_M, // ��¼M
	LOGIN_G, // ��¼G
};

// �ͻ���socket

class CTCPClientSocket
{
public:
	CTCPClientSocket();
	~CTCPClientSocket();
	friend CTCPSocketManage;
public:
	bool Init();
	bool Connect(std::string strIPAddr, int nPort); // ����
	int  SendData(void* pData, int nSize, int nMainID, int nAssistID); //��������
	void UDPRecvData(void* pBuff, int& nLen, struct PackageSend* pSend); // udp��������
	void HandleData(); // ������յ�����
	void CloseSocket(); // �ر�socket
public:
	const char* GetErrorMsg();
	void SetSecretKey(int nSecretKey); // ������Կ
	int GetSecretKey();
	void SetEncrypt(bool bEncrypt); // �Ƿ����
	bool GetEncrypt();
	void SetReserve(int nReserve); // �����ֶ�
	int  GetReserve();
	void SetType(LoginMG mg);
	LoginMG GetType();
	void SetUserID(int nUserID);
	int  GetUserID();
	void SetExit(bool bExit);
private:
	void SendDataPackage(void* pData);
	void RecvDataPackage(void* pData);
	void UDPRecvDataPackage(void* pData);
	void RecvData(struct PackageRecv* pSend); // ��������
	void HandleData(void* pData, int nLen);

private:
	void HandleConnectMessage(void* pData, int nMainID, int nAssistID);

private:
	void CleanUpSocket();
	void SetError();
private:
	SOCKET m_sTCPSocket; // TCPSocket
	SOCKET m_sUDPSocket; // UDPSocket
	std::string m_strErrorMsg;

	int m_nSecretKey; // ��Կ
	bool m_bEncrypt; // �Ƿ����
	bool m_bConnect; // �Ƿ����ӳɹ�
	bool m_bExit; // �ȴ��˳�
	int  m_nReserve;
	LoginMG m_Type; // ��M����G������
	int  m_nUserID; // ��֮��Ӧ��userid

	std::string m_strIP;
	int m_nPort;

	int m_nRecvMaxNum; // ��ദ���������
	std::vector<struct PackageRecv*> m_vectPackageRecv; // ���ջ�����
};

#endif