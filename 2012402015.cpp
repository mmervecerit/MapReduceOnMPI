#include <iostream>
#include "mpi.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
/*string temp = "cat";
char tab2[1024];
strncpy(tab2, temp.c_str(), sizeof(tab2));
tab2[sizeof(tab2) - 1] = 0;
*/
struct my_pair{
	string key;
	int value;
};
void master(){
	int rank,size;
	MPI_Status status;
	ifstream infile;
	infile.open("speech_tokenized.txt");
    	string token;
	MPI_Comm_size(MPI_COMM_WORLD,&size); 
	vector<string> all_words;
	if(infile.is_open()){
		while(infile>>token){
			all_words.push_back(token);
		}
	}
	int numoflines=all_words.size();
	if(numoflines%(size-1)==0){
		cout<<"tam bolunuyo"<<endl;
		//for(int i=1;i<size;i++){
		//read,send
		//}
	}
	else{
	int numofnulls=0;
	numofnulls=(size-1)-(numoflines%(size-1));
		cout<<"tam bolunmuyo su kadar null ekleyecez: "<<numofnulls<<endl;
		//for(int i=1;i<size;i++){
		//read,send
		//}	
	}	
}
void slave(){
}
int main (){
	MPI::Init(); 
	int rank,num_of_processes;
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_processes);
	rank = MPI::COMM_WORLD.Get_rank();
	if(rank==0){
		master();
	}
	else if(rank>0 && rank<=num_of_processes-1){
		slave();
	}
	else{	
	cout<<"Error in ranks and size"<<endl;
	}

	MPI::Finalize();
	return 0;
}





