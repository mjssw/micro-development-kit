/*
 * main.cpp
 *
 *  Created on: 2010-9-25
 *      Author: lyh
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mysql.h>
#include "odbc.h"
#include "dbmgr.h"

int mysql() {
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;

	mysql_init(&mysql); // ��ʼ��mysql�ṹ

	if (!mysql_real_connect(&mysql, "localhost", "root", "root", "test", 3306,
			NULL, 0)) {
		printf("\n���ݿ����ӷ�������!");
	} else {
		printf("\n���ݿ����ӳɹ�!\n");

		if (mysql_query(&mysql, "select id, name from test")) {
			printf("\n���ݿ��ѯ��������");
		} else {
			printf("\n��ʼ��ѯ...");
			result = mysql_store_result(&mysql); // ��ý����
			int numRows = mysql_num_rows(result); // ��ý�����еļ�¼��
			if (numRows > 0) {
				int numFields = mysql_num_fields(result); // ��ñ����ֶ���
				printf("\n�� %d �м�¼��ÿ�� %d ���ֶΡ�", numRows, numFields);
				int j = 0;
				while (row = mysql_fetch_row(result)) {
					printf("\n �� %d �У�", j);
					for (int i = 0; i < numFields; i++) {
						fprintf(stdout, " %s", row[i]); // ��ӡ�ֶ�ֵ
					}
					j++;
				}
			} else {
				printf("\n�޲�ѯ���!");
			}

			mysql_free_result(result); // �ͷŽ����
		}
	}

	mysql_close(&mysql); // �ͷ����ݿ�����
	return 0;
}

void InsertLogMsg(LPTSTR lpszMsg) {
	printf("%s", lpszMsg);
}
int main() {
	mysql();
	odbc();
	char sql[256];
	char* name = "root";
	GetDBManager()->Init(InsertLogMsg, "test", "root", "root");
	CConnection *pConn = GetDBManager()->m_dbMain.CreateConnection("test",
			"root", "root");
	if (pConn) {
		//insert record
		CRecordset *rec = pConn->CreateRecordset();
		for(int i = 0; i<10; i++) {
			sprintf(sql,"insert into test(id,name) values('%s')",name);
			if(!rec->Execute(sql)&&rec->GetRowCount()<=0) {
				printf("insert into falied!\n");
			}
		}
		pConn->DestroyRecordset(rec);
		//select record
		rec = pConn->CreateRecordset();
		if (!rec)
			return false;
		if (rec->Execute("select * from test")) {
			while (rec->Fetch()) {
				printf("id=%d, name=%s\n", atoi(rec->Get("id")), rec->Get(
						"name"));
			}
		}
		pConn->DestroyRecordset(rec);
	}
	GetDBManager()->m_dbMain.DestroyConnection(pConn);
	return 0;
}
