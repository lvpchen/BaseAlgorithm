#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <unistd.h>
using namespace std;
#define MAX_INPUT_NUM 100
#define MAX_OUT_NUM 100
#define MAX_DATA_NUM 100
/*********      插入排序      **********/
void straight_insert_sort(int* output_s, int n);              // 直接插入排序
void shell_sort(int* output_s, int n);                        // 希尔排序（改良的直接插入排序，又称缩小增量排序）
void bin_insert_sort(int* output_s, int n);                   // 二分插入排序
/*********      选择排序      **********/
void selection_sort(int* output_s, int n);                    // 选择排序
void heap_sort(int* output_s, int n);                         // 堆排序（改良的选择排序）
void heap_adjust(int *input, int i, int n);                   // 堆排序-堆调整函数
/*********      交换排序      **********/
void bubble_sort(int* output_s, int n);                       // 冒泡排序（交换排序）
void better_bubble_sort(int* output_s, int n);                // 改良的冒泡排序（交换排序）
void quick_sort(int* output_s, int l, int r, int n);          // 快速排序
int partition(int * output_s, int l, int r, int n);           // 快速排序-分治法
/*********      归并排序      **********/
void merge_sort(int* output_s, int l, int r, int n);          // 归并排序
void merge_array(int * input, int l, int m, int r, int n);    // 归并排序-合并数组
/*********      基数排序      **********/
void radix_sort(int* output_s, int n);                        // 基数排序
int getdigit(int data, int x);                                // 基数排序-获取数据的指定位数字
int getdistance(int * input, int n);                          // 基数排序-获取关键码
/*********      基数排序      **********/
void print_answer(int* output_p, int n);                      // 结果打印函数
void swap(int *s, int *t);                                    // 交换函数
int change_to_num(char* a, int m);                            // 字符串转换成整形数据

static int step = 0;
int main()
{
	string str;
	int i = 0, j = 0;
	int sort_num = 0;
	int count_c = 0;                 // 输入的有效字符个数
	int count_n = 0;                 // 输入的每个数据位数
	int count = 0;                   // 数据个数
	char temp[MAX_DATA_NUM];
	int input[MAX_INPUT_NUM];
	int output[MAX_OUT_NUM];
	cout << "\n                Kind of sort                 " << endl;
	cout << "---------------------------------------------" << endl;
	cout << "**        1.  Straight Insert Sort         **" << endl;
	cout << "**        2.  Shell Sort                   **" << endl;
	cout << "**        3.  Binary Insertong Sort        **" << endl;
	cout << "**        4.  Bubble Sort                  **" << endl;
	cout << "**        5.  Better Bubble Sort           **" << endl;
	cout << "**        6.  Selection Sort               **" << endl;
	cout << "**        7.  Quick Sort                   **" << endl;
	cout << "**        8.  Heap Sort                    **" << endl;
	cout << "**        9.  Merge Sort                   **" << endl;
	cout << "**        10. Radix Sort                   **" << endl;
	cout << "---------------------------------------------" << endl;
	cout << "\nPlease input the data you want to sort, and separated by spaces\n" << endl;
	while (getline(cin, str, '\n'))
	{
		j = 0;
		count = 0;
		count_c = 0;
		count_n = 0;
		memset(temp, 0, sizeof(temp));
		memset(input, 0, sizeof(input));
		memset(output, 0, sizeof(output));
		if(0 == str.size())             // 直接回车，没输入任何字符
			continue;
		for (i = 0; i < str.size(); i++)
		{
			if (isdigit(str[i]) || isspace(str[i]))
			{
				temp[i] = str[i];
				count_c++;
			}
		}
		if(0 == count_c)                // 没有输入任何有效字符数据
		{
			cout << "There are none of any effective data input, please input again：" << endl;
			continue;
		}
		while(isspace(str[i - 1]))         // 避免最后一个字节为空格的产生数据 0 的bug
		{
			i--;
			count_c--;
		}
		while(isspace(temp[j]))         // 避免最前面几个字符为空格产生数据 0 的bug
			j++;
		for (i = j; i <= count_c; i++)  // 转换成数据
		{
			if(!isdigit(temp[i]) || temp[i] == '\0')  // 遇到空格或者结束符，表示获取了一个数据的字符串，进行转换
			{
				input[count] = change_to_num(&temp[i - count_n], count_n);
				count_n = 0;
				count++;
				while(isspace(temp[i + 1]))          // 排序中间多个空格产生 0 的bug
					i++;
			}
			else
				count_n++;      // 一个数据的位宽
		}
		while(1)
		{
			cout << "\nPlease select kind of sort of 1 - 10" << endl;
			cout << "(If you want new sort data, please input 0)" << endl;
			cin >> sort_num;
			if(sort_num < 0 || sort_num > 10)
			{
				cout << "\nThe last num " << sort_num << " is false, ";
				cout << "it should more than 0 and not more than 10" << endl;
				continue;
			}
			else if(0 == sort_num)
			{
				cout << "\nPlease input the data you want to sort, and separated by spaces\n" << endl;
				break;
			}
			step = 0;                              // 打印步骤初始为0
			for(i = 0; i < count; i++)             // 拷贝给输出
			{
				output[i] = input[i];
			}
			print_answer(output, count);             // 打印输入数据
			switch (sort_num)
			{
				case 1:
					cout << "**        1.  Straight Insert Sort         **" << endl;
					straight_insert_sort(output, count);
					break;
				case 2:
					cout << "**        2.  Shell Sort                   **" << endl;
					shell_sort(output, count);
					break;
				case 3:
					cout << "**        3.  Binary Insertong Sort        **" << endl;
					bin_insert_sort(output, count);
					break;
				case 4:
					cout << "**        4.  Bubble Sort                  **" << endl;
					bubble_sort(output, count);
					break;
				case 5:
					cout << "**        5.  Better Bubble Sort           **" << endl;
					better_bubble_sort(output, count);
					break;
				case 6:
					cout << "**        6.  Selection Sort               **" << endl;
					selection_sort(output, count);
					break;
				case 7:
					cout << "**        7.  Quick Sort                   **" << endl;
					quick_sort(output, 0, count - 1, count);            // 最后一个参数打印用
					break;
				case 8:
					cout << "**        8.  Heap Sort                    **" << endl;
					heap_sort(output, count);
					break;
				case 9:
					cout << "**        9.  Merge Sort                   **" << endl;
					merge_sort(output, 0, count - 1, count);            // 最后一个参数打印用
					break;
				case 10:
					cout << "**        10. Radix Sort                   **\n" << endl;
					radix_sort(output, count);
					break;
				default:
					break;
			}
		}
	}
	return 0;
}
/**********************************************************************
 * Function: 直接插入排序函数
 * 最坏时间复杂度：O(n^2)
 * 平均时间复杂度：O(n^2)
 * 最优时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 稳定性：稳定
 * Author: Moshui
 **********************************************************************/
