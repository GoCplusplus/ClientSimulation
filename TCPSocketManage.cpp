#include "TCPSocketManage.h"
#include "md5.h"


CTCPSocketManage::CTCPSocketManage() : m_mXmlMParse(".\\MStructDefine.xml"), m_mXmlGParse(".\\GStructDefine.xml")
{
	m_bExit = false;
	m_bStart = false;
	CLog::GetInstance()->Init();
	m_threadHandleData = std::thread([this]() {
		while (!m_bExit)
		{
			m_mutxUsers.lock();
			if (m_treeClientInfo.Empty() && m_bStart)
			{
				m_mutxUsers.unlock();
				break;
			}
			m_treeClientInfo.DoSomthing(std::bind(&CTCPSocketManage::HandleData, this, std::placeholders::_1));
			// ���δ���ӵ�socket �����йر�

			m_mutxUsers.unlock();
		}
	}); // ��������
}


CTCPSocketManage::~CTCPSocketManage()
{
}

bool CTCPSocketManage::UserLogin(int nUserID, const char* szUserName, const char* szPassWord, LoginMG login, int nNameID)
{
	ClientUserInfo userLogin;
	userLogin.nUserID = nUserID;
	strcpy(userLogin.szUserName, szUserName);
	userLogin.socketHandle = new CTCPClientSocket;
	userLogin.socketHandle->SetSecretKey(m_nSecretKey);
	userLogin.socketHandle->SetEncrypt(m_bEncrypt);
	userLogin.socketHandle->SetUserID(nUserID);
	bool nRet;
	nRet = userLogin.socketHandle->Init();
	if (!nRet)
	{
		delete userLogin.socketHandle;
		return true;
	}
	nRet = userLogin.socketHandle->Connect(m_strIP, m_nPort); // ���ӳɹ�
	if (!nRet)
	{
		delete userLogin.socketHandle;
		return true;
	}
	else
	{
		m_mutxUsers.lock();
		m_treeClientInfo.InsertElement(userLogin);
		m_mutxUsers.unlock();
	}
	// �û���¼
	while (userLogin.socketHandle->GetReserve() == 0)
		std::this_thread::sleep_for(std::chrono::microseconds(10)); //�ȴ���ֵ
	userLogin.socketHandle->SetType(login);
	if (login == LoginMG::LOGIN_M)
		UserLogin(&userLogin, szUserName, szPassWord); // ��¼����
	else
		UserLogin(&userLogin, szPassWord, nNameID); // ��¼G������
	m_bStart = true;
	return true;
}

bool CTCPSocketManage::UserLogOut(int nUserID)
{
	return false;
}

bool CTCPSocketManage::UserSendMessage(int nUserID, int nMainID, int nAssistID)
{
	return false;
}

void CTCPSocketManage::SetSecretKey(int nSecretKey) // ��������������Կ
{
	m_nSecretKey = nSecretKey;
}

void CTCPSocketManage::SetEncrypt(bool bEncrypt) // ���������Ƿ����
{
	m_bEncrypt = bEncrypt;
}

void CTCPSocketManage::SetConnectParam(std::string strIP, int nPort)
{
	m_strIP = strIP;
	m_nPort = nPort;
}

void CTCPSocketManage::HandleData(ClientUserInfo user)
{
	user.socketHandle->HandleData();
}

void CTCPSocketManage::UserLogin(ClientUserInfo* userInfo, const char* szUserName, const char* szPassWord)
{
	MSG_GP_S_LogonByNameStruct loginPlace;
	memset(&loginPlace, 0, sizeof(loginPlace));

	loginPlace.bForced = false;
	loginPlace.iUserID = userInfo->nUserID;
	loginPlace.gsqPs = 5471;
	strcpy(loginPlace.szName, szUserName);

	MD5_CTX Md5;
	Md5.MD5Update((unsigned char *)szPassWord, lstrlen(szPassWord));
	unsigned char szMDTemp[16];
	Md5.MD5Final(szMDTemp);
	char szMD5Input[64];
	for (int i = 0; i < 16; i++)
	{
		wsprintf(&szMD5Input[i * 2], "%02x", szMDTemp[i]);
	}
	strcpy(loginPlace.szMD5Pass, szMD5Input);

	userInfo->socketHandle->SendData(&loginPlace, sizeof(loginPlace), MDM_GP_LOGON, ASS_GP_LOGON_BY_NAME); // ���͵�¼
}

