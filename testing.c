void test(void) {
	do {
		struct
			__attribute__((packed))
			{	unsigned long cnt;
				struct
					__attribute__((packed)) {unsigned long base;
						unsigned long len;
					} ranges[2];
				} runner_using;
				{
					runner_using.cnt = 2;
					runner_using.ranges[0].base = (x);
					runner_using.ranges[0].len = (&((x)[4]) - &((x)[0]));
					runner_using.ranges[1].base = (y);
					runner_using.ranges[1].len = (&((y)[4]) - &((y)[0]));
				}
				struct
					__attribute__((packed)) {unsigned long cnt;
					} runner_watching;
					{
						runner_watching.cnt = 0;
					}
					extern void l0_syscall_spawn(const char*, const char*,
							const char*);
					l0_syscall_spawn(newr0, runner_using, runner_watching);
					{
						main();
					}
				} while (0);

			}
