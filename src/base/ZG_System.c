#include "@zg_base.h"

uint32_t ZG_System_GetTicks(void){
#if defined(_WIN32 )
	 SYSTEMTIME st;
	GetSystemTime(&st);
	return st.wMilliseconds;
#else
	struct timeval tval;

	gettimeofday(&tval_before, NULL);
	// multiply 0.001 to convert us to ms
	return (long int)tval.tv_usec*(0.001f);
#endif
}
