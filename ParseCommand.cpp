#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include "ParseCommand.h"
#include <stdarg.h>
#include <cctype>
#include <sstream>
using namespace std;


CParseCommand::CParseCommand()
{
	m_bQuit = false;
}


CParseCommand::~CParseCommand()
{
}

bool CParseCommand::ParseInput(const char* pInput)
{
	if (*pInput == 0)
	{
		PrintErrorMsg("invalid input");
		return false;
	}
	memset(m_szInput, 0, sizeof(m_szInput));
	m_nIndex = 0;
	strncpy(m_szInput, pInput, 512);

	FilterSpace(); // �ȹ��˿ո�
	int nIndex = m_nIndex;
	char ch;
	while (((ch = GetCh()) != ' ') && ch != 0)
	{
		if (!isalpha(ch))
		{
			PrintErrorMsg("invalid input: %s", m_szInput);
			return false;
		}
	}
	char szKey[20] = { 0x00 }; // ��ȡkey�ؼ���
	strncpy(szKey, m_szInput + nIndex, m_nIndex - nIndex);
	if (m_szInput[m_nIndex - 1] == ' ')
		szKey[m_nIndex - 1] = 0;
	return ParseCmd(szKey);
}

void CParseCommand::PrintErrorMsg(const char* p, ...)
{
	char szMsg[512] = { 0x00 };
	va_list arg;
	va_start(arg, p);
	vsprintf(szMsg, p, arg);
	m_strErrorMsg = std::move(std::string(szMsg));
}

bool CParseCommand::ParseCmd(const char* pCmd)
{
	int nToken;
	bool bHaveToken = TokensTable::FindToken(pCmd, nToken);
	if (!bHaveToken)
	{
		PrintErrorMsg("invalid keyword: %s", pCmd);
		return false;
	}

	switch (nToken)
	{
	case token_help:
		return ParseHelp();
	case token_set:
		return ParseSet();
	case token_loginM:
		return ParseLoginM();
	case token_loginG:
		return ParseLoginG();
	case token_quit:
		return ParseQuit();
	case token_sendM: // �������� loopcount
		return ParseSendM();
	case token_sendG: // �������� loopcount
		return ParseSendG();
	case token_reload:
		return ParseReload(); // ���¼���xml�ļ�
	case token_loginMbatch:
		return ParseLoginMBatch(); // ������¼M������
	case token_loginGbatch:
		return ParseLoginGBatch(); // ������¼G������
	case token_sendbatch:
		return ParseSendBatch(); // ����������Ϣ
	default:
		return true;
	}

}

bool CParseCommand::ParseHelp()
{
	std::cout <<"list of command: " << std::endl << std::endl;
	
	std::cout << "help:\t" << "����help�����Բ鿴֧�ֵ�������" << std::endl;
	printf("\t%-10s", "example: help");
	printf("\n\n");

	std::cout << "set:\t" << "����set���������������Ϣ" << std::endl;
	printf("\t%-10s", "set secretkey\t������Կ");
	printf("\n");
	printf("\t%-10s", "example: set secretkey 20170401");
	printf("\n");
	printf("\t%-10s", "set encrypt\t�����Ƿ���� 0������ 1����");
	printf("\n");
	printf("\t%-10s", "example: set encrypt 1");
	printf("\n");
	printf("\t%-10s", "set connect\t�������Ӳ���");
	printf("\n");
	printf("\t%-10s", "example: set connect 192.168.1.106 3015\n");
	printf("\n");

	std::cout << "loginM:\t" << "�û���¼M������\n"<< "\tloginM userid username password " << std::endl;
	printf("\t%-10s", "example: loginM 10027 hn7800 123456\n");
	printf("\n");

	std::cout << "loginMbatch: " << "�����û���¼M������\n" << "\tloginMbatch useridstart useridend prefixname prefixstart password " << std::endl;
	printf("\t%-10s", "example: loginMbatch 10018 10026 HN 1001 123456\n");
	printf("\n");

	std::cout << "loginGbatch: " << "�����û���¼G������\n" << "\tloginGbatch useridstart useridend prefixname prefixstart password nameid" << std::endl;
	printf("\t%-10s", "example: loginGbatch 10018 10026 HN 1001 123456 12100004\n");
	printf("\n");

	std::cout << "loginG:\t" << "�û���¼G������\n" << "\tloginG userid username password nameid" << std::endl;
	printf("\t%-10s", "example: loginG 10027 hn7800 123456 12100004\n");
	printf("\n");

	std::cout << "sendM:\t" << "�û�������Ϣ��M������\n" << "\tsendM mainid assistid loopcount " << std::endl;
	printf("\t%-10s", "example: sendM 136 2 1\n");
	printf("\n");

	std::cout << "sendG:\t" << "�û�������Ϣ��G������\n" << "\tsendG mainid assistid loopcount " << std::endl;
	printf("\t%-10s", "example: sendG 181 2 1\n");
	printf("\n");

	std::cout << "sendbatch: " << "����������Ϣ\n" << "\tsendbatch [M|G] mainid assistid loopcount" << std::endl;
	printf("\t%-10s", "example: sendbatch M 136 2 1000\n");
	printf("\n");

	std::cout << "reload:\t" << " ���¼���xml�ļ�\n"<< "\t���޸�����Ϣ����xml�ļ�ʱ���ô��������¼��� " << std::endl;
	printf("\t%-10s", "example: reload");
	printf("\n\n");

	std::cout << "quit:\t" << " �˳��ó���" << std::endl;
	printf("\t%-10s", "example: quit");
	printf("\n");
	return true;
}

