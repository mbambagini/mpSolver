#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>

#include <hardware.pb.h>

#include <gflags/gflags.h>


using namespace std;
using namespace Hardware;


DEFINE_string(srcFile, "", "Binary input file");


int main (int argc, char* argv[])
{
	//check parameters
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	google::ParseCommandLineFlags(&argc, &argv, false);

	if (FLAGS_srcFile.compare("")==0) {
		cerr<<"The source file is mandatory"<<endl;
		return 1;
	}

	Platform hw;
	fstream input(FLAGS_srcFile.c_str(), ios::in | ios::binary);
	if (!hw.ParseFromIstream(&input)) {
		cerr << "Failed to parse " << FLAGS_srcFile << "." << endl;
		exit(0);
	}
	input.close();

	cout<<"#Processors : "<<hw.processors_size()<<endl;
	cout<<"\t"<<setw(5)<<"ID"<<setw(15)<<"NAME"<<setw(10)<<"SPEED"<<endl;
	for (int i=0; i<hw.processors_size(); i++) {
		Processor p = hw.processors(i);
		cout<<"\t"<<setw(5)<<p.id()<<setw(15)<<p.name()<<setw(10)<<p.speed();
		cout<<endl;
	}
	cout<<endl;

	cout<<"#Links : "<<hw.links_size()<<endl;
	cout<<"\t"<<setw(10)<<"FROM"<<setw(10)<<"TO"<<setw(15)<<"BANDWIDTH"<<endl;
	for (int i=0; i<hw.links_size(); i++) {
		Link l = hw.links(i);
		cout<<"\t"<<setw(10)<<l.id1()<<setw(10)<<l.id2()<<setw(15)<<l.speed();
		cout<<endl;
	}

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

