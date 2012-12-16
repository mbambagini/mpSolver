#include <gnuplot_printer.hpp>

using namespace std;


//TODO: schedule dimensions have to be managed differently
#define HEIGHT_PER_PROCESSOR 100
#define HEIGHT_TASK 80


void GnuplotPrinter::draw_header (Solution& sol, ostream& file)
{
	file<<"set title \""<<sol.getTime()<<"\""<<endl<<endl;

	file<<"set xtics font \"Times-Roman, 28\"  offset 0,-1"<<endl;
	file<<"set ytics font \"Times-Roman, 28\"  offset -1,-0"<<endl<<endl;

	file<<"set xlabel \"Time\" font \"Times-Roman, 34\" offset -0,-2"<<endl;
	file<<"set ylabel \"Processors\" font \"Times-Roman, 34\" offset -8, -1";
	file<<endl<<endl;

	for (int i=0; i<sol.getProcessors(); i++) {
		file<<"set label \""<<i<<"\" at -17,";
		file<<HEIGHT_PER_PROCESSOR*(i+0.5);
		file<<" font \"Helvetica,25\""<<endl;
	}

	file<<endl<<endl;
}

void GnuplotPrinter::draw_footer (Solution& sol, ostream& file)
{
	file<<"set arrow "<<sol.getDependencies()+1<<" filled from 0,0 to 0,";
	file<<HEIGHT_PER_PROCESSOR*sol.getProcessors()<<" size 10,10 nohead"<<endl;
	file<<"set arrow "<<sol.getDependencies()+2<<" filled from 0,0 to ";
	file<<sol.getSolution()+50<<",0 size 10,10"<<endl<<endl;

	file<<"set arrow "<<sol.getDependencies()+3<<" filled from "<<sol.getSolution();
	file<<",0 to "<<sol.getSolution()<<","<<HEIGHT_PER_PROCESSOR*sol.getProcessors();
	file<<" size 10,10 nohead ls 4 lc rgb \"red\""<<endl;

	file<<"set label \""<<sol.getSolution()<<"\" at "<<sol.getSolution()-7<<",-15";
	file<<" font \"Helvetica,25\" tc rgb \"red\""<<endl;

	file<<"set noborder"<<endl<<endl;

	file<<"unset ytics"<<endl<<endl;

	file<<"plot [0:"<<sol.getSolution()<<"] [0:";
	file<<HEIGHT_PER_PROCESSOR*sol.getProcessors()<<"] NaN notitle"<<endl<<endl;
}

void GnuplotPrinter::draw_tasks (Solution& sol, ostream& file)
{
	for (int i=0; i<sol.getTasks(); i++) {
		file<<"set object "<<i+1<<" rectangle ";
		file<<" from "<<sol.getTaskStart(i)<<",";
//		file<<task->processorId*HEIGHT_PER_PROCESSOR;
		file<<0*HEIGHT_PER_PROCESSOR;
		file<<" to "<<sol.getTaskStop(i)<<",";
//		file<<task->processorId*HEIGHT_PER_PROCESSOR+HEIGHT_TASK;
		file<<0*HEIGHT_PER_PROCESSOR+HEIGHT_TASK;
		file<<" fs pattern 2";
		file<<endl;
		file<<"set label \""<<i<<"\\n("<<sol.getTaskDuration(i)<<")\" at ";
		file<<sol.getTaskStop(i)/2;
//		file<<","<<task->processorId*HEIGHT_PER_PROCESSOR+HEIGHT_TASK/2;
		file<<","<<0*HEIGHT_PER_PROCESSOR+HEIGHT_TASK/2;
		file<<" font \"Helvetica,25\""<<endl;
	}
}

void GnuplotPrinter::draw_dependencies (Solution& sol, ostream& file)
{
	for (int i=0; i<sol.getDependencies(); i++) {
/*
		dependency_t* dep = sol.getDependency(i);
		task_t* from = sol.getTask(dep->fromTaskId);
		task_t* to = getTask(dep->toTaskId);
		file<<"set arrow "<<dep->id+1<<" filled ";
		file<<" from "<<from->start+from->duration<<",";
		file<<HEIGHT_PER_PROCESSOR*from->processorId+HEIGHT_TASK/2;
		file<<" to "<<to->start<<",";
		file<<HEIGHT_PER_PROCESSOR*to->processorId+HEIGHT_TASK/2;
		file<<"size 10,10"<<endl;
*/
	}
}


void GnuplotPrinter::print (Solution& sol,  ostream& file)
{
//	ostream file;
//	file.open(filename.c_str());

	file<<"set terminal postscript eps enhanced  size 6,3.5 color"<<endl;
	file<<"set output \"schedules.eps\""<<endl<<endl;

	draw_header(sol, file);
	draw_tasks(sol, file);
	draw_dependencies(sol, file);
	draw_footer(sol, file);

//	file.close();
}

