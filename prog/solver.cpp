#include <gflags/gflags.h>

#include <base/commandlineflags.h>
#include <base/commandlineflags.h>

#include <iostream>

#include <parser_standard.hpp>
#include <solution.hpp>

#include <sched_solver.hpp>

//#include <gnuplot_printer.hpp>


DEFINE_string (taskGraphFile, "", "Problem description file [input]");
DEFINE_string (platformFile, "", "Problem description file [input]");
DEFINE_string (outputFile, "",
					 "Gnuplot script file with the scheduling result [output]");


int main (int argc, char* argv[])
{

	google::ParseCommandLineFlags(&argc, &argv, false);

	srand(time(NULL));


	Parser *parser = new StandardParser();
//	Solution *sol = new Solugnuplot_printer();

	//parse the problem
	if (FLAGS_taskGraphFile.compare("")==0 ||
		FLAGS_platformFile.compare("")==0 ||  parser->load(FLAGS_taskGraphFile, FLAGS_platformFile)) {
		std::cout<<"ERROR"<<std::endl;
		exit(0);
	}

	//solve and save the scheduling if possible
	SchedulingSolver s;
	s.setParser(parser);
	if (s.solve()) {
/*
		s.getResult(sol);
		if (!sol->validate(parser))
			std::cout<<"Solution not valid\n";
		else
			if (FLAGS_outputFile.compare("")!=0)
				sol->print(FLAGS_outputFile);
*/
	} else
		std::cout<<"Solution not found\n";

	delete parser;
//	delete sol;

	return 0;
}

