#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1
#ifdef DEBUG
 #define MY_ASSERT(x)  ((x)? 0: (printf("assert file=%s,line=%d\n",__FILE__,__LINE__),abort(),1))
 #define TRACE(x)  (printf x)
#else
 #define MY_ASSERT(x)  //nothing
 #define TRACE(x) //nothing
#endif

typedef unsigned long long UINT64;
typedef int Color;
typedef int Pos;

#define  BAN_SIZE 8
#define  BLACK 0
#define  WHITE 1
#define  DRAW  2

#define  BIT(x)     (1LL<<(x))
#define  BIT2(x,y)  BIT(POS(x,y))
#define  POS(x,y)   ((y)*BAN_SIZE+(x))
#define  getBIT(data,x)  (((data)>>(x))&1)
typedef struct Analize_result{
	UINT64 mobility;
	UINT64 mobility_m9,
		   mobility_m8,
		   mobility_m7,
		   mobility_m1,
		   mobility_p1,
		   mobility_p7,
		   mobility_p8,
		   mobility_p9;
	char   mobility_sorted[60];
} Analize_result;

class Ban{
	UINT64 koma[2];  //#0: Black, #1:White
	Color    teban;
	int      pass_cnt;  //直前の相手がPASSした。双方PASS検出用
public:
	Ban(){ init(); }
	~Ban(){}

	void init(){
		memset(&koma, 0, sizeof(koma));
		put_simple(3,3,WHITE); put_simple(4,4,WHITE);
		put_simple(4,3,BLACK); put_simple(3,4,BLACK);
		teban=BLACK;
		pass_cnt=0;
	}
	void print(UINT64 mob=0);
	Color get(const int x, const int y){
		int bitn = y*BAN_SIZE+x;
		int bitdata = (getBIT(koma[0],bitn) + getBIT(koma[1],bitn)*2 -1)&3;
		bitdata ^= (bitdata>>1);
		return bitdata;
	}
	void put_simple(const int x, const int y, Color color){
		koma[color] |= BIT2(x,y);
	}
	void put_simple(int n, Color color){
		koma[color] |= BIT(n);
	}
	void put(const int x, const int y){put(POS(x,y));}
	void put(Pos p);
	void put(Pos p, const Analize_result* ana);
	void pass(){ teban^=1; pass_cnt++; }
	void put_and_ana( Pos p, const Analize_result* now_ana, Analize_result *nextana );
	
	void analize(Analize_result*);
	bool isEndGame();
	Color judge();
	friend void taikyoku(Ban&);
	friend Pos  think(const Ban&, const Analize_result* ana);
	friend Pos  think_sub(const Ban& ban, const Analize_result* ana, Color *winner);
};