bool CParseCommand::ParseSet()
{
	std::string szKeyWord = "";
	bool nRet = GetString(szKeyWord);
	if (!nRet)
		return false;
	int nToken;
	bool bHaveToken = TokensTable::FindToken(szKeyWord.c_str(), nToken);
	if (!bHaveToken)
	{
		PrintErrorMsg("invalid keyword: %s", szKeyWord.c_str());
		return false;
	}
	switch (nToken)
	{
	case token_secretkey: // ������Կ
	{
		bool bRet;
		int nSecretKey = 0;
		bRet = GetNumber(nSecretKey);
		if (!bRet)
			return false;
		m_tcpSocketManage.SetSecretKey(nSecretKey);
		return true;
	}
	case token_encrypt: // ���ü���
	{
		bool bRet;
		int bEncrypt = 0;
		bRet = GetNumber(bEncrypt);
		if (!bRet)
			return false;
		m_tcpSocketManage.SetEncrypt((bEncrypt > 0) ? true : false);
		return true;
	}
	case token_connect: // ��������
	{
		std::string strIP = "";
		bool bRet;
		int nPort;
		bRet = GetIp(strIP);
		if (!bRet)
			return false;
		bRet = GetNumber(nPort);
		if (!bRet)
			return false;
		if (nPort <= 0)
		{
			std::cout << "invalid port: " << nPort << std::endl;
			return false;
		}
		m_tcpSocketManage.SetConnectParam(strIP, nPort);
		return true;
	}
	default:
		break;
	}
	return true;
}

bool CParseCommand::ParseEncrypt()
{
	return true;
}

bool CParseCommand::ParseLoginM()
{
	bool bRet;
	int nUserID;
	std::string strUserName;
	std::string strPassWord;
	bRet = GetNumber(nUserID);
	if (!bRet)
		return false;
	bRet = GetUserName(strUserName);
	if (!bRet)
		return false;
	bRet = GetPassWord(strPassWord);
	if (!bRet)
		return false;
	m_tcpSocketManage.UserLogin(nUserID, strUserName.c_str(), strPassWord.c_str(), LoginMG::LOGIN_M);
	m_bStart = true;
	return true;
}

bool CParseCommand::ParseLoginG()
{
	bool bRet;
	int nUserID;
	int nNameID;
	std::string strUserName;
	std::string strPassWord;
	bRet = GetNumber(nUserID);
	if (!bRet)
		return false;
	bRet = GetUserName(strUserName);
	if (!bRet)
		return false;
	bRet = GetPassWord(strPassWord);
	if (!bRet)
		return false;
	bRet = GetNumber(nNameID);
	if (!bRet)
		return false;
	if (nNameID <= 0)
	{
		std::cout << "invalid nameid " << nNameID << std::endl;
		return false;
	}
	m_tcpSocketManage.UserLogin(nUserID, strUserName.c_str(), strPassWord.c_str(), LoginMG::LOGIN_G, nNameID);
	m_bStart = true;
	return true;
}

