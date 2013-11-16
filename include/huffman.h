/*
 * =====================================================================================
 *
 *       Filename:  huffman.h
 *
 *    Description:  哈夫曼压缩解压的头文件
 *
 *        Version:  1.0
 *        Created:  2013年11月10日 10时21分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lgt, lgtcarol@gmail.com
 *        Company:  Class 1203
 *
 * =====================================================================================
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>

#define CHAR_NUM 	256
#define NODE_NUM 	CHAR_NUM * 2 -1
#define GROUP_SIZE 	8

typedef struct
{
	int 	weight;
	int 	parent;
	int 	Lchild;
	int     Rchild;
}HUNode, HuffmanTree[NODE_NUM + 1]; 				//0号单元不用
typedef struct
{
//	char 	old_file[PATH_MAX];
	int 	old_bytes;
	int 	add_zero;
}HeadInfo;

HuffmanTree 	g_ht;
char* 		g_code_str[CHAR_NUM + 1] ; 			//0号单元不用,其实自己会被默认初始化为NULL

void CompressFile(char inpt_file[], char out_file[]); 				//文件压缩
void DeCompressFile(char inpt_file[], char out_file[]); 				//文件解压
void PrintCmprssInfo(char inpt_file[]);
void PrintUsage(void); 

void CrtHuffmanTree(int w[], int num);
void CrtHuffmanCode(int w[]);
void  Select(int n, int *s1, int *s2);

int StrToCh(char code_str[]);
void ChToStr(int num, char str[]);
void OpenFileError(char file_name[], int line);
void FreeSpace(void);
