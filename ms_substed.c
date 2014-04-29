#include "common.h"
#include "ms_substed.h"
#include "count.h"
#include "score.h"
#include <stdio.h>
#include <string.h>

static _Bool ms_substed[X*X];

static void ms_substed_func(int elem_start, int *cur_1_num, int *cur_score);
static int ms_substed_subtraction_fill(const int elem);
static void ms_substed_cp(_Bool dst[X*X], _Bool src[X*X]);
static void ms_substed_output(int cur_score);

void ms_substed_main()
{
	int i;
	int cur_1_num, cur_score;

	for(i=0; i<X*X; i++)
		ms_substed[i]=0;

	cur_1_num=0;
	cur_score=0;
	ms_substed_func(0, &cur_1_num, &cur_score);

	return;
}

static _Bool ms_substed_is_filled_all()
{
	int i;

	for(i=0; i<X*X; i++)
		if(!ms_substed[i])
			return 0;
	
	return 1;
}

static void ms_substed_func(int elem_start, int *cur_1_num, int *cur_score)
{
	int elem;
	const int cur_score_backup=*cur_score;
	_Bool ms_substed_backup[X*X];

	if((*cur_1_num==MIN_1_NUM)&&(*cur_score>cur_best_score)){
		if(ms_substed_is_filled_all()){
			cur_best_score=*cur_score;
			ms_substed_output(*cur_score);
		}
	}

	ms_substed_cp(ms_substed_backup, ms_substed);

	for(elem=elem_start; elem<X*X-1; elem++){
		if(!ms_substed[elem]){
			ms_substed_func(elem+1, cur_1_num, cur_score);
			ms_substed[elem]=1;
			(*cur_1_num)++;
			if(*cur_1_num>MIN_1_NUM){
				(*cur_1_num)--;
				continue;
			}
			(*cur_score)+=ms_substed_subtraction_fill(elem)*(X*X-(*cur_1_num));
			ms_substed_func(elem+1, cur_1_num, cur_score);
			ms_substed_cp(ms_substed, ms_substed_backup);
			(*cur_1_num)--;
			*cur_score=cur_score_backup;
		}
	}

	return;
}

static void ms_substed_cp(_Bool dst[X*X], _Bool src[X*X])
{
	memcpy(dst, src, (X*X)*sizeof(_Bool));

	return;
}

static int ms_substed_subtraction_fill(const int elem)
{
	int i, f;
	int num_subtractive_fill=0;

	/* the horizontal line on which elem is */
	f=0;
	for(i=((int)(elem/X))*X; i<elem/X+X; i++)
		if(!ms_substed[i]){
			if(!f)
				/* found 1 empty mass so far so save the element */
				f=i+1;
			else{
				/* found more than 1 empty masses so no chance to fill this line */
				f=0;
				break;
			}
		}
	if(f){
		ms_substed[f-1]=1;
		num_subtractive_fill++;
		num_subtractive_fill+=ms_substed_subtraction_fill(f-1);
	}

	/* the vertical line on which elem is */
	f=0;
	for(i=elem%X; i<X*X; i+=X)
		if(!ms_substed[i]){
			if(!f)
				/* found 1 empty mass so far so save the element */
				f=i+1;
			else{
				/* found more than 1 empty masses so no chance to fill this line */
				f=0;
				break;
			}
		}
	if(f){
		ms_substed[f-1]=1;
		num_subtractive_fill++;
		num_subtractive_fill+=ms_substed_subtraction_fill(f-1);
	}

	/* the left-top to right-bottom diagonal line on which elem is (if it is) */
	if(elem%(X+1)==0){
		f=0;
		for(i=0; i<X*X; i+=X+1)
			if(!ms_substed[i]){
				if(!f)
					/* found 1 empty mass so far so save the element */
					f=i+1;
				else{
					/* found more than 1 empty masses so no chance to fill this line */
					f=0;
					break;
				}
			}
		if(f){
			ms_substed[f-1]=1;
			num_subtractive_fill++;
			num_subtractive_fill+=ms_substed_subtraction_fill(f-1);
		}
	}

	/* the right-top to left-bottom diagonal line on which elem is (if it is) */
	if((elem!=0)&&(elem%(X-1)==0)){
		f=0;
		for(i=X-1; i<X*X; i+=X-1)
			if(!ms_substed[i]){
				if(!f)
					/* found 1 empty mass so far so save the element */
					f=i+1;
				else{
					/* found more than 1 empty masses so no chance to fill this line */
					f=0;
					break;
				}
			}
		if(f){
			ms_substed[f-1]=1;
			num_subtractive_fill++;
			num_subtractive_fill+=ms_substed_subtraction_fill(f-1);
		}
	}

	return num_subtractive_fill;
}

static void ms_substed_output(int score)
{
	int i;
	
	printf("ms_substed:");
	for(i=0; i<X*X; i++)
		printf(" %d", ms_substed[i]?1:0);
	putchar('\n');

	printf("score: %d\n", score);

	return;
}