bool CParseCommand::ParseQuit()
{
	std::cout << "waiting for quit." << std::endl;
	if (m_bStart)
		m_tcpSocketManage.Quit();
	else
		m_tcpSocketManage.SetStart(true); // δ��¼ʱ���˳�
	m_bQuit = true;
	m_bStart = false;
	return true;
}

bool CParseCommand::ParseSendM()
{
	int nUserID = 0;
	bool bRet = GetNumber(nUserID);
	if (!bRet)
		return false;
	int nMainID = 0; // ��ID
	bRet = GetNumber(nMainID);
	if (!bRet)
		return false;
	int nAssistID = 0; // ����ID
	bRet = GetNumber(nAssistID);
	if (!bRet)
		return false;
	int nLoopCount = 0;
	bRet = GetNumber(nLoopCount); // ���ʹ���
	if (!bRet)
		return false;
	if (nLoopCount == 0)
		nLoopCount = 1;
	m_tcpSocketManage.SendData(nUserID, nMainID, nAssistID, nLoopCount, LoginMG::LOGIN_M);
	return true;
}

bool CParseCommand::ParseSendG()
{
	int nUserID = 0;
	bool bRet = GetNumber(nUserID);
	if (!bRet)
		return false;
	int nMainID = 0; // ��ID
	bRet = GetNumber(nMainID);
	if (!bRet)
		return false;
	int nAssistID = 0; // ����ID
	bRet = GetNumber(nAssistID);
	if (!bRet)
		return false;
	int nLoopCount = 0;
	bRet = GetNumber(nLoopCount); // ���ʹ���
	if (!bRet)
		return false;
	if (nLoopCount == 0)
		nLoopCount = 1;
	m_tcpSocketManage.SendData(nUserID, nMainID, nAssistID, nLoopCount, LoginMG::LOGIN_G);
	return true;
}

bool CParseCommand::ParseReload()
{
	bool bRet = false;
	std::string M_G; // ���¼���M��G��xml�ļ�
	bRet = GetString(M_G);
	if (!bRet)
		return false;
	if (M_G == "M")
		m_tcpSocketManage.Reload(LoginMG::LOGIN_M);
	else if (M_G == "G")
		m_tcpSocketManage.Reload(LoginMG::LOGIN_G);
	else
		std::cout << "invalid input: " << M_G.c_str() << std::endl;
	return true;
}

bool CParseCommand::ParseLoginMBatch() // ������¼M������
{
	bool bRet = false;
	int nUserStart; //�û�IDstart
	int nUserEnd;   // �û�IDEnd
	std::string strPrefix; // ǰ׺
	int nStart; // ��ʼ
	std::string strPassWord;
	bRet = GetNumber(nUserStart); // ��ʼ
	if (!bRet)
		return false;
	bRet = GetNumber(nUserEnd); // ����
	if (!bRet)
		return false;
	bRet = GetString(strPrefix); // ǰ׺
	if (!bRet)
		return false;
	bRet = GetNumber(nStart); // ��ʼ��
	if (!bRet)
		return false;
	bRet = GetPassWord(strPassWord);
	if (!bRet)
		return false;
	for (int i = 0; i < (nUserEnd - nUserStart + 1); i++)
	{
		std::stringstream strStream;
		strStream << strPrefix;
		strStream << (nStart + i);
		std::string strName;
		strStream >> strName;
		m_tcpSocketManage.UserLogin(nUserStart + i, strName.c_str(), strPassWord.c_str(), LoginMG::LOGIN_M);
	}
	m_bStart = true;
	return true;
}

bool CParseCommand::ParseLoginGBatch()
{
	bool bRet = false;
	int nUserStart; //�û�IDstart
	int nUserEnd;   // �û�IDEnd
	std::string strPrefix; // ǰ׺
	int nStart; // ��ʼ
	int nNameID; //nameid
	std::string strPassWord;
	bRet = GetNumber(nUserStart); // ��ʼ
	if (!bRet)
		return false;
	bRet = GetNumber(nUserEnd); // ����
	if (!bRet)
		return false;
	bRet = GetString(strPrefix); // ǰ׺
	if (!bRet)
		return false;
	bRet = GetNumber(nStart); // ��ʼ��
	if (!bRet)
		return false;
	bRet = GetPassWord(strPassWord);
	if (!bRet)
		return false;
	bRet = GetNumber(nNameID);
	if (!bRet)
		return false;
	if (nNameID <= 0)
	{
		std::cout << "invalid nameid " << nNameID << std::endl;
		return false;
	}
	for (int i = 0; i < (nUserEnd - nUserStart); i++)
	{
		std::stringstream strStream;
		strStream << strPrefix;
		strStream << (nStart + i);
		std::string strName;
		strStream >> strName;
		m_tcpSocketManage.UserLogin(nUserStart + i, strName.c_str(), strPassWord.c_str(), LoginMG::LOGIN_M, nNameID);
	}
	m_bStart = true;
	return true;
}

