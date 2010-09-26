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

	mysql_init(&mysql); // 初始化mysql结构

	if (!mysql_real_connect(&mysql, "localhost", "root", "root", "test", 3306,
			NULL, 0)) {
		printf("\n数据库连接发生错误!");
	} else {
		printf("\n数据库连接成功!\n");

		if (mysql_query(&mysql, "select id, name from test")) {
			printf("\n数据库查询发生错误");
		} else {
			printf("\n开始查询...");
			result = mysql_store_result(&mysql); // 获得结果集
			int numRows = mysql_num_rows(result); // 获得结果集中的记录数
			if (numRows > 0) {
				int numFields = mysql_num_fields(result); // 获得表中字段数
				printf("\n共 %d 行记录，每行 %d 个字段。", numRows, numFields);
				int j = 0;
				while (row = mysql_fetch_row(result)) {
					printf("\n 第 %d 行：", j);
					for (int i = 0; i < numFields; i++) {
						fprintf(stdout, " %s", row[i]); // 打印字段值
					}
					j++;
				}
			} else {
				printf("\n无查询结果!");
			}

			mysql_free_result(result); // 释放结果集
		}
	}

	mysql_close(&mysql); // 释放数据库连接
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
