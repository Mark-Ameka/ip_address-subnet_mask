#include <stdio.h>
#define ip_size 4
#define sub_size 4
#define bytes 32

int cmpfunc(const void *a, const void *b){
   return ( *(int*)b - *(int*)a );
}

typedef struct{
	int o1;
	int o2;
	int o3;
	int o4;
}Address;

typedef struct{
	int n1;
	int n2;
	int n3;
	int n4;
}Mask;

typedef struct{
	int num_of_host;
	int prefix;
	int delta;
	Mask sub_mask;
}Octet;

int main(){
	//ip address
	Address ip_add = {65, 32, 44, 15};
	//prefix
	int prefix = 17;
	//hosts
	int noh[] = {18004, 6005, 777, 202, 133, 45, 12, 2, 2, 2, 2, 2};
	
	int i, j, res;
	
	Mask subnet;
	Address new_ip;
	
	//sorts noh array
	int size = sizeof(noh)/sizeof(int);
	qsort(noh, size, sizeof(int), cmpfunc);
	
				  //4th Octet
	Octet oc[] = {{2,   30, 4,   {255,255,255,252}},
				  {6,   29, 8,   {255,255,255,248}},
				  {14,  28, 16,  {255,255,255,240}},
				  {30,  27, 32,  {255,255,255,224}},
				  {62,  26, 64,  {255,255,255,192}},
				  {126, 25, 128, {255,255,255,128}},
				  {254, 24, 1,   {255,255,255,0}},
				  //3rd Octet
				  {510,   23, 2,   {255,255,254,0}},
				  {1022,  22, 4,   {255,255,252,0}},
				  {2046,  21, 8,   {255,255,248,0}},
				  {4094,  20, 16,  {255,255,240,0}},
				  {8190,  19, 32,  {255,255,224,0}},
				  {16382, 18, 64,  {255,255,192,0}},
				  {32766, 17, 128, {255,255,128,0}},
				  {65534, 16, 1,   {255,255,0,0}},
				  //2nd Octet
				  {131070,   15, 2,   {255,254,0,0}},
				  {262142,   14, 4,   {255,252,0,0}},
				  {524286,   13, 8,   {255,248,0,0}},
				  {1048574,  12, 16,  {255,240,0,0}},
				  {2097150,  11, 32,  {255,224,0,0}},
				  {4194302,  10, 64,  {255,192,0,0}},
				  {8388606,  9,  128, {255,128,0,0}},
				  {16777214, 8,  1,   {255,0,0,0}},
				  //1st Octet
				  {33554430,   7, 2,   {254,0,0,0}},
				  {67108862,   6, 4,   {252,0,0,0}},
				  {134217726,  5, 8,   {248,0,0,0}},
				  {268435454,  4, 16,  {240,0,0,0}},
				  {536870910,  3, 32,  {224,0,0,0}},
				  {1073741822, 2, 64,  {192,0,0,0}},
				  {2147483646, 1, 128, {128,0,0,0}}};
	
	//find prefix and retrieve subnet mask
	for(i = 0; i < bytes && prefix != oc[i].prefix; i++){}
	if(i < bytes){
		subnet = oc[i].sub_mask;
	}
	
	printf("IP ADD: ");
	printf("%11d.", ip_add.o1);
	printf("%d.", ip_add.o2);
	printf("%d.", ip_add.o3);
	printf("%d", ip_add.o4);
	printf("\n");
	
	printf("SUBNET MASK: ");
	printf("%6d.", subnet.n1);
	printf("%d.", subnet.n2);
	printf("%d.", subnet.n3);
	printf("%d", subnet.n4);
	printf("\n");
	
	//network
	new_ip.o1 = ip_add.o1 & subnet.n1;
	new_ip.o2 = ip_add.o2 & subnet.n2;
	new_ip.o3 = ip_add.o3 & subnet.n3;
	new_ip.o4 = ip_add.o4 & subnet.n4;
	
	printf("NETWORK: ");
	printf("%10d.", new_ip.o1);
	printf("%d.", new_ip.o2);
	printf("%d.", new_ip.o3);
	printf("%d", new_ip.o4);
	printf("\n\n");
	
	printf("%-7s  %-10s|%-7s|%-5s| %-15s|%-8s\n", " ", "NOH", "PREFIX", "DELTA", "IP", "SUBNET");
	for(i = 0; i<size; i++){
		printf("%-7d: ", noh[i]);
		//find first occurence
		for(j = 0; j < bytes && (oc[j].num_of_host - noh[i]) < 0;  j++){}
		if(j < bytes){
			printf("%-10d|%-7d|%-5d| ", oc[j].num_of_host, oc[j].prefix, oc[j].delta);
			printf("%-3d.%-3d.%-3d.%-3d|", new_ip.o1, new_ip.o2, new_ip.o3, new_ip.o4);

			if(oc[j].sub_mask.n4 == 0){
				new_ip.o3 += oc[j].delta;
			} else if(oc[j].sub_mask.n4 != 0){
				new_ip.o4 += oc[j].delta;
			} else if(oc[j].sub_mask.n2 != 0){
				new_ip.o2 += oc[j].delta;
			} else if(oc[j].sub_mask.n2 == 0){
				new_ip.o1 += oc[j].delta;
			}
			
			printf("%-3d.%-3d.%-3d.%-3d\n", oc[j].sub_mask.n1, oc[j].sub_mask.n2, oc[j].sub_mask.n3, oc[j].sub_mask.n4);
		}
	}
		
	return 0;
}