void straight_insert_sort(int *output_s, int n)
{
	int i = 0, j = 0, k = 0;
	int temp = 0;                // 哨兵
	for(i = 1; i < n; i++)
	{
		if(output_s[i] < output_s[i - 1])    // 如果后面一个比前面小
		{
			temp = output_s[i];          // 赋值给哨兵
			for(j = i - 1; j >= 0 && output_s[j] > temp; j--)  // 前面 i 个数据大的往后挪
			{
				output_s[j + 1] = output_s[j];
			}
			output_s[j + 1] = temp;      // 哨兵的值赋值给最后一个往后挪的数据的位置
		}
		print_answer(output_s, n);           // 打印每一步排序结果
	}
}

/**********************************************************************
 * Function: 希尔排序函数
 * 最坏时间复杂度：
 * 平均时间复杂度：O(n^1.25)
 * 最优时间复杂度：
 * 空间复杂度：O(1)
 * 稳定性：不稳定
 * Author: Moshui
 **********************************************************************/
void shell_sort(int * output_s, int n)
{
	int i = 0, j = 0, k = 0;
	int temp = 0;                // 哨兵
	int gap = 0;
	for(gap = n / 2; gap > 0; gap /= 2)                           // 数据分组
	{
#if 0           // 便于理解的代码
		for(i = 0; i < gap; i++)                              // 分成gap组进行直接插入排序
		{
			for(j = i + gap; j < n; j += gap)             // 对每一组进行直接插入排序，每一组的步长是gap
			{
				if(output_s[j] < output_s[j - gap])
				{
					temp = output_s[j];
					for(k = j - gap; k >= 0 && output_s[k] > temp; k -= gap)  // 前面数据大的往后挪
					{
						output_s[k + gap] = output_s[k];
					}
					output_s[k + gap] = temp;      // 哨兵的值赋值给最后一个往后挪的数据的位置	
				}
			}
		}
#else           // 简洁的代码
		for(i = gap; i < n; i++)         // 各组穿插进行直接插入排序
		{
			for(j = i - gap; j >= 0 && output_s[j] > output_s[j + gap]; j -= gap)
			{
				swap(output_s + j, output_s + j + gap);
			}
		}
#endif
		print_answer(output_s, n);       // 打印每一步排序结果
	}
}

