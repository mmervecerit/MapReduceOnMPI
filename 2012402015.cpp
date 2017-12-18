#include <iostream>
#include "mpi.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

#define WORKTAG     1
#define HOPTAG     2
typedef struct{
  int howmany;
  char word[1024];
}my_pair;
int main (){
	MPI::Init(); 

	int rank,num_of_processes;
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_processes);
	rank = MPI::COMM_WORLD.Get_rank();
	my_pair sender;
 	MPI_Datatype mWord;
  	MPI_Aint disp[2];
	int blockLen[2]={1,1024};
 	MPI_Get_address(&(sender.howmany),&(disp[0]));
  	MPI_Get_address(&(sender.word),&(disp[1]));
	MPI_Datatype types[2]={MPI_INT,MPI_CHAR};
 	 disp[1]=disp[1]-disp[0];
 	 disp[0]=(MPI_Aint) 0;
 	 MPI_Type_create_struct(2,blockLen,disp,types,&mWord);
 	 MPI_Type_commit(&mWord);
	MPI_Status status;

	int howmanylinestosend;
	int numoflines;
	if(rank==0){
	int size;
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
	numoflines=all_words.size();
	if(numoflines%(size-1)==0){

		howmanylinestosend=numoflines/(size-1);
		for(int i=1;i<size;i++){
			MPI_Send(&howmanylinestosend,1,MPI_INT,i,WORKTAG,MPI_COMM_WORLD);
			for(int j =0; j<howmanylinestosend;j++){
				string work = all_words.front();
				all_words.erase(all_words.begin());
				int length = work.length();
				char tab2[length+1];
				strncpy(tab2, work.c_str(), sizeof(tab2));
				tab2[sizeof(tab2) - 1] = 0;
				MPI_Send(&tab2,length+1,MPI_CHAR,i,WORKTAG,MPI_COMM_WORLD);			
			}
		}
	}
	else{
	int numofnulls=0;
	numofnulls=(size-1)-(numoflines%(size-1));

		for(int i = 0 ; i<numofnulls;i++){
			string null="";
			all_words.push_back(null);		
		}
		numoflines=all_words.size();
		howmanylinestosend=numoflines/(size-1);
		for(int i=1;i<size;i++){
			MPI_Send(&howmanylinestosend,1,MPI_INT,i,WORKTAG,MPI_COMM_WORLD);
			for(int j =0; j<howmanylinestosend;j++){
				string work = all_words.front();
				all_words.erase(all_words.begin());
				int length = work.length();
				char tab2[length+1];
				strncpy(tab2, work.c_str(), sizeof(tab2));
				tab2[sizeof(tab2) - 1] = 0;
				MPI_Send(&tab2,length+1,MPI_CHAR,i,WORKTAG,MPI_COMM_WORLD);			
			}
		}
			
	}
	vector<my_pair> pairs_came_from_slaves;
	for(int i=1;i<size;i++){
		for(int j =0; j<howmanylinestosend;j++){
		my_pair pushthis;
		MPI_Recv(&pushthis,1,mWord,i,HOPTAG,MPI_COMM_WORLD,&status);
		pairs_came_from_slaves.push_back(pushthis);
		}
	}
	for(int i=1;i<size;i++){
		for(int j =0; j<howmanylinestosend;j++){
			my_pair sendthis;
			sendthis=pairs_came_from_slaves.front();
			pairs_came_from_slaves.erase(pairs_came_from_slaves.begin());
			MPI_Send(&sendthis,1,mWord,i,WORKTAG,MPI_COMM_WORLD);
			
		}	
	}
	}
	else{
		vector<string> received_words;
		char received_word[1024];
		int howmanylinestoreceive;
		MPI_Recv(&howmanylinestoreceive,1,MPI_INT,0,WORKTAG,MPI_COMM_WORLD,&status);
		for(int j =0; j<howmanylinestoreceive;j++){
    		MPI_Recv(&received_word,1024,MPI_CHAR,0,WORKTAG,MPI_COMM_WORLD,&status);
		string str(received_word);
		received_words.push_back(str);
		}
		for(vector<string>::iterator it = received_words.begin(); it != received_words.end(); ++it) {
			string temp=*it;
			my_pair send_to_master;
			strncpy(send_to_master.word, temp.c_str(), sizeof(send_to_master.word));
			send_to_master.word[sizeof(send_to_master.word) - 1] = 0;
			send_to_master.howmany=1;
			MPI_Send(&send_to_master,1,mWord,0,HOPTAG,MPI_COMM_WORLD);
    		}

		vector<my_pair> my_pairs_came_from_master;
		for(int j =0; j<howmanylinestoreceive;j++){
			my_pair pushit; 
			MPI_Recv(&pushit,1,mWord,0,WORKTAG,MPI_COMM_WORLD,&status);
			my_pairs_came_from_master.push_back(pushit);
		}
		//cout<<my_pairs_came_from_master[my_pairs_came_from_master.size()-1].word<<" "<<my_pairs_came_from_master[my_pairs_came_from_master.size()-1].howmany<<endl;
	
		
	}
 	MPI_Type_free(&mWord);
	MPI::Finalize();
	return 0;
}





