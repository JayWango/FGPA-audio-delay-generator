#include "xil_cache.h"		                /* Cache Drivers */
#include "bsp.h"

int main() {
	Xil_ICacheInvalidate();
	Xil_ICacheEnable();
	Xil_DCacheInvalidate();
	Xil_DCacheEnable();

	BSP_init();

	while(1);

	return 0;
}
