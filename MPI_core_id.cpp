#include <iostream>
#include <mpi.h>
#include "sock_core_map.h"
#include <sched.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <stdlib.h>

//#include <numaif.h>

int main( int argc , char **argv )
{
	int 		rank, sz;
	cpu_set_t	mycore;
	int 		rc_affinity;
	pid_t		mypid;
	int 		mycoreid;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &sz);
	if(!(sz&0x02))
	{
		std::cout << "need exactly two processes\n";
		return -1;
	}

	int **sock_core_map, sock_sz, core_per_sock;

	sock_core_info(sock_core_map, sock_sz, core_per_sock);
	mypid	= getpid();

	CPU_ZERO(&mycore);
	CPU_SET(sock_core_map[rank][0], &mycore);
	
	rc_affinity= sched_setaffinity(mypid, sizeof(cpu_set_t), &mycore);
	if (rc_affinity != 0) perror("sched_setaffinity");
	rc_affinity= sched_getaffinity(mypid, sizeof(cpu_set_t), &mycore);
	if (rc_affinity != 0) perror("sched_getaffinity");
	if (CPU_ISSET(sock_core_map[rank][0], &mycore ))
		mycoreid= sock_core_map[rank][0];
	std::cout<<"mypid"<<mypid<<" ->"<<mycoreid<<"\n";
//	unsigned long nm = 3;

//	set_mempolicy(MPOL_BIND, nm, 2);

	if(rank == 0)
	{
	//	std::cout<<"proc 0 responsible for GPU\n";
	//	std::cout<<"sock count: "<<sock_sz<<"\n";
	//	std::cout<<"core per sock: "<<core_per_sock<<"\n";
	//	for(int i = 0; i< sock_sz; i++)
	//	{
	//		std::cout<<"sock "<<i<<":\n";
	//		for(int j = 0; j < core_per_sock; j++)
	//			std::cout<<sock_core_map[i][j]<<" ";
	//		std::cout<<"\n";
	//	}
		int count;

		MPI_Recv(&count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
					MPI_STATUS_IGNORE);

		int *m	= new int[count];

		MPI_Recv(m, count, MPI_INT, 1, 0, MPI_COMM_WORLD, 
					MPI_STATUS_IGNORE);
		
		for(int i =0; i< 100; i++)
			std::cout<<m[i]<<"\n";

	}else{
	//	std::cout<<"proc 1 responsible for assist pre-explosion\n";
		const int sz= 1<<28;
		MPI_Send(&sz, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		int *data	= new int[sz];

		for(int i = 0; i< 100; i++)
		{
			data[i] = 10086;
		}
		MPI_Send(data, sz, MPI_INT, 0, 0, MPI_COMM_WORLD);

	}
	std::cout<<"malloc done\n";

	MPI_Finalize();
	return 0;
}
