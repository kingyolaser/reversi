#include <stdio.h>
#include <string.h>
#include "reversi.h"

#define  PRIO(x) PriorityTable[x]

int PriorityTable[60];

// *******************************************************************
int countbit(UINT64 x)
{
	int ret=0;
	for( int i=0; i<64; i++ ){
		ret += (x&1);
		x>>=1;
	}
	return ret;
}

// *******************************************************************
// *******************************************************************


// *******************************************************************
void Ban::print(UINT64 mob)
{
	//printf("black %016llx\n", koma[0]);
	//printf("white %016llx\n", koma[1]);
	printf("  A B C D E F G H\n");
	for(int y=0; y<BAN_SIZE; y++ ){
		printf("%d ",y+1);
		for( int x=0; x<BAN_SIZE; x++){
			if( mob & BIT2(x,y) ){
				printf("+ ");
			}else{
				printf("%c ", "@o."[get(x,y)]);
			}
		}
		printf("\n");
	}
	printf("BLACK=%d, WHITE=%d\n", countbit(koma[BLACK]), countbit(koma[WHITE]) );
	const char* teban_str[2]={"BLACK","WHITE"};
	printf("teban=%s\n", teban_str[teban]);
}

// *******************************************************************
// *******************************************************************
void Ban::analize(Analize_result *result)
{
	Color aite=teban^1;
	const UINT64 koma_teban = koma[teban];
	const UINT64 koma_aite  = koma[aite];
	const UINT64 blank = ~(koma_teban | koma_aite);
	UINT64 t;
	UINT64 mobility=0;
	
	UINT64 w = koma_aite & 0x7e7e7e7e7e7e7e7e;
	//置石の左方向, bitmapは下位方向、下位から上位へ向かってチェックする
	t = w & (koma_teban << 1);
	t |= w & (t << 1);
	t |= w & (t << 1);
	t |= w & (t << 1);
	t |= w & (t << 1);
	t |= w & (t << 1);
	mobility |= (result->mobility_m1 = blank & (t << 1));

	//置石の右方向
	t = w & (koma_teban >> 1);
	t |= w & (t >> 1);
	t |= w & (t >> 1);
	t |= w & (t >> 1);
	t |= w & (t >> 1);
	t |= w & (t >> 1);
	mobility |= (result->mobility_p1 = blank & (t >> 1));

	//ななめ
	t = w & (koma_teban << 9);
	t |= w & (t << 9);
	t |= w & (t << 9);
	t |= w & (t << 9);
	t |= w & (t << 9);
	t |= w & (t << 9);
	mobility |= (result->mobility_m9 = blank & (t << 9));

	t = w & (koma_teban << 7);
	t |= w & (t << 7);
	t |= w & (t << 7);
	t |= w & (t << 7);
	t |= w & (t << 7);
	t |= w & (t << 7);
	mobility |= (result->mobility_m7 = blank & (t << 7));
	
	t = w & (koma_teban >> 9);
	t |= w & (t >> 9);
	t |= w & (t >> 9);
	t |= w & (t >> 9);
	t |= w & (t >> 9);
	t |= w & (t >> 9);
	mobility |= (result->mobility_p9 = blank & (t >> 9));

	t = w & (koma_teban >> 7);
	t |= w & (t >> 7);
	t |= w & (t >> 7);
	t |= w & (t >> 7);
	t |= w & (t >> 7);
	t |= w & (t >> 7);
	mobility |= (result->mobility_p7 = blank & (t >> 7));

	//上方向
	t = koma_aite & (koma_teban << 8);
	t |= koma_aite & (t << 8);
	t |= koma_aite & (t << 8);
	t |= koma_aite & (t << 8);
	t |= koma_aite & (t << 8);
	t |= koma_aite & (t << 8);
	mobility |= (result->mobility_m8 = blank & (t << 8));
	
	//下方向
	t = koma_aite & (koma_teban >>8);
	t |= koma_aite & (t >> 8);
	t |= koma_aite & (t >> 8);
	t |= koma_aite & (t >> 8);
	t |= koma_aite & (t >> 8);
	t |= koma_aite & (t >> 8);
	mobility |= (result->mobility_p8 = blank & (t >> 8));

	//printf("mobility\n"); print_bitmap(mobility);
	
	result->mobility = mobility;
	//おける場所を優先順位でソートする。
	int i,j;
	for( i=0, j=0; i<60; i++){ //優先順位順にループ
		Pos p=PRIO(i);
		if( mobility & BIT(p) ){
			result->mobility_sorted[j++]=p;
		}
	}
	for(;j<60;j++){ result->mobility_sorted[j]=-1; }
}
void reversi_init()
{
}


// *******************************************************************
// *******************************************************************
int main()
{
	printf("reversi\n");

	reversi_init();
	Analize_result ana_result;

	
	Ban  ban;
	ban.analize(&ana_result);
	ban.print(ana_result.mobility);

	printf("------------------------\n");
	ban.init();
	
	return 0;
}


