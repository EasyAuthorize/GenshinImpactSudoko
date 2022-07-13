#include <stdio.h>
#include <time.h>

#include <stdbool.h>

struct sudoko{
	//当前值
	int val;
	//最小值
	int min;
	//最大值
	int max;
	//是否是固定的数值
	bool fixed;
};

bool check(struct sudoko *sd,size_t line);

int main(int argc, char *argv[]){
	puts("数独穷举");

	//初始化
	//0 = 1 - 4
	//-1 = 5 - 8

	struct sudoko init[9] = {
	{1,1,4,0},{9,9,9,1},{2,2,2,1},
	{3,3,3,1},{5,5,8,0},{5,5,8,0},
	{8,8,8,1},{1,1,4,0},{6,6,6,1}
	};
//穷举

//记忆全部可编辑位置的下标
int x[sizeof(init)/sizeof(struct sudoko)] = {0};
size_t len = 0;
int i = 0;
while(i<sizeof(init)/sizeof(struct sudoko)){
	//越过固定数值
	if(init[i].fixed == true){
		i++;
		continue;
	}
	x[len] = i;
	len++;
	i++;
}

/*
	原理图
	在低位一直+1		到达最大值时向前(→)进一
	┌	┬	┐	┌	┬	┐	┌	┬	┐	┌	┬	┐
	├	1	┤	├	2	┤	├	3	┤	├	0	┤
	└	┴	┘	└	┴	┘	└	┴	┘	└	┴	┘
*/

//a = 穷举位置
//b = 当前穷举最高位
int a = 0,b = 0;
while(1){

	//如果达到最大值则进一
	if(init[x[a]].val == init[x[a]].max){
		//如果进位已到达最大则继续进位
		do{
			a++;
			if(a==len){
				puts("穷举完毕，没有结果");
				return 1;
			}
		}while(init[x[a]].val == init[x[a]].max);
		init[x[a]].val++;
		//如果当前穷举位置大于高位则设置高位为当前位置
		if(a>b) b=a;
		//低位清空
		while(a > 0){
			a--;
			init[x[a]].val = init[x[a]].min;
		}
	}else{
		init[x[a]].val++;
	}

	//校验结果
	if(check(init,3)){
		puts("穷举完毕");
		printf("结果为：%d %d %d %d\n",init[x[0]].val,init[x[1]].val,init[x[2]].val,init[x[3]].val);
		printf("%d\t%d\t%d\n%d\t%d\t%d\n%d\t%d\t%d\n",init[0].val,init[1].val,init[2].val,init[3].val,init[4].val,init[5].val,init[6].val,init[7].val,init[8].val);
		puts("完工大吉~");
		break;
	}
	puts("\t--\t结果包含不相等的值\t--\t");

	printf("Debug: 当前穷举值为%d %d %d %d\n",init[x[0]].val,init[x[1]].val,init[x[2]].val,init[x[3]].val);

	printf("%d\t%d\t%d\n%d\t%d\t%d\n%d\t%d\t%d\n",init[0].val,init[1].val,init[2].val,init[3].val,init[4].val,init[5].val,init[6].val,init[7].val,init[8].val);

}


	return 0;
}

bool check(struct sudoko *sd,size_t line){
	int w = 1,h = 1;
	int res[line*2+2];
	int *i = res;
	//计算横向每一行
	while(h<=line){
		//计算一行
		*i = 0;
		while (w <= line)
		{
			*i += sd[(h - 1)*line + w - 1].val;
			w++;
		}
		printf("横向计算结果:%d\n",*i);
		i++;
		h++;
		w = 1;
	}

	w=h=1;
	//计算纵向每一行
	while(w<=line){
		//计算一行
		*i = 0;
		while (h <= line)
		{
			*i += sd[(h - 1)*line + w - 1].val;
			h++;
		}
		printf("纵向计算结果:%d\n",*i);
		i++;
		w++;
		h = 1;
	}

	//计算右斜边
	w = h = 1;
	*i = 0;
	while(w<=line){
		*i += sd[(h - 1)*line + w - 1].val;
		w++;
		h++;
	}
	printf("右斜线计算结果:%d\n",*i);
	i++;

	//计算左斜边
	w = line;
	h = 1;
	*i = 0;
	while(h<=line){
		*i += sd[(h - 1)*line + w - 1].val;
		w--;
		h++;
	}
	printf("左斜线计算结果:%d\n",*i);

	//挨个比较 如果有数值不相同则返回false
	for(int m = 1;m<line*2+2;m++){
		if(res[m] != res[0])
		return false;
	}

	return true;
}
