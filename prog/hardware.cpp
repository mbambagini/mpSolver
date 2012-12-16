#include <iostream>
#include <fstream>
#include <string>

#include <limits>

#include <stdio.h>
#include <stdlib.h>

#include <hardware.pb.h>

#include <gflags/gflags.h>



using namespace std;


using namespace Hardware;


DEFINE_string(binaryFile, "", "Binary output file");



int add_processors(Platform& hw);

int add_links(Platform& hw);




int main (int argc, char* argv[])
{

	//check parameters
	google::ParseCommandLineFlags(&argc, &argv, false);

	if (FLAGS_binaryFile.compare("")==0) {
		cerr<<"The input/output file parameters are mandatory"<<endl;
		return 1;
	}

	//architecture definition 
	Platform hw;

	if (add_processors(hw)<1) {
		cerr<<"Data not valid."<<endl;
		return -2;
	}

	if (add_links(hw)<1) {
		cerr<<"Data not valid."<<endl;
		return -2;
	}
/*
	//output - plain text file
	if (FLAGS_plainTextFile.compare("")) {
		FileHardwareSerializer ser;
		fstream output(FLAGS_plainTextFile.c_str(), ios::out | ios::trunc);
		ser.serialize(output, hw);
		output.close();
	}

	//output - graphviz
	if (FLAGS_graphvizFile.compare("")) {
		GraphvizHardwareSerializer ser;
		fstream output(FLAGS_graphvizFile.c_str(), ios::out | ios::trunc);
		ser.serialize(output, hw);
		output.close();
	}
*/
	//output - serialization
	fstream output(FLAGS_binaryFile.c_str(), ios::out | ios::trunc | ios::binary);
	if (!hw.SerializeToOstream(&output)) {
		cerr << "Failed to serialize." << endl;
		return -1;
	}

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}



int add_processors(Platform& hw)
{
	cout<<"ADD PROCESSORS"<<endl;
	int id = 0;
	do {
		Processor *p = hw.add_processors();

		string name;
		cout<<"Add processor name : ";
		cin>>name;
		double speed;
		cout<<"Add speed : ";
		cin>>speed;

		p->set_id(id++);
		p->set_name(name);
		p->set_speed(speed);

		char c;
		cout<<"Add a new processor (Y/n)? ";
		cin>>c;
		if (c!='Y' && c!='y')
			break;
	} while (true);

	return 1;
}



int add_links(Platform& hw)
{
	cout<<"ADD LINKS"<<endl;
	for (int i=0; i<hw.processors_size(); i++)
		for (int j=i; j<hw.processors_size(); j++) {
			cout<<"Insert byte/second of the link between ";
			cout<<i<<" and "<<j<<": ";
			int speed;
			cin>>speed;
			Link *link = hw.add_links();
			link->set_id1(i);
			link->set_id2(j);
			link->set_speed(speed);
		}
	return 1;
}