/**********************************************************************
 * Function: 二分插入排序函数
 * 最坏时间复杂度：O(n^2)
 * 平均时间复杂度：O(n^2)
 * 最优时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 稳定性：稳定
 * Author: Moshui
 **********************************************************************/
void bin_insert_sort(int * output_s, int n)
{
	int i = 0, j = 0;
	int temp = 0;
	int l = 0, m = 0, r = 0;
	for(i = 1; i < n; i++)                           // i 从 1 开始，因为第一个数据直接插入
	{
		temp = output_s[i];                      // 临时值
		l = 0;             
		r = i;
		while(l <= r)
		{
			m = (r + l) / 2;                 // 取中间位值
			if(temp > output_s[m])           // 此处不可加 "=" 号
				l = m + 1;
			else
				r = m - 1;
		}
		for(j = i - 1; j >= l; j--)
			output_s[j + 1] = output_s[j];   // 往后挪，腾出 l 位置
		output_s[l] = temp;                      // 插入到这个位置
		print_answer(output_s, n);               // 打印每一步排序结果
	}
}

/**********************************************************************
 * Function: 冒泡排序函数
 * 最坏时间复杂度：O(n^2)
 * 平均时间复杂度：O(n^2)
 * 最优时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 稳定性：稳定
 * Author: Moshui
 **********************************************************************/
void bubble_sort(int * output_s, int n)
{
	int i = 0, j = 0, k = 0;
	int temp = 0;
	for(i = 0; i < n - 1; i++)             // i 不需要等于 n - 1
	{
		for(j = n - 1; j > i; j--)                   // 前面已经排序好的不需要比较
		{
			if(output_s[j - 1] > output_s[j])    // 如果后面一个比前面小
			{
				swap(output_s + j, output_s + j - 1);
			}
		}
		print_answer(output_s, n);     // 打印每一步排序结果
	}
}

/**********************************************************************
 * Function: 改进的冒泡排序函数
 * 最坏时间复杂度：O(n^2)
 * 平均时间复杂度：O(n^2)
 * 最优时间复杂度：O(n)
 * 空间复杂度：O(1)
 * 稳定性：稳定
 * Author: Moshui
 **********************************************************************/
void better_bubble_sort(int * output_s, int n)
{
	int i = 0, j = 0, k = 0;
	int temp = 0;                          // 哨兵
	int last_pos = 0;                      // 记录最近一次交换的位置
	int last_pos_t = 0;                    // 记录最近一次交换的位置
	for(i = 0; i < n - 1; i++)             // i 不需要等于 n - 1
	{
		last_pos = last_pos_t;
		for(j = n - 1; j > last_pos; j--)            // last_pos前面已经排序好的不需要比较
		{
			if(output_s[j - 1] > output_s[j])    // 如果后面一个比前面小
			{
				swap(output_s + j, output_s + j - 1);
				last_pos_t = j;
			}
		}
		print_answer(output_s, n);     // 打印每一步排序结果
		if(last_pos == last_pos_t)     // 如果本次循环未发生交换，说明已经有序
			break;
	}
}

/**********************************************************************
 * Function: 选择排序函数
 * 最坏时间复杂度：O(n^2)
 * 平均时间复杂度：O(n^2)
 * 最优时间复杂度：O(n^2)
 * 空间复杂度：O(1)
 * 稳定性：不稳定
 * Author: Moshui
 **********************************************************************/
void selection_sort(int * output_s, int n)
{
	int i = 0, j = 0, k = 0;
	int min = 0;
	int temp = 0;
	for(i = 0; i < n; i++)
	{
		min = i;                         // 记录最小值的下标
		for(j = i + 1; j < n; j++)
		{
			if(output_s[min] > output_s[j])
				min = j;         // 记录最小值的下标
		}
		if(min != i)                     // 剩余数据里面有更小的数据，交换数据
		{
			swap(output_s + min, output_s + i);
		}
		print_answer(output_s, n);       // 打印每一步排序结果
	}
}

/**********************************************************************
 * Function: 快速排序函数
 * 最坏时间复杂度：O(n^2)     待排序的为正序或者逆序的时候，每次划分只少掉一个记录
 * 平均时间复杂度：O(nlogn)
 * 最优时间复杂度：O(nlogn)
 * 空间复杂度：O(logn)        递归的深度
 * 稳定性：不稳定
 * Author: Moshui
 **********************************************************************/
