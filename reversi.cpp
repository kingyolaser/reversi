#include <stdio.h>
#include <string.h>
#include "reversi.h"

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
void reversi_init()
{
}

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
int main()
{
	printf("reversi\n");

	reversi_init();
	Ban  ban;
	ban.print();
	printf("------------------------\n");
	ban.init();
	
	return 0;
}


