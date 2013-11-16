/*
 * =====================================================================================
 *
 *       Filename:  compress.c
 *
 *    Description:  压缩函数
 *
 *        Version:  1.0
 *        Created:  2013年11月10日 11时19分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lgt, lgtcarol@gmail.com
 *        Company:  Class 1203
 *
 * =====================================================================================
 */

#include "huffman.h"
//#define COMBUG
void CompressFile(char inpt_file[], char out_file[])
{
	FILE 	*fp_inpt, *fp_out, *fp_tmp;
	int 	w[CHAR_NUM + 1] = {0}; 				//0号单元不用，故而字符+1后与下标对应
	int 	to_ch, i, leaf_num = 0;
	char 	tmp_file[PATH_MAX];
	char 	tmp_code_str[GROUP_SIZE + 1];
	int    ch;
	int 	count_zero = 0, count_bytes = 0, group_num, group_remain;
	int 	sum = 0;
	HeadInfo head;

	//获取权值
	if((fp_inpt = fopen(inpt_file, "rb")) == NULL)
	      OpenFileError(inpt_file, __LINE__);
	while((ch = fgetc(fp_inpt)) != EOF)
	{
			w[ch+1]++;
		count_bytes++;
	}
	//获取出现的字符个数
	for(i = 1; i <= CHAR_NUM; i++)
	      if(w[i] != 0)
		    leaf_num++;
#ifdef COMBUG
	printf("value:\n");
	for(i = 1; i <= CHAR_NUM; i++)
	      if(w[i] != 0)
		    printf("char: %c   weight: %d\n", i-1, w[i]);
			sum+= w[i];
	printf("sum: %d\n", sum);
#endif
//	strcpy(head.old_file, inpt_file);
	head.old_bytes = count_bytes;
	//建立哈夫曼树，并完成字符编码
	CrtHuffmanTree(w, leaf_num);
	CrtHuffmanCode(w);
	//借助.tmp将源文件转为其哈夫曼编码
	strcpy(tmp_file, inpt_file);
	strcat(tmp_file, ".tmp");
	fseek(fp_inpt, 0, 0);
	if((fp_tmp = fopen(tmp_file, "wb+")) == NULL)
	      OpenFileError(tmp_file, __LINE__);
	while((ch = fgetc(fp_inpt)) != EOF)
	{
	      fwrite(g_code_str[ch+1], sizeof(char),(strlen(g_code_str[ch+1])), fp_tmp);
	}
	fclose(fp_inpt);
	int tmp_file_len = ftell(fp_tmp);
	group_num = tmp_file_len/GROUP_SIZE;
	group_remain = tmp_file_len%GROUP_SIZE;
//	printf("每8个一组后的余数; %d\n", group_remain);
	rewind(fp_tmp);
	//建立压缩文件
	if((fp_out = fopen(out_file, "wb")) == NULL)
	      OpenFileError(out_file, __LINE__);
	fwrite(&head, sizeof(HeadInfo), 1, fp_out);

	for(i = 0; i < group_num; i++)
	{
		fread(tmp_code_str, sizeof(char), GROUP_SIZE, fp_tmp);
		tmp_code_str[GROUP_SIZE] = '\0';
		to_ch = StrToCh(tmp_code_str);
		fputc(to_ch, fp_out);
	}
	fscanf(fp_tmp, "%s", tmp_code_str);
//	printf("不够8的那几个字符是：%s\n", tmp_code_str);
	while(group_remain != 0 && group_remain < 8)
	{
		tmp_code_str[group_remain] = '0';
		count_zero++;
		group_remain++;
	}
	head.add_zero = count_zero;
	if(count_zero != 0){
	tmp_code_str[group_remain] = '\0';
	printf("du qu after zero %s\n", tmp_code_str);
	to_ch = StrToCh(tmp_code_str);
	fputc(to_ch, fp_out);
	}
	//完善压缩文件并处理善后工作
//	printf("压缩文件：%s已生成， 压缩率是：%.3lf %%\n", （double）(ftell(fp_out)/head.old_btes）);
	fseek(fp_out, 0, SEEK_SET);
	fwrite(&head, sizeof(HeadInfo), 1, fp_out);
	fseek(fp_out, 0, SEEK_END);
	printf("压缩文件：%s 已生成\n", out_file);
	fclose(fp_tmp);
	remove(tmp_file);
	fclose(fp_out);
	FreeSpace();
}

void FreeSpace(void)
{
	int 		i;
	for(i = 1; i <= CHAR_NUM; i++)
	      if(g_code_str[i] != NULL)
		    free(g_code_str[i]);
}

int StrToCh(char code_str[])
{
	int 	i, sum = 0;
	for(i = 0; i < GROUP_SIZE; i++)
	      sum = sum*2 + (code_str[i] - '0');
	return sum;
}
