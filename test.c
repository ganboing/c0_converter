standalone int x[5];
standalone int y[5];

void main()
{
	abort;
	commit;
}

void test()
{
	runner main() using x[0,,4], y[0,,4];
	commit;
}