void quick_sort(int * output_s, int l, int r, int n)
{
	int m = 0;
	if(l < r)
	{
		m = partition(output_s, l ,r, n);
		quick_sort(output_s, l, m - 1, n);
		quick_sort(output_s, m + 1, r, n);
	}
}
/**********************************************************************
 * Function: 分治函数
 * Input：总排序数据，排序数据起始下标，排序数据结束下标
 * Information：将数据分为A[l,m-1],A[m+1,r]，
 * 		使左边数组小于A[m]，右边数组大于A[m]。
 * Author: Moshui
 **********************************************************************/
int partition(int * output_s, int l, int r, int n)
{
	int i = 0, j = 0;
	int x = 0, temp = 0;
	x = output_s[r];                           // 最后一个值，用于比较
	i = l - 1;                                 // 先把 i 移出比较数据外 
	for(j = l; j <= r - 1; j++)                // 对当前数据（l，r）分组
	{
		if(output_s[j] <= x)               // 小于 x 的在左
		{
			i++;                       // x 的正确位置加 1
			swap(output_s + i, output_s + j);
		}
	}
	swap(output_s + i + 1, output_s + r);      // 确定x的正确位置 i+1
	print_answer(output_s, n);                 // 打印每一步排序结果
	return i + 1;
}
/**********************************************************************
 * Function: 堆排序函数
 * 最坏时间复杂度：O(nlogn)
 * 平均时间复杂度：O(nlogn)
 * 最优时间复杂度：O(nlogn)
 * 空间复杂度：O(1)
 * 稳定性：不稳定
 * Author: Moshui
 **********************************************************************/
void heap_sort(int * output_s, int n)
{
	int i = 0, j = 0, k = 0;
	int min = 0;
	int temp = 0;
	for(i = n / 2 - 1; i >= 0; i--)        // 建堆,建堆完成后第一个数据最大（或最小）
	{
		heap_adjust(output_s, i, n);
	}
	print_answer(output_s, n);             // 打印每一步排序结果
	for(i = n - 1; i >= 0; i--)   
	{
		swap(output_s, output_s + i);  // 交换堆顶和最后一个元素，即每次将剩余元素中最大者放到最后面
		heap_adjust(output_s, 0, i);   // 对剩余元素重新调整成大顶堆（或小顶堆）
		print_answer(output_s, n);     // 打印每一步排序结果
	}
}
/**********************************************************************
 * Function: 堆调整函数
 * Input：排序数据，最后一个非叶节点下标，数据长度
 * Author: Moshui
 **********************************************************************/
void heap_adjust(int * input, int i, int n)
{
	int lchild = 2 * i + 1;                // 左孩子下标
	int rchild = 2 * i + 2;                // 右孩子下标
	int max = i;                           // 临时下标变量
	if(i < n / 2)
	{
		if(lchild < n && input[lchild] > input[max])  // 大顶堆  
			max = lchild;
		if(rchild < n && input[rchild] > input[max])  // 大顶堆
			max = rchild;
		if(max != i)
		{
			swap(input + max, input + i);         // 交换子节点和父节点
			heap_adjust(input, max, n);           // 避免调整之后以max为父节点的子树不是堆
		}
	}
}

/**********************************************************************
 * Function: 归并排序函数
 * 最坏时间复杂度：O(nlogn)
 * 平均时间复杂度：O(nlogn)
 * 最优时间复杂度：O(nlogn)
 * 空间复杂度：O(n)
 * 稳定性：稳定
 * Author: Moshui
 **********************************************************************/
void merge_sort(int * output_s, int l, int r, int n)
{
	if(l < r)
	{
		int m = l + (r - l) / 2;               // 效果同 (l + r)/2，避免 l+r 过大溢出
		merge_sort(output_s, l, m, n);         // 递归调用，先拆左边，拆至左边只有 1 个数
		merge_sort(output_s, m + 1, r, n);     // 递归调用，一点点将右边合并
		merge_array(output_s, l, m, r, n);     // 合并有序数组[l,m]，[m+1,r]
	}
}
/**********************************************************************
 * Function: 合并有序序列函数
 * Input：排序数据，左边下标，中间下标，右边下标，
 * 	  以这3个下标将其分成2个数组
 * Author: Moshui
 **********************************************************************/
