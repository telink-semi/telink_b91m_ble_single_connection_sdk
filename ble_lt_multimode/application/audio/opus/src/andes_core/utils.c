#include "core_v5.h"

unsigned long long rdminstret(void)
{
	#if __riscv_xlen == 32
	do {
			unsigned long hi = read_csr(NDS_MINSTRETH);
			unsigned long lo = read_csr(NDS_MINSTRET);

			if (hi == read_csr(NDS_MINSTRETH))
				return ((unsigned long long)hi << 32) | lo;
	} while(1);
	#else
		return (unsigned long long)read_csr(NDS_MINSTRET);
	#endif
}

unsigned long long rdmcycle(void)
{
	#if __riscv_xlen == 32
	do {
			unsigned long hi = read_csr(NDS_MCYCLEH);
			unsigned long lo = read_csr(NDS_MCYCLE);

			if (hi == read_csr(NDS_MCYCLEH))
				return ((unsigned long long)hi << 32) | lo;
	} while(1);
	#else
		return (unsigned long long)read_csr(NDS_MCYCLE);
	#endif
}

void write_stack(int size)
{
	int i;
	unsigned int *s = __nds__get_current_sp();

	for(i = 0; i < size; i++) {

		*(--s) = 0x0;
	}
	return;
}

int cal_used_stack(int size)
{
	int i;
	unsigned int *s = __nds__get_current_sp();
	s -= size;

	for(i = 0; i < size; i++) {
		if(*(s++) != 0x5A5A5A5A)
			break;
	}

	return (size-i)*sizeof(int);
}
