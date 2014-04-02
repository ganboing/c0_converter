__attribute__((aligned(0x1000))) int x[5];
__attribute__((aligned(0x1000))) int y[5];

void main()
{
	do{void abort(void) __attribute__((noreturn)); abort();}while(0);
	do{void commit(void) __attribute__((noreturn)); commit();}while(0);
}

void test()
{
	//runner main() using x[0,,4], y[0,,4];
	do{void commit(void) __attribute__((noreturn)); commit();}while(0);
}
