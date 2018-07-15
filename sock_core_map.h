#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#define	SZ	255

int sock_core_info(	int** 	&sock_core_map, 
					int 	&sock_sz, 
					int 	&core_per_sock)
{
	std::string sline;
	std::string str;
	
	std::stringstream ss;
	
	int *core_id	= new int[SZ];
	int *sock_id	= new int[SZ];

	std::ifstream myfile("/proc/cpuinfo");

	if(!myfile.is_open())
		return -1;
	
	int ct_sz	= 0;
	while(std::getline(myfile, sline))
	{
		ss.str("");
		ss.clear();

		ss<<sline;

		ss>>str;
		if(str	== "processor")
		{
			ss>>str;//escape: ":"	
			ss>>core_id[ct_sz];
	//		std::cout<<core_id[ct_sz]<<"\n";
		}

		if(str	== "physical")
		{
			ss>>str;//escape: "id"
			ss>>str;//escape: ":"
			ss>>sock_id[ct_sz];
	//		std::cout<<sock_id[ct_sz]<<"\n";
			
			++ct_sz;
		}
	}
	myfile.close();
//	std::cout<<"Number of cores: "<<ct_sz<<"\n";	
	
	sock_sz	= 0;
	for(int i = 0; i< ct_sz; i ++)
	{
		if(sock_sz < sock_id[i]) sock_sz = sock_id[i];
	}
	++sock_sz;//sock size is one more than sock id
	
	core_per_sock	= ct_sz/sock_sz;

	int *ptr		= new int[sock_sz];
	sock_core_map	= new int*[sock_sz];
	
	for(int i = 0; i< sock_sz; i++){
		ptr[i]				= 0;
		sock_core_map[i]	= new int[core_per_sock];
	}

	for(int i =0; i< ct_sz; i++){
		sock_core_map[sock_id[i]][ptr[sock_id[i]]] = core_id[i];
		ptr[sock_id[i]]++;
	}

	delete[] sock_id;
	delete[] core_id;
	delete[] ptr;


	return 0;
}
