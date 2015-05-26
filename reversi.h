typedef unsigned long long UINT64;
typedef int Color;
#define  BAN_SIZE 8
#define  BLACK 0
#define  WHITE 1

#define  BIT(x)     (1LL<<(x))
#define  BIT2(x,y)  BIT(POS(x,y))
#define  POS(x,y)   ((y)*BAN_SIZE+(x))
#define  getBIT(data,x)  (((data)>>(x))&1)
class Ban{
	UINT64 koma[2];  //#0: Black, #1:White
	Color    teban;
public:
	Ban(){ init(); }
	~Ban(){}

	void init(){
		memset(&koma, 0, sizeof(koma));
		put_simple(3,3,WHITE); put_simple(4,4,WHITE);
		put_simple(4,3,BLACK); put_simple(3,4,BLACK);
		teban=BLACK;

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
};

