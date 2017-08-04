#ifndef __NETMESSAGE__H
#define __NETMESSAGE__H

#include <Windows.h>

#pragma pack(1)

const int MaxBuffLen = 2044;

static unsigned char g_chSecretKey[128] = "jS9uhwvVUTqiiJR1YdIzjvqutXSbJoypX3XA2rRnkgFL327OYj38DSKI7T36dDt5";

///������Ϣ
#define MDM_CONNECT						1			///������Ϣ����
///����������Ϣ��־
#define ASS_NET_TEST					1			///�������
#define ASS_CONNECT_SUCCESS 			3			///���ӳɹ�

struct NetMessageHead
{
	UINT uMessageSize; // ����С
	UINT bMainID;      // ��ID
	UINT bAssistantID; // ����ID
	UINT bHandleCode;  // handleCode
	UINT bReserver;    // ������
};

///���ӳɹ���Ϣ 
struct MSG_S_ConnectSuccess
{
	BYTE						bMaxVer;							///���°汾����
	BYTE						bLessVer;							///��Ͱ汾����
	BYTE						bReserve[2];						///�����ֶ�
	UINT						i64CheckCode;						///���ܺ��У���룬�ɿͻ��˽����ڰ�ͷ�з���
};

#define	MDM_GP_LOGON					100								///������½
#define ASS_GP_LOGON_BY_NAME			1							    ///ͨ���û����ֵ�½
#define ASS_GP_LOGON_SUCCESS			5							    ///��½�ɹ�

///�û���½���ʺţ��ṹ
struct MSG_GP_S_LogonByNameStruct
{
	bool								bForced;							///����
	UINT								uRoomVer;							///�����汾
	char								szName[64];							///��½����
	char								TML_SN[128];
	char								szMD5Pass[52];						///��½����
	char								szMathineCode[64];					///���������� ��������
	char                                szCPUID[24];						///CPU��ID
	char                                szHardID[64];						///Ӳ�̵�ID
	char								szIDcardNo[64];						///֤����
	char								szMobileVCode[8];					///�ֻ���֤��
	int									gsqPs;
	int									iUserID;							///�û�ID��¼�����ID>0��ID��¼
};

///������½�������ݰ�
struct MSG_GP_R_LogonResult
{
	long int							dwUserID;							///�û� ID 
	long int							dwGamePower;						///��ϷȨ��
	long int							dwMasterPower;						///����Ȩ��
	int									dwMobile;							///�ֻ�����
	int									dwAccID;							///Acc ����
	ULONG								dwLastLogonIP;						///�ϴε�½ IP
	ULONG								dwNowLogonIP;						///���ڵ�½ IP
	int									iLogonType;							//��¼����
	UINT								bLogoID;							///�û�ͷ��
	bool								bBoy;								///�Ա�
	char								szName[61];							///�û���¼��
	char								TML_SN[128];						///����ǩ��
	char								szMD5Pass[50];						///�û�����
	char								nickName[32];						///�û��ǳ�
	__int64								i64Money;							///�û����
	__int64								i64Bank;							///�û��Ƹ�
	int									iJewels;							//��ʯ
	int									iLotteries;							///��ȯ
	int									dwFascination;						///����

																			//JianGK 20111107���û�����
	char								szSignDescr[128];					///����ǩ��
	char								szRealName[20];						///��ʵ����
	char								szIDCardNo[36];						///֤����
	char								szMobileNo[50];						///�ƶ��绰
	char								szQQNum[20];						///QQ����
	char								szAdrNation[50];					///��ҵĹ���
	char								szAdrProvince[50];					///������ڵ�ʡ��
	char								szAdrCity[50];						///������ڵĳ���
	char								szZipCode[10];						///��������
	char								szEmail[50];						///�����ʼ�
	char								szAddress[128];						///��ϵ��ַ
	char								szSchooling[20];					///�Ļ��̶�
	char								szHomePage[128];					///������ҳ
	char								szTelNo[20];						///�̶��绰
	char								szMSNID[50];						//MSN�ʺ�
																			//end JianGK 20111107
	char								szHeadUrl[256];						//ͷ��URL
	int									dwTimeIsMoney;						///�ϴε�½ʱ������ȡ�Ľ��
	int									iVipTime;							///
	int									iDoublePointTime;					///˫������ʱ��
	int									iProtectTime;						///�����ʱ�䣬����
	bool								bLoginBulletin;						///�Ƿ��е�¼���棬Fred Huang,2008-05-20
	int									iLockMathine;						///��ǰ�ʺ��Ƿ�������ĳ̨������1Ϊ������0Ϊδ���� zxj 2009-11-13
	int									iBindMobile;						///��ǰ�ʺ��Ƿ���ֻ����룬1Ϊ�󶨣�0Ϊδ�� jianguankun 2012.10.10

	int									iAddFriendType;						///�Ƿ������κ��˼�Ϊ����
	int									iCutRoomID;							//�������������
};

// ��¼����
#define MDM_GR_LOGON 100
#define ASS_GR_LOGON_BY_ID 5									///ͨ���û� ID ��½
#define ASS_GR_LOGON_SUCCESS			2									///��½�ɹ�

///��Ϸ�����½ ����ά�Ȳ�һ����
struct MSG_GR_S_RoomLogon
{
	bool								bForced;							///����
	UINT								uNameID;							///���� ID
	UINT								dwUserID;							///�û� ID
	UINT								uRoomVer;							///�����汾
	UINT								uGameVer;							///��Ϸ�汾
	CHAR								szMD5Pass[50];						///��������
	//float                               fLongitude;                         ///����
	//float                               fLatitude;                          ///γ��
};

/////��Ϸ�����½
//struct MSG_GR_R_LogonResult
//{
//	LONG								dwGamePower;						///�û�Ȩ��
//	LONG								dwMasterPower;						///����Ȩ��
//	LONG								dwRoomRule;							///���ù���
//	UINT								uLessPoint;							///���پ���ֵ
//	UINT								uMaxPoint;							///��ྭ��ֵ
//	UserInfoStruct						pUserInfoStruct;					///�û���Ϣ
//	RECEIVEMONEY                        strRecMoney;                        //�Ǳ�������ҽ�Ҳ����Զ�����
//
//																			///wushuqun 2009.6.6
//																			///��¼����ʱ��ʱ��ȡ�����������
//	int									nVirtualUser;
//	int									nPresentCoinNum;  ///< ���ͽ������
//
//
//
//														  //����ר��
//	int									iContestID;
//	int									iLowCount;
//	__int64								i64Chip;
//	__int64								i64TimeStart;
//	__int64								i64TimeEnd;
//	__int64								i64LowChip;
//	int									iTimeout;
//	int									iBasePoint;
//
//	bool								bGRMUser;//����ҿ��Դ򿪷��������
//	bool								bGRMRoom;//�÷�����Դ򿪷��������
//
//	MSG_GR_GRM_UpData					GRM_Updata;//�����ڸ�������
//
//	MSG_GR_R_LogonResult()
//	{
//		memset(this, 0, sizeof(MSG_GR_R_LogonResult));
//	}
//
//
//};















#pragma pack()


#endif
