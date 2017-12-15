#include <iostream>
#include "mpi.h"
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;
int main (){
	MPI::Init(); 
	MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
	ifstream infile;
	infile.open("speech_tokenized.txt");
    	string token;
	int i=0;
	if (infile.is_open()){
		while(infile>>token){
		i++;	
		}
	}
	try {
		int rank = MPI::COMM_WORLD.Get_rank();
		cout << "I am " << rank << std::endl;
		cout<<i<<endl;
	}
	catch (MPI::Exception e) {
		cout << "MPI ERROR: " << e.Get_error_code() \
		<< " - " << e.Get_error_string() \
		<< endl;
	}
	MPI::Finalize();
	return 0;
}