void CTCPSocketManage::UserLogin(ClientUserInfo* userInfo, const char* szPassWord, int nNameID) // ��¼G������
{
	MSG_GR_S_RoomLogon loginRoom;
	memset(&loginRoom, 0, sizeof(loginRoom));
	loginRoom.bForced = false;
	loginRoom.dwUserID = userInfo->nUserID;
	loginRoom.uNameID = nNameID;

	MD5_CTX Md5;
	Md5.MD5Update((unsigned char *)szPassWord, lstrlen(szPassWord));
	unsigned char szMDTemp[16];
	Md5.MD5Final(szMDTemp);
	char szMD5Input[64];
	for (int i = 0; i < 16; i++)
	{
		wsprintf(&szMD5Input[i * 2], "%02x", szMDTemp[i]);
	}
	strcpy(loginRoom.szMD5Pass, szMD5Input);
	userInfo->socketHandle->SendData(&loginRoom, sizeof(loginRoom), MDM_GR_LOGON, ASS_GR_LOGON_BY_ID); // ���͵�¼
	
}

void CTCPSocketManage::SendData(int nUserID, int nMainID, int nAssistID, int nLoopCount, LoginMG login) // ��������
{
	ClientUserInfo userInfo;
	userInfo.nUserID = nUserID;
	userInfo.socketHandle = nullptr;
	m_mutxUsers.lock();
	bool bRet = m_treeClientInfo.FindElement(userInfo);
	m_mutxUsers.unlock();
	if (!bRet)
		return;
	if (!userInfo.socketHandle)
		return;
	char szMessage[MaxBuffLen] = { 0x00 };
	if (login == LoginMG::LOGIN_M) // M
	{
		int nLen = 0;
		m_mXmlMParse.GetMessageBody(nMainID, nAssistID, szMessage, nLen);
		userInfo.socketHandle->SendData(szMessage, nLen, nMainID, nAssistID);
	}
	else // G
	{
		int nLen;
		m_mXmlGParse.GetMessageBody(nMainID, nAssistID, szMessage, nLen);
		userInfo.socketHandle->SendData(szMessage, nLen, nMainID, nAssistID);
	}
}

void CTCPSocketManage::Reload(LoginMG login) // ���¼���xml
{
	if (login == LoginMG::LOGIN_M)
		m_mXmlMParse.ReloadXml();
	else
		m_mXmlGParse.ReloadXml();
}

void CTCPSocketManage::SendDataBatch(int nMainID, int nAssistID, int nLoopCount, LoginMG login) // ������������ nLoopCount��
{
	char szMessage[MaxBuffLen] = { 0x00 };
	int nLen = 0;
	if (login == LoginMG::LOGIN_M)
		m_mXmlMParse.GetMessageBody(nMainID, nAssistID, szMessage, nLen);
	else
		m_mXmlGParse.GetMessageBody(nMainID, nAssistID, szMessage, nLen);
	for (int i = 0; i < nLoopCount; i++)
	{
		m_mutxUsers.lock();
		m_treeClientInfo.DoSomthing([&](ClientUserInfo& info) { info.socketHandle->SendData(szMessage, nLen, nMainID, nAssistID); });
		m_mutxUsers.unlock();
	}
}

void CTCPSocketManage::Quit()
{
	m_mutxUsers.lock();
	m_treeClientInfo.DoSomthing([](ClientUserInfo& info) { info.socketHandle->SetExit(true); }); // ����Ͷ�ݽ�����Ϣ
	m_mutxUsers.unlock();

	g_ThreadPool.Quit(); // �ȴ���������ִ�����
	CloseAll(); // �ر�����socket
	m_mutxUsers.lock();
	m_treeClientInfo.Clear(); // ������нڵ�
	m_mutxUsers.unlock();
	if (m_threadHandleData.joinable())
		m_threadHandleData.join(); // �����߳��˳�
	CLog::GetInstance()->Write("Quit...");
	CLog::GetInstance()->Exit();
}

void CTCPSocketManage::CloseAll()
{
	m_mutxUsers.lock();
	m_treeClientInfo.DoSomthing([](ClientUserInfo& info) { info.socketHandle->CloseSocket(); });
	m_mutxUsers.unlock();
}

void CTCPSocketManage::SetStart(bool bStart) // δ��¼ʱ���˳�
{
	m_bStart = true;
	if (m_threadHandleData.joinable())
		m_threadHandleData.join();
}