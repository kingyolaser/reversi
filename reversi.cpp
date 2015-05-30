#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
const int prio_orgdata[]=
{
	 1,49,33,34,35,36,51, 2,
	50,57,17,18,19,20,58,52,
	48,32,13, 5, 6,14,21,37,
	47,31,12,99,99, 7,22,38,
	46,30,11,99,99, 8,23,39,
	45,29,16,10, 9,15,24,40,
	56,60,28,27,26,25,59,53,
	 3,55,44,43,42,41,54, 4
};

// *******************************************************************
int str2pos(const char str[2])
{  //速度重視しない。
	char str_x = toupper(str[0]);
	char str_y = str[1];
	if( 'A'<=str_x && str_x<='H'
		&& '1'<=str_y && str_y<='8' ){
		int x=str_x-'A';
		int y=str_y-'1';
		return POS(x,y);
	}else{
		return -1;
	}
}

void printpos(Pos p)
{
	printf("%c%c\n", "ABCDEFGH"[p%BAN_SIZE], p/8+'1');
}

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
void Ban::put(Pos p, const Analize_result* ana)
{
	Color aite = teban^1;
	UINT64 koma_aite = koma[aite];
	UINT64 aitenaka = koma_aite&0x7e7e7e7e7e7e7e7e;
	UINT64 rev_map=0;
	UINT64 check;

	if( p==-1 ){
		MY_ASSERT(0);
	}
	
	//printpos(p);
	
	//置石の左上方向、bitmapは下位方向
	check=BIT(p);
	if( ana->mobility_m9 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check>>=9; rev_map|=(check&=aitenaka);
		check>>=9; rev_map|=(check&=aitenaka);
		check>>=9; rev_map|=(check&=aitenaka);
		check>>=9; rev_map|=(check&=aitenaka);
		check>>=9; rev_map|=(check&=aitenaka);
		check>>=9; rev_map|=(check&=aitenaka);
	}

	//置石の上方向、bitmapは下位方向
	check=BIT(p);
	if( ana->mobility_m8 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check>>=8; rev_map|=(check&=koma_aite);
		check>>=8; rev_map|=(check&=koma_aite);
		check>>=8; rev_map|=(check&=koma_aite);
		check>>=8; rev_map|=(check&=koma_aite);
		check>>=8; rev_map|=(check&=koma_aite);
		check>>=8; rev_map|=(check&=koma_aite);
	}

	//置石の右上方向、bitmapは下位方向
	check=BIT(p);
	if( ana->mobility_m7 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check>>=7; rev_map|=(check&=aitenaka);
		check>>=7; rev_map|=(check&=aitenaka);
		check>>=7; rev_map|=(check&=aitenaka);
		check>>=7; rev_map|=(check&=aitenaka);
		check>>=7; rev_map|=(check&=aitenaka);
		check>>=7; rev_map|=(check&=aitenaka);
	}

	//置石の左方向、bitmapは下位方向
	check=BIT(p);
	if( ana->mobility_m1 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check>>=1; rev_map|=(check&=aitenaka);
		check>>=1; rev_map|=(check&=aitenaka);
		check>>=1; rev_map|=(check&=aitenaka);
		check>>=1; rev_map|=(check&=aitenaka);
		check>>=1; rev_map|=(check&=aitenaka);
		check>>=1; rev_map|=(check&=aitenaka);
	}
	
	//置石の右方向、bitmapは上位方向
	check=BIT(p);
	if( ana->mobility_p1 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check<<=1; rev_map|=(check&=aitenaka);
		check<<=1; rev_map|=(check&=aitenaka);
		check<<=1; rev_map|=(check&=aitenaka);
		check<<=1; rev_map|=(check&=aitenaka);
		check<<=1; rev_map|=(check&=aitenaka);
		check<<=1; rev_map|=(check&=aitenaka);
	}
	
	//置石の左下方向、bitmapは上位方向
	check=BIT(p);
	if( ana->mobility_p7 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check<<=7; rev_map|=(check&=aitenaka);
		check<<=7; rev_map|=(check&=aitenaka);
		check<<=7; rev_map|=(check&=aitenaka);
		check<<=7; rev_map|=(check&=aitenaka);
		check<<=7; rev_map|=(check&=aitenaka);
		check<<=7; rev_map|=(check&=aitenaka);
	}
	
	//置石の下方向、bitmapは上位方向
	check=BIT(p);
	if( ana->mobility_p8 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check<<=8; rev_map|=(check&=koma_aite);
		check<<=8; rev_map|=(check&=koma_aite);
		check<<=8; rev_map|=(check&=koma_aite);
		check<<=8; rev_map|=(check&=koma_aite);
		check<<=8; rev_map|=(check&=koma_aite);
		check<<=8; rev_map|=(check&=koma_aite);
	}
	//置石の右下方向、bitmapは上位方向
	check=BIT(p);
	if( ana->mobility_p9 & check){  //置けることが確認済み
		//printf("p1 rev\n");
		check<<=9; rev_map|=(check&=aitenaka);
		check<<=9; rev_map|=(check&=aitenaka);
		check<<=9; rev_map|=(check&=aitenaka);
		check<<=9; rev_map|=(check&=aitenaka);
		check<<=9; rev_map|=(check&=aitenaka);
		check<<=9; rev_map|=(check&=aitenaka);
	}
	
	MY_ASSERT(rev_map!=0);
	koma[0] ^= rev_map;
	koma[1] ^= rev_map;
	put_simple(p,teban);
	teban ^= 1;
	pass_cnt=0;
}

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

// *******************************************************************
void Ban::put_and_ana( Pos p, const Analize_result* now_ana, Analize_result *nextana )
{
	put(p, now_ana );
	analize(nextana);
	if( nextana->mobility != 0 ){
		return;
	}
	//nextana->mobility==0
	pass();
	analize(nextana);
	if( nextana->mobility != 0 ){
		return;
	}
	//nextana->mobility==0
	pass();  //endgame
	return;
}

// *******************************************************************
void Ban::kifu_in(const char* kifu)
{	//速度重視しない
	Analize_result ana_result;
	int kifu_len=strlen(kifu);

	while( kifu_len>=2 ){
		analize(&ana_result);
		if( ana_result.mobility == 0 ){
			pass();
			if( pass_cnt>=2 ){
				printf("endgame\n");
				abort(); //棋譜にのこりがあるのに両PASS⇒NG
				break;
			}else{
				//printf("pass\n");
				continue;
			}
		}
		
		//print(ana_result.mobility);
		put(str2pos(kifu),&ana_result);
		kifu_len-=2;
		kifu+=2;
	}
	analize(&ana_result);
	print(ana_result.mobility);
	if( isEndGame() ){
		const char* teban_str[3]={"BLACK","WHITE","DRAW"};
		printf("Judge=%s\n", teban_str[judge()]);
	}
}

// *******************************************************************
bool Ban::isEndGame()
{
	if( 0 == ~(koma[0]|koma[1]) || pass_cnt>=2 ){
		return true;
	}else{
		return false;
	}
}

// *******************************************************************
Color Ban::judge()
{	//ゲーム終了になっていることを呼び側で保障すること
	int black_num = countbit(koma[BLACK]);
	int white_num = countbit(koma[WHITE]);
	if( black_num > white_num ){
		return BLACK;
	}else if( black_num < white_num ){
		return WHITE;
	}else{
		return DRAW;
	}
}
// *******************************************************************
void reversi_init()
{
	for( int pos=0; pos<64; pos++){
		if( prio_orgdata[pos]<=64 ){
			PriorityTable[prio_orgdata[pos]-1]=pos;
		}
	}
}


