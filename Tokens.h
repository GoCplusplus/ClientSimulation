#ifndef __TOKENS__H
#define __TOKENS__H
#include <cstring>

// �ؼ��ֶ���
enum Tokens
{
	token_help = 1, // help
	token_set, // set
	token_secretkey, // ��Կ
	token_encrypt, // �Ƿ����
	token_loginM, // ��¼
	token_loginG,
	token_quit, // �˳�
	token_sendM, // ����M��Ϣ
	token_sendG,
	token_connect, // ����
	token_reload, // ���¼���xml�ļ�
	token_loginMbatch, // M������¼
	token_loginGbatch, // G������¼
	token_sendbatch, // �����������������Ϣ
};

typedef struct {
	int ntokens;
	char szDesc[20];
}TOKENS_DESCRIBE;

const TOKENS_DESCRIBE tokens[] = {
	{
		token_help,
		"help"
	},
	{
		token_set,
		"set"
	},
	{
		token_secretkey,
		"secretkey"
	},
	{
		token_encrypt,
		"encrypt"
	},
	{
		token_loginM,
		"loginM"
	},
	{
		token_loginG,
		"loginG"
	},
	{
		token_quit,
		"quit"
	},
	{
		token_sendM,
		"sendM"
	},
	{
		token_sendG,
		"sendG"
	},
	{
		token_connect,
		"connect"
	},
	{
		token_reload,
		"reload"
	},
	{
		token_loginMbatch,
		"loginMbatch"
	},
	{
		token_loginGbatch,
		"loginGbatch"
	},
	{
		token_sendbatch,
		"sendbatch"
	}
};

const int TOKEN_ITEM_SIZE = 50;

class TokensTable 
{

private:
	struct HashNode
	{
		TOKENS_DESCRIBE token;
		HashNode* pNext;
		HashNode()
		{
			token.ntokens = -1;
			memset(token.szDesc, 0, sizeof(token.szDesc));
			pNext = nullptr;
		}
	};

private:
	TokensTable() {}
public:
	~TokensTable()
	{
		for (int i = 0; i < TOKEN_ITEM_SIZE; i++)
		{
			HashNode hashNode = hashTable[i];
			DeleteHashNode(hashNode.pNext);
		}
	}
private:
	void DeleteHashNode(HashNode*& pNode)
	{
		if (!pNode)
			return;
		DeleteHashNode(pNode->pNext);
		delete pNode;
		pNode = nullptr;
	}
public:
	static int AddTokens(TOKENS_DESCRIBE token);
	static bool FindToken(const char* pWord, int& nToken);
private:
	static int GetHashCode(const char* pWord, int tablesize);
private:
	static HashNode hashTable[TOKEN_ITEM_SIZE];
};

void AddAllTokens();



#endif
