#ifndef _DOUDYA_C_ODBC_H
#define _DOUDYA_C_ODBC_H
/** 包含文件 **/
#include <windef.h>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

/** 简单的宏,用于判断ODBCAPI的执行结果是否正确 **/
#define isSuc(result) ((result) == SQL_SUCCESS || (result) == SQL_SUCCESS_WITH_INFO)

/** 可扩展设置 **/
char szConnectString[256] =
		"driver={Microsoft Access Driver (*.mdb)};DBQ=E:" "ODBC.MDB";
char szSQL[256] = "select * from `test`";
/** 输出缓存 **/
char szDriverOutput[256];
short sDriverOutputLength;
/** 用于测试的变量 **/
int a = 0;
char b[255];
char c[255];
long temp = 0;

/** 用于测试的函数 **/
int odbc() {
	short sret; //返回代码
	void* henv; //环境句柄
	void* hdbc; //连接句柄
	long mode;//提交模式
	void* hsmt; //语句句柄
	/** 申请环境句柄 **/
	sret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (!isSuc(sret))
		printf("申请环境句柄出错\n");
	/** 设置环境属性,声明ODBC版本 **/
	sret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3,
			SQL_IS_INTEGER);
	if (!isSuc(sret))
		printf("声明ODBC版本出错\n");
	/** 申请连接句柄 **/
	sret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	if (!isSuc(sret))
		printf("申请连接句柄出错\n");
	/** 设置连接属性 **/
	sret = SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT,
			(void*) SQL_AUTOCOMMIT_OFF, SQL_IS_INTEGER);
	if (!isSuc(sret))
		printf("设置连接属性出错\n");
	/** 取得连接属性 **/
	sret = SQLGetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (void*) &mode,
			sizeof(mode) + 1, NULL);
	if (!isSuc(sret))
		printf("取得连接属性出错\n");
	/** 连接数据源 **/
	//	sret = SQLDriverConnect(hdbc, NULL, (unsigned char *) szConnectString,
	//			SQL_NTS, (unsigned char *) szDriverOutput, 256,
	//			&sDriverOutputLength, SQL_DRIVER_NOPROMPT);
	sret = SQLDriverConnect(hdbc, NULL, (unsigned char *)"DSN=test;UID=root;PASSWD=root", SQL_NTS,
			(unsigned char *) szDriverOutput, 256, &sDriverOutputLength,
			SQL_DRIVER_COMPLETE);
	if (!isSuc(sret))
		printf("连接数据源出错\n");
	/** 分配语句句柄 **/
	sret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hsmt);
	if (!isSuc(sret))
		printf("分配语句句柄出错\n");
	/** 执行语句 **/
	sret = SQLExecDirect(hsmt, (SQLCHAR *) szSQL, SQL_NTS);
	if (!isSuc(sret))
		printf("执行语句出错");
	/** 绑定结果集和获得数据 **/
	/**
	 小提示，这里应该可以先取得字段列表然后用循环做成自动邦定的。
	 **/
	SQLBindCol(hsmt, 1, SQL_C_LONG, &a, sizeof(a), &temp);//测试时
	SQLBindCol(hsmt, 2, SQL_C_CHAR, &b, sizeof(b), &temp);//测试时
	SQLBindCol(hsmt, 3, SQL_C_CHAR, &c, sizeof(c), &temp);//测试时
	/** 取得数据 **/
	while (1) {
		/** 滚动 **/
		sret = SQLFetchScroll(hsmt, SQL_FETCH_NEXT, 0);
		/**
		 前面如果发生错误,这里很可能得不到SQL_NO_DATA_FOUND而发生死循环，
		 所以我不用而用SQL_SUCCESS来判断，但这句在杂复的程序中里依然有用。
		 **/
		//if (sret == SQL_NO_DATA_FOUND) break; //如果没有数据就打断。
		if (sret != SQL_SUCCESS)
			break;
		printf("a = %d\n", a);//测试时
		printf("b = %s\n", b);//测试时
		printf("c = %s\n", c);//测试时
	}
	/** 释放语句句柄 **/
	SQLFreeHandle(SQL_HANDLE_STMT, hsmt);
	/** 提交连接的事务 **/
	SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
	/** 断开与数据源的连接 **/
	SQLDisconnect(hdbc);
	/** 释放连接句柄 **/
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	/** 释放环境句柄 **/
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
	/////////无关语句，仅仅是让程序停下来，看结果。
	//getchar();//测试时
	return 0;
}

#endif