// *******************************************************************
void taikyoku(Ban &ban)
{
	Analize_result ana_result;
	Pos pos;

	for(;;){
		if( ban.isEndGame() ){
			break;
		}
		ban.analize(&ana_result);
		if( ana_result.mobility == 0 ){
			printf("pass\n");
			ban.pass();
			continue;
		}
		ban.print(ana_result.mobility);
		switch(ban.teban){
		case BLACK:
			for(;;){
				printf("Input te: ");
				char buf[40];
				fgets(buf,sizeof(buf), stdin);
				int len=strlen(buf);
				if( len!=2 && len!=3 ){ continue; }
				if( buf[len-1]=='\n' ){ buf[len-1]=0; len--; }
				if( len!=2 ){ printf("strlen not 2 <%s>\n", buf); continue;}
				pos=str2pos(buf);
				if(pos==-1){ printf("??\n"); continue; }
				if( (ana_result.mobility & BIT(pos))==0 ){
					printf("cannot put\n");
					continue;
				}
				break; //ok
			}
			ban.put(pos, &ana_result);
			break;
		case WHITE:
			pos = think(ban,&ana_result);
			printpos(pos);
			ban.put(pos, &ana_result);
			break;
#ifdef DEBUG
		default:
			MY_ASSERT(0);
			break;
#endif
		}
	}
	printf("----game end----\n");
	ban.print(0);

}

// *******************************************************************
Pos  think(const Ban& ban, const Analize_result* ana)
{ //１箇所以上置けることが確認済みであること
	int zan = countbit(~(ban.koma[0]|ban.koma[1]));
	if( zan > 23 ){
		return ana->mobility_sorted[0];
	}
	//zan<=3
	printf("***** think start *****\n");
	Color winner;
	Pos ret = think_sub(ban, ana, &winner);
	printf("***** think end: winner = %d *****\n", winner);
	return ret;
}

Pos  think_sub(const Ban& ban, const Analize_result* ana, Color *winner)
{ //１箇所以上置けることが確認済みであること
	Pos draw_pos=-1;
	for(int i=0; ana->mobility_sorted[i]!=-1;i++){
		Pos nextpos = ana->mobility_sorted[i];
		Ban nextban = ban;
		Analize_result  nextana;
		
		nextban.put_and_ana( nextpos, ana, &nextana );
		if( nextban.isEndGame() ){
			*winner = nextban.judge();
			if( *winner == DRAW ){
				draw_pos = nextpos;
				continue;
			}else if( *winner == ban.teban ){ //勝ち筋発見。終了。
				return nextpos;
			}else{ //負けなので、勝ち筋探し続行
				continue;
			}
		}
		
		//isEndGame()==false
		think_sub( nextban, &nextana, winner );
		if( *winner == DRAW ){
			draw_pos = nextpos;
			continue;
		}else if( *winner == ban.teban ){ //勝ち筋発見。終了。
			return nextpos;
		}else{ //負けなので、勝ち筋探し続行
			continue;
		}
	}
	
	//勝ち筋が見つからなかった。
	if( draw_pos==-1 ){  //引き分けの手もなし
		*winner = ban.teban^1; //相手勝ち
		return ana->mobility_sorted[0];
	}else{ //引き分けの手あり。
		*winner = DRAW;
		return draw_pos;
	}
}

// *******************************************************************
int main()
{
	printf("reversi\n");

	reversi_init();
	Analize_result ana_result;

	
	Ban  ban;
	ban.analize(&ana_result);
	ban.print(ana_result.mobility);
	printf("kifu_in test------------------------\n");
	ban.init();
	ban.analize(&ana_result);
	ban.kifu_in("F5D6C4F4F6F3D3F7E6C5D7E3E2F2G7E7C3G6G5C7D2B4G3C2B2B3B5B6B7C6G2");
	
	printf("------------------------\n");

	printf("taikyoku start------------------------\n");
	ban.init();
	taikyoku(ban);
	
	return 0;
}