bool CParseCommand::ParseSendBatch() // ����������Ϣ
{
	bool bRet;
	int nMainID;
	int nAssistID;
	int nLoopCount;
	std::string m_g;
	bRet = GetString(m_g);
	if (!bRet)
		return false;
	if (m_g == "M" || m_g == "G")
	{
		bRet = GetNumber(nMainID);
		if (!bRet)
			return false;
		bRet = GetNumber(nAssistID);
		if (!bRet)
			return false;
		bRet = GetNumber(nLoopCount);
		if (!bRet)
			return false;
		if (nLoopCount == 0)
			nLoopCount = 1;
		if (m_g == "M")
			m_tcpSocketManage.SendDataBatch(nMainID, nAssistID, nLoopCount, LoginMG::LOGIN_M);
		else
			m_tcpSocketManage.SendDataBatch(nMainID, nAssistID, nLoopCount, LoginMG::LOGIN_G);
		return true;
	}
	std::cout << "invalid input: " << m_g << std::endl;
	return false;
}

bool CParseCommand::GetString(std::string& str)
{
	FilterSpace(); // ���˿ո�
	int nIndex = m_nIndex;
	char ch;
	while (((ch = GetCh()) != ' ') && ch != 0)
	{
		if (!isalpha(ch))
		{
			PrintErrorMsg("invalid input: %s", m_szInput);
			return false;
		}
	}
	char szParseString[512] = { 0x00 };
	strncpy(szParseString, m_szInput + nIndex, m_nIndex - nIndex);
	if (m_szInput[m_nIndex - 1] == ' ')
		szParseString[m_nIndex - nIndex - 1] = 0;
	str = szParseString;
	return true;
}

bool CParseCommand::GetNumber(int& nNum)
{
	FilterSpace(); // ���˿ո�
	int nIndex = m_nIndex;
	char ch;
	while (((ch = GetCh()) != ' ') && ch != 0)
	{
		if (!isdigit(ch))
		{
			PrintErrorMsg("invalid input: %s, not number", m_szInput);
			return false;
		}
	}
	char szParseString[512] = { 0x00 };
	strncpy(szParseString, m_szInput + nIndex, m_nIndex - nIndex);
	if (m_szInput[m_nIndex - 1] == ' ')
		szParseString[m_nIndex - nIndex - 1] = 0;
	nNum = atoi(szParseString);
	return true;
}

bool CParseCommand::GetIp(std::string& strIP)
{
	FilterSpace(); // ���˿ո�
	int nIndex = m_nIndex;
	char ch;
	while (((ch = GetCh()) != ' ') && ch != 0)
	{
		if (!isalpha(ch) && !isdigit(ch) && ch != '.')
		{
			PrintErrorMsg("invalid input: %s", m_szInput);
			return false;
		}
	}
	char szParseString[512] = { 0x00 };
	strncpy(szParseString, m_szInput + nIndex, m_nIndex - nIndex);
	if (m_szInput[m_nIndex - 1] == ' ')
		szParseString[m_nIndex - nIndex - 1] = 0;
	strIP = szParseString;
	return true;
}

bool CParseCommand::GetUserName(std::string& userName)
{
	FilterSpace(); // ���˿ո�
	int nIndex = m_nIndex;
	char ch;
	while (((ch = GetCh()) != ' ') && ch != 0)
	{
		if (!isalpha(ch) && !isdigit(ch))
		{
			PrintErrorMsg("invalid input: %s", m_szInput);
			return false;
		}
	}
	char szParseString[512] = { 0x00 };
	strncpy(szParseString, m_szInput + nIndex, m_nIndex - nIndex);
	if (m_szInput[m_nIndex - 1] == ' ')
		szParseString[m_nIndex - nIndex - 1] = 0;
	userName = szParseString;
	return true;
}

bool CParseCommand::GetPassWord(std::string& passWord)
{
	return GetUserName(passWord);
}