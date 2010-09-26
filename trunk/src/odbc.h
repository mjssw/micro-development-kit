#ifndef _DOUDYA_C_ODBC_H
#define _DOUDYA_C_ODBC_H
/** �����ļ� **/
#include <windef.h>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

/** �򵥵ĺ�,�����ж�ODBCAPI��ִ�н���Ƿ���ȷ **/
#define isSuc(result) ((result) == SQL_SUCCESS || (result) == SQL_SUCCESS_WITH_INFO)

/** ����չ���� **/
char szConnectString[256] =
		"driver={Microsoft Access Driver (*.mdb)};DBQ=E:" "ODBC.MDB";
char szSQL[256] = "select * from `test`";
/** ������� **/
char szDriverOutput[256];
short sDriverOutputLength;
/** ���ڲ��Եı��� **/
int a = 0;
char b[255];
char c[255];
long temp = 0;

/** ���ڲ��Եĺ��� **/
int odbc() {
	short sret; //���ش���
	void* henv; //�������
	void* hdbc; //���Ӿ��
	long mode;//�ύģʽ
	void* hsmt; //�����
	/** ���뻷����� **/
	sret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (!isSuc(sret))
		printf("���뻷���������\n");
	/** ���û�������,����ODBC�汾 **/
	sret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3,
			SQL_IS_INTEGER);
	if (!isSuc(sret))
		printf("����ODBC�汾����\n");
	/** �������Ӿ�� **/
	sret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	if (!isSuc(sret))
		printf("�������Ӿ������\n");
	/** ������������ **/
	sret = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT,
			(void*) SQL_AUTOCOMMIT_OFF, SQL_IS_INTEGER);
	if (!isSuc(sret))
		printf("�����������Գ���\n");
	/** ȡ���������� **/
	sret = SQLGetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (void*) &mode,
			sizeof(mode) + 1, NULL);
	if (!isSuc(sret))
		printf("ȡ���������Գ���\n");
	/** ��������Դ **/
	//	sret = SQLDriverConnect(hdbc, NULL, (unsigned char *) szConnectString,
	//			SQL_NTS, (unsigned char *) szDriverOutput, 256,
	//			&sDriverOutputLength, SQL_DRIVER_NOPROMPT);
	sret = SQLDriverConnect(hdbc, NULL, (unsigned char *)"DSN=test;UID=root;PASSWD=root", SQL_NTS,
			(unsigned char *) szDriverOutput, 256, &sDriverOutputLength,
			SQL_DRIVER_COMPLETE);
	if (!isSuc(sret))
		printf("��������Դ����\n");
	/** ��������� **/
	sret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hsmt);
	if (!isSuc(sret))
		printf("�������������\n");
	/** ִ����� **/
	sret = SQLExecDirect(hsmt, (SQLCHAR *) szSQL, SQL_NTS);
	if (!isSuc(sret))
		printf("ִ��������");
	/** �󶨽�����ͻ������ **/
	/**
	 С��ʾ������Ӧ�ÿ�����ȡ���ֶ��б�Ȼ����ѭ�������Զ���ġ�
	 **/
	SQLBindCol(hsmt, 1, SQL_C_LONG, &a, sizeof(a), &temp);//����ʱ
	SQLBindCol(hsmt, 2, SQL_C_CHAR, &b, sizeof(b), &temp);//����ʱ
	SQLBindCol(hsmt, 3, SQL_C_CHAR, &c, sizeof(c), &temp);//����ʱ
	/** ȡ������ **/
	while (1) {
		/** ���� **/
		sret = SQLFetchScroll(hsmt, SQL_FETCH_NEXT, 0);
		/**
		 ǰ�������������,����ܿ��ܵò���SQL_NO_DATA_FOUND��������ѭ����
		 �����Ҳ��ö���SQL_SUCCESS���жϣ���������Ӹ��ĳ���������Ȼ���á�
		 **/
		//if (sret == SQL_NO_DATA_FOUND) break; //���û�����ݾʹ�ϡ�
		if (sret != SQL_SUCCESS)
			break;
		printf("a = %d\n", a);//����ʱ
		printf("b = %s\n", b);//����ʱ
		printf("c = %s\n", c);//����ʱ
	}
	/** �ͷ������ **/
	SQLFreeHandle(SQL_HANDLE_STMT, hsmt);
	/** �ύ���ӵ����� **/
	SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	/** �Ͽ�������Դ������ **/
	SQLDisconnect(hdbc);
	/** �ͷ����Ӿ�� **/
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	/** �ͷŻ������ **/
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
	/////////�޹���䣬�������ó���ͣ�������������
	//getchar();//����ʱ
	return 0;
}

#endif