void merge_array(int * input, int l, int m, int r, int n)
{
	int i = l, j = m + 1, k = 0;
	int temp[r - l + 1];                          // 临时数组
	while(i <= m && j <=r)
	{
		if(input[i] <= input[j])              // 把小的放到临时数据中
			temp[k++] = input[i++];
		else
			temp[k++] = input[j++];
	}
	while(i <= m)                                 // 剩余的直接拷贝，i <= m 或者 j <= r
		temp[k++] = input[i++];
	while(j <= r)
		temp[k++] = input[j++];
	for(i = 0; i < k; i++)                        // 赋值给排序数组
		input[l + i] = temp[i];
	print_answer(input, n);                       // 打印每一步排序结果
}

/**********************************************************************
 * Function: 基数排序函数
 * 最坏时间复杂度：O(d(r+n))                d:关键码 r:基数 n:数据个数
 * 平均时间复杂度：O(d(r+n))        例如：      d      r      n
 * 最优时间复杂度：O(d(r+n))        1 23 123    3      10     3
 * 空间复杂度：O(r+n)               a wqvd      4      26     2
 * 稳定性：稳定
 * Author: Moshui
 **********************************************************************/
void radix_sort(int * output_s, int n)
{
	int i = 0, j = 0, k = 0;
	int distance = 0;                        // 关键码
	const int radix = 10;                    // 数字排序的基数为10，申明为固定变量
	int count[radix];                        // 统计各个桶所盛数据个数
	int bucket[n];                           // 所有桶的空间开辟总数
	distance = getdistance(output_s, n);     // 获取关键码
	for(k = 1; k <= distance; k++)           // distance 次分配操作
	{
		for(i = 0; i < radix; i++)          // 清空，为位数不够的数据进行补 0 操作
			count[i] = 0;
		for(i = 0; i < n; i++)              // 统计每次分配后各个桶所需要的存储空间大小
			count[getdigit(output_s[i],k)]++;
		for(i = 1; i < radix; i++)          // count[i]表示收集每个桶后 bucket[] 所存有的上边界
			count[i] += count[i - 1];   // 作用是避免覆盖，此步骤可用于后续放桶和收集合二为一 
		for(i = n - 1; i >= 0; i--)      
		{
			j = getdigit(output_s[i], k);        // 求出关键字的第 k 位数字
			bucket[count[j] - 1] = output_s[i];  // 放入桶中，并收集
			count[j]--;                          // 收集好一个数后相应桶中的个数减 1
		}
		for(i = 0; i < n; i++)
			output_s[i] = bucket[i];
		print_answer(output_s, n);                   // 打印每一步排序结果
	}
}


/**********************************************************************
 * Function: 取数据各位数字函数
 * Input：数据，要取数据第几位的数字
 * Return：指定位数据
 * Author: Moshui
 **********************************************************************/
int getdigit(int data, int x)
{
	while(--x)
		data /= 10;
	return data % 10;
}

/**********************************************************************
 * Function: 获取待排列数据中位数最多的数据位,用以确定分配收集次数
 * Input：待排序数据，待排序数据长度
 * Return：关键码
 * Author: Moshui
 **********************************************************************/
int getdistance(int * input, int n)
{
	int i = 0, j = 0;
	int d = 0;              // 保存最大数据的关键码
	int temp = -1;
	for(i = 0; i < n; i++)
	{
		if(input[i] > temp)
			temp = input[i];
	}
	while(0 != temp)
	{
		d++;
		temp /= 10;
	}
	return d;
}

/**********************************************************************
 * Function: 打印排序步骤函数
 * Author: Moshui
 **********************************************************************/
void print_answer(int * output_p, int n)
{
	int i = 0;
	if(0 == step)
	{
		cout << "The input data is: ";
		for(i = 0; i < n; i++)
			cout << output_p[i] << " ";
		cout << endl;
		cout << "The number of input data is: " << n << "." << endl;
	}
	else
	{
		cout << "Step " << step << ": ";        // 打印每一步排序结果
		for(i = 0; i < n; i++)
			cout << output_p[i] << " ";
		cout << endl;
	}
	step++;
}
/**********************************************************************
 * Function: 数据交换函数
 * Author: Moshui
 **********************************************************************/
void swap(int *s, int *t)
{
	int temp = 0;
	temp = *s;
	*s = *t;
	*t = temp;
}
/**********************************************************************
 * Function: char To int 函数
 * Author: Moshui
 **********************************************************************/
int change_to_num(char *a, int m)
{
	int s = 0;
	for (int i = 0; i < m; i++)
	{
		s += (a[i] - '0')*pow(10.0, m - 1 - i);
	}
	return s;
}

