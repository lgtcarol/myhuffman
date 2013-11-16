/*
 * =====================================================================================
 *
 *       Filename:  huffman.c
 *
 *    Description:  哈夫曼的主函数及一些辅助函数
 *
 *        Version:  1.0
 *        Created:  2013年11月11日 14时09分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lgt, lgtcarol@gmail.com
 *        Company:  Class 1203
 *
 * =====================================================================================
 */

#include "huffman.h"
//#define DEBUG 
int main(int argc, char *argv[])
{
	clock_t 	deal_time;
	
	deal_time = clock();
	if(argc == 3 && strcmp(argv[1], "-p") == 0)
	      PrintCmprssInfo(argv[2]);
	else if(argc == 4)
	{
		if(strcmp(argv[1], "-c") == 0)
		{
			CompressFile(argv[2], argv[3]);
			printf("压缩用时: %.5lf\n", (double)(clock()-deal_time)/CLOCKS_PER_SEC);
		}
		if(strcmp(argv[1], "-d") == 0)
		{
			DeCompressFile(argv[2], argv[3]);
			printf("压缩用时: %.5lf\n", (double)(clock()-deal_time)/CLOCKS_PER_SEC);
		}
		
	}
	else
	      PrintUsage();

	return 0;
}

void PrintUsage(void)
{
	puts("\n程序说明：一个使用 Huffman 算法进行单文件压缩的小程序\n");
	puts("参数：\n\t-c : 压缩文件\n\t-d : 解压缩文件\n\n\t-p : 查看压缩文件信息\n");
}

void PrintCmprssInfo(char inpt_file[])
{
	FILE  		*fp;
	HeadInfo 	head;

	if((fp = fopen(inpt_file, "rb")) == NULL)
	      OpenFileError(inpt_file, __LINE__);
	fread(&head,  sizeof(HeadInfo), 1, fp);
	printf("源文件大小: %d\n", head.old_bytes);
	fclose(fp);
}

void OpenFileError(char inpt_file[], int line)
{
	fprintf(stderr, "%d  ", line);
	perror(inpt_file);
	exit(1);
}

void CrtHuffmanTree(int w[], int leaf_num)
{
	int 		i, s1, s2;
	int 		remain = 1;
	FILE 		*fp;

	if((fp = fopen("maptree", "wt")) == NULL)
		OpenFileError("maptree", __LINE__);
	for(i = 1; i <= CHAR_NUM; i++)
	{
		g_ht[i].weight = w[i];
		g_ht[i].parent = g_ht[i].Lchild = g_ht[i].Rchild = 0;
	}
	for(i = CHAR_NUM+1; i <= NODE_NUM; i++)
	{
		g_ht[i].weight = 0;g_ht[i].parent = 0;g_ht[i].Lchild = 0;
	        g_ht[i].Rchild = 0;
	}
	for(i = CHAR_NUM+1; i <= CHAR_NUM+leaf_num-1; i++)
	{
		Select(i-1, &s1, &s2);
		if(remain == 0)
		      break;
		g_ht[i].weight = g_ht[s1].weight + g_ht[s2].weight;
//		printf("remain: %d 建立的节点：%d  weight: %d\n", remain, i, g_ht[i].weight);
		g_ht[i].Lchild = s1;
		g_ht[i].Rchild = s2;
		g_ht[s1].parent = g_ht[s2].parent = i;
	}

//	printf("alphabet:\n");
//	for(i = 1; i <= CHAR_NUM+leaf_num-1; i++)
//	      if(g_ht[i].weight != 0)
//	      	printf("index: %d  weight: %d parent: %d Lchild: %d Rchild: %d\n", i, g_ht[i].weight, g_ht[i].parent, g_ht[i].Lchild, g_ht[i].Rchild);



//	fwrite(g_ht, sizeof(HUNode), CHAR_NUM+leaf_num-1, fp);
	fwrite(g_ht, sizeof(HUNode), NODE_NUM+1, fp);
	fclose(fp);
}

void Select(int n, int *s1, int *s2)
{
	int 	i, min1 = INT_MAX, min2 = INT_MAX;
	//int 	flag = 0;

	for(i = 1; i <= n; i++)
	{
		if(g_ht[i].weight != 0 && g_ht[i].parent == 0 )
		{
//			printf("select中下标为%d符合，权值：%d\n", i, g_ht[i].weight);
			if(min1 > g_ht[i].weight)
			{
				min2 = min1;
				min1 = g_ht[i].weight;
				*s2 = *s1;
				*s1 = i;
			}
			else if(min2 > g_ht[i].weight)
			{
				min2 = g_ht[i].weight;
				*s2 = i;
			}
		}
//		flag = 0;
	}
//	printf("select index: %d %d\n", *s1, *s2);
}

void CrtHuffmanCode(int w[])
{
	char 		*cd;
	int 		start, c, p, i;
	cd = (char *)malloc(CHAR_NUM * sizeof(char));
	cd[CHAR_NUM-1] = '\0';
//	memset(cd, NULL, CHAR_NUM);
	for(i = 1; i <= CHAR_NUM; i++)
	{
		if(g_ht[i].weight != 0)
		{
			start = CHAR_NUM-1;
			c = i; 
			p = g_ht[i].parent;
			while(p != 0)
			{
				--start;
				if(g_ht[p].Lchild == c)
			      		cd[start] = '0';
				else
			      		cd[start] = '1';
				c = p;
				p = g_ht[p].parent;
			}
//			assert(start >= 0); 		//编码长度大于CHAR_NUM
//		if(strlen(cd) != 0)
//		{
			g_code_str[i] = (char *)malloc((CHAR_NUM - start) * sizeof(char));
			strcpy(g_code_str[i], &cd[start]);
//		}
	    	}
	}
#ifdef DEBUG
	printf("codestr:\n");
	for(i=1; i <= CHAR_NUM; i++)
	      if(g_code_str[i] != NULL)
		    printf("%c : %s\n", i, g_code_str[i]);
#endif
}































