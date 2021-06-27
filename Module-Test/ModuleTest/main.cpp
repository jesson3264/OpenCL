#include <iostream>
using namespace std;

enum InnerMsgID
{
    INNER_MSG_ID_BACKWARD_INFO = 1, ///< ��ʾ��̨�·����õı��
    INNER_MSG_ID_STATIS_INFO = 2, ///< ��ʾ�����̷�����Ϣ���ӽ��̣���ȡͳ����Ϣ
    INNER_MSG_ID_REOPEN_LOG_FILE = 3 ///< ��־�и�
};

enum MsgDirection
{
    INNER_MSG_DIRECTION_UNDEFINED = 0,
    INNER_MSG_DIRECTION_MASTER_TO_CHILD = 1, ///< �����̵��ӽ���
    INNER_MSG_DIRECTION_CHILD_TO_MASTER = 2  ///< �ӽ��̵�������
};

typedef struct InnerMsgHead
{
    InnerMsgID msgid;    ///< ��ϢID
    MsgDirection direction;  ///< ��Ϣ����
    char uuid[37];           ///< UUID
    int processidx;          ///< ����������
    int datalen;             ///< ���ݳ��ȣ� �� data ��ʼ
    char data[0];
}InnerMsgHead;

char* memstr(const char* str1, int n1, const char* str2, int n2)
{
	if (n1 < n2)
	{
		return NULL;
	}

	int i = 0;
	int j = 0;

	char* s1 = NULL;
	char* s2 = NULL;
	char* cp = (char*)str1;

	for (; i < n1; i++)
	{
		s1 = cp;
		s2 = (char*)str2;

		while (*s1 == *s2)
		{
			s1++;
			s2++;
			j++;

			if (j == n2)
			{
				return cp;
			}
		}

		cp++;
		j = 0;
	}

	return NULL;
}
int main()
{
    const char* cstr = "POST /emm-core/securityController/validateIdentityReturnInfoV1.json HTTP/1.1\r\n"\
        "Content-Type: application/x-www-form-urlencoded\r\n"\
        "Cookie: JSESSIONID=6cae8782-b38c-4951-a95a-364dd44a9a45\r\n"\
        "Host: 127.0.0.1:8080\r\n\r\n"\
        "&strusername=yl2&tokenId=5fe7-4ca6e0f6871db303e53-&loginname=yl2&uidmobiledevid=5724afa6f31d8db40958b081bdd1fa2e";
	const char* cstr_end = cstr + strlen(cstr);
	char* jessonid = memstr(cstr, strlen(cstr), "JSESSIONID=", strlen("JSESSIONID="));
	char* jessonstart = jessonid + strlen("JSESSIONID=");
	char* jessonend = NULL;

	for (jessonend = jessonid + strlen("JSESSIONID="); jessonend < cstr_end; ++jessonend)
	{
		if (*jessonend == '\r')
			break;
	}

	char session[60] = {0};
	int len = jessonend - jessonstart;
	memcpy(session, jessonstart, len);
	cout << "len:" << len<<endl;
	cout << "session:"<<session << endl;

    //cout << str << endl;
    //cout << sizeof(InnerMsgHead) << endl;
	//cout << "Hello World." << endl;
	return 0;
}