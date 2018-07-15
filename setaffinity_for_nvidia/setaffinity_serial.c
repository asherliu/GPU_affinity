#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int setaffinity_for_nvidia(int, int, int);

	// Call setaffinity_for_nvidia() early in the code before
	// large data structures are potentially assigned to numa memory nodes
	// that may be potentially distant from the bus with your target GPU.
	// For the serial case, set mpi my_rank to 0.
	// PPN can be any non-zero int.
	// GPU may be specified from the environment variable MYCUDADEVICE,
	// or use -1 to let setaffinity return a gpudevice number.
	int gpudevice= setaffinity_for_nvidia(0, 6, -1);
	// cudaSetDevice(mygpudevice);

	// OpenACC code could call:
	// acc_set_device_num( mygpudevice, devicetype );
	// acc_get_device_num( devicetype );

	return(0);
}
