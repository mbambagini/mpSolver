#include <iostream>
#include <fstream>

#include <gflags/gflags.h>

#include <parser_standard.hpp>
#include <solution.hpp>
#include <sched_solver.hpp>
#include <gnuplot_printer.hpp>


DEFINE_string (taskGraphFile, "", "Problem description file - Task set [in]");
DEFINE_string (platformFile, "", "Problem description file - Hardware [in]");
DEFINE_string (gnuplotFile, "", "Gnuplot script with the schedule [out]");

DEFINE_int32 (print, 0, "After how many branches to print state [in]");
DEFINE_int32 (limit, 0, "Time limit for the searching (seconds) [in]");


void fatal_error (string str);


int main (int argc, char* argv[])
{

	google::ParseCommandLineFlags(&argc, &argv, false);

	srand(time(NULL));

	//check arguments
	if (FLAGS_taskGraphFile.compare("")==0 || FLAGS_platformFile.compare("")==0)
		fatal_error("Wrong parameters");

	//parse the problem
	StandardParser parser;
	if (!parser.load(FLAGS_taskGraphFile, FLAGS_platformFile))
		fatal_error("Impossible to parse the models");

	//find the optimal schedule
	SchedulingSolver sol;
	sol.setParser(&parser);
	sol.setPrintRate(FLAGS_print);
	sol.setTimeLimit(FLAGS_limit);
	if (sol.solve()) {
		//print the schedule
		GnuplotPrinter p;
		const Solution s = sol.getResult();
		if (!s.validate(&parser))
			fatal_error("Problems during the schedule check");
		if (FLAGS_gnuplotFile.compare("")!=0) {
			std::ofstream file(FLAGS_gnuplotFile.c_str());
			p.print(s, file);
			file.close();
		}
	} else
		std::cout<<"Solution not found\n";

	return 0;
}


void fatal_error (string str)
{
	std::cerr<<"Error: "<<str<<std::endl;
	exit(0);
}

