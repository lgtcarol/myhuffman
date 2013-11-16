/*
 * =====================================================================================
 *
 *       Filename:  decompress.c
 *
 *    Description:  解压缩函数
 *
 *        Version:  1.0
 *        Created:  2013年11月10日 17时10分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lgt, lgtcarol@gmail.com
 *        Company:  Class 1203
 *
 * =====================================================================================
 */
#include "huffman.h"
void DeCompressFile(char inpt_file[], char out_file[])
{
	FILE 		*fp_inpt, *fp_out, *fp_tmp, *fp_map;
	HeadInfo 	head;
	int 		ch; 			//这个数据类型起到关键作用
	char 		old_str[GROUP_SIZE+1];
	char 		tmp_file[PATH_MAX];
	int 		i, p;
	long 		ch_num;
	
	//将压缩文件初步解压为二进制
	strcpy(tmp_file, inpt_file);
	strcat(tmp_file, ".tmp");
	if((fp_tmp = fopen(tmp_file, "wb+")) == NULL)
	      OpenFileError(inpt_file, __LINE__);
	if((fp_inpt = fopen(inpt_file, "rb")) == NULL)
	      OpenFileError(inpt_file, __LINE__);
	fread(&head, sizeof(HeadInfo), 1, fp_inpt);
	fseek(fp_inpt, 0, SEEK_END);
	//预处理补零的部分
	ch_num = ftell(fp_inpt) - sizeof(HeadInfo); 		//文件中总共的字符数
	fseek(fp_inpt, sizeof(HeadInfo), SEEK_SET); 		//将文件指针移到第一个有效字符
	 for(i = 1; i < ch_num+(head.add_zero == 0); i++)  //翻译除去最后一个字符的其他
	  {
		ch = fgetc(fp_inpt);
		ChToStr(ch, old_str);
		fprintf(fp_tmp, "%s", old_str);
	}
	if(head.add_zero != 0){ 
	ch = fgetc(fp_inpt);
	ChToStr(ch, old_str); 							//处理补0的部分
//	printf("最后的一个串是: %s\n", old_str);
	old_str[GROUP_SIZE-head.add_zero] = '\0';
//	printf("截取后的字符串: %s\n", old_str);
	fprintf(fp_tmp, "%s", old_str); 					//将最后一个字符的真实串写入
	}
	fseek(fp_tmp, 0, SEEK_SET);
	//读取字母表
	if((fp_map = fopen("maptree", "rb")) == NULL)
	      OpenFileError("maptree", __LINE__);
	fread(g_ht, sizeof(HUNode), NODE_NUM + 1, fp_map);
	fclose(fp_map);
	//开始正式差表解压
	if((fp_out = fopen(out_file, "wb+")) == NULL)
	      OpenFileError(out_file, __LINE__);
	for(i = CHAR_NUM+1; g_ht[i].parent != 0 && i <= NODE_NUM; i++)  ;
	p = i; 				 					//找到根节点
	printf("root: %d\n", p);
	while((ch = fgetc(fp_tmp)) != EOF)
	{
		if(ch == '0')
		      p = g_ht[p].Lchild;
		if(ch == '1')
		      p = g_ht[p].Rchild;

		if(g_ht[p].weight != 0 && g_ht[p].Lchild == 0 && g_ht[p].Rchild == 0)
		{
			fputc(p-1, fp_out);
//			printf("一个字符恢复：%c\n", p-1);
			p = i;
		}

	}
	fclose(fp_inpt);
	fclose(fp_tmp);
	remove(tmp_file);
	fclose(fp_out);
}

void ChToStr(int ch, char tmp_str[])
{
	int 		i;

	for(i = 0; i < GROUP_SIZE; i++)
	      tmp_str[i] = '0';
	for(i = GROUP_SIZE-1; ch != 0 && i >= 0; i--)
	{
		tmp_str[i] = ch%2 + '0';
		ch /= 2;
	}
	tmp_str[GROUP_SIZE] = '\0';
}
