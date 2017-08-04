#ifndef __PARSESTRUCT__H
#define __PARSESTRUCT__H
#include "tinyxml.h"
#include <map>
#include <iostream>
#include "Log.h"

struct MessageBody
{
	char* pData; // ����
	int   nLen;  // ����
	MessageBody()
	{
		pData = nullptr;
		nLen = 0;
	}
};

struct MessageID
{
	int nMainID;
	int nAssistID;
	MessageID()
	{
		nMainID = 0;
		nAssistID = 0;
	}
	
	bool operator< (const MessageID& rhs) const // ����Ϊconst
	{
		if (nMainID < rhs.nMainID)
			return true;
		else if (nMainID > rhs.nMainID)
			return false;
		else
		{
			if (nAssistID < rhs.nAssistID)
				return true;
			else
				return false;
		}
	}

	bool operator== (MessageID& rhs)
	{
		return (nMainID == rhs.nMainID) && (nAssistID == rhs.nAssistID);
	}
};


class ParseStruct
{
public:
	ParseStruct(std::string strPath) : m_xmlDoc(strPath.c_str())
	{
		if (!m_mapMessage.empty())
		{
			for (auto message : m_mapMessage)
				delete[] message.second.pData;
			m_mapMessage.clear();
		}

		bool bLoad = m_xmlDoc.LoadFile();
		if (!bLoad)
		{
			m_strError = "����xml�ļ�ʧ��";
			CLog::GetInstance()->Write("����xml�ļ�ʧ��: %s", strPath.c_str());
		}
		bLoad = InitXml();
	}
	~ParseStruct();
	bool GetMessageBody(int nMaidID, int nAssistID, void* pData, int& nLen); // ������ɵ����� nLen��ʾʵ���ж����ֽ�

	void ReloadXml()
	{
		if (!m_mapMessage.empty())
		{
			for (auto message : m_mapMessage)
				delete[] message.second.pData;
			m_mapMessage.clear();
		}
		m_xmlDoc.LoadFile();
		InitXml();
	}

private:
	bool InitXml();
	void ParseBody(std::string& filedType, std::string& filedValue, char* pData, int& nLen); // �˴�����˶���

private:
	std::string m_strError;
	TiXmlDocument m_xmlDoc;
	std::map <struct MessageID, struct MessageBody> m_mapMessage;

};


#endif

