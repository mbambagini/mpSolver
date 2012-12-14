#include <gnuplot_printer.hpp>

using namespace std;


//TODO: schedule dimensions have to be managed differently
#define HEIGHT_PER_PROCESSOR 100
#define HEIGHT_TASK 80


void gnuplot_printer::draw_header (ofstream& file)
{
	file<<"set title \""<<getTime()<<"\""<<endl<<endl;

	file<<"set xtics font \"Times-Roman, 28\"  offset 0,-1"<<endl;
	file<<"set ytics font \"Times-Roman, 28\"  offset -1,-0"<<endl<<endl;

	file<<"set xlabel \"Time\" font \"Times-Roman, 34\" offset -0,-2"<<endl;
	file<<"set ylabel \"Processors\" font \"Times-Roman, 34\" offset -8, -1";
	file<<endl<<endl;

	for (int i=0; i<getProcessors(); i++) {
		file<<"set label \""<<i<<" ("<<getProcessor(i)->typeId<<")\" at -17,";
		file<<HEIGHT_PER_PROCESSOR*(i+0.5);
		file<<" font \"Helvetica,25\""<<endl;
	}

	file<<endl<<endl;
}

void gnuplot_printer::draw_footer (ofstream& file)
{
	file<<"set arrow "<<getDependencies()+1<<" filled from 0,0 to 0,";
	file<<HEIGHT_PER_PROCESSOR*getProcessors()<<" size 10,10 nohead"<<endl;
	file<<"set arrow "<<getDependencies()+2<<" filled from 0,0 to ";
	file<<getSolution()+50<<",0 size 10,10"<<endl<<endl;

	file<<"set arrow "<<getDependencies()+3<<" filled from "<<getSolution();
	file<<",0 to "<<getSolution()<<","<<HEIGHT_PER_PROCESSOR*getProcessors();
	file<<" size 10,10 nohead ls 4 lc rgb \"red\""<<endl;

	file<<"set label \""<<getSolution()<<"\" at "<<getSolution()-7<<",-15";
	file<<" font \"Helvetica,25\" tc rgb \"red\""<<endl;

	file<<"set noborder"<<endl<<endl;

	file<<"unset ytics"<<endl<<endl;

	file<<"plot [0:"<<getSolution()<<"] [0:";
	file<<HEIGHT_PER_PROCESSOR*getProcessors()<<"] NaN notitle"<<endl<<endl;
}

void gnuplot_printer::draw_tasks (ofstream& file)
{
	//TODO: colours have to be managed differently
	static string colours[] = {"red", "yellow", "green", "gray", "orange",
							   "brown",	"blue", "aquamarine", "bisque", "cyan"};

	for (int i=0; i<getTasks(); i++) {
		task_t* task = getTask(i);
		file<<"set object "<<task->id+1<<" rectangle ";
		file<<" from "<<task->start<<",";
		file<<task->processorId*HEIGHT_PER_PROCESSOR;
		file<<" to "<<task->start+task->duration<<",";
		file<<task->processorId*HEIGHT_PER_PROCESSOR+HEIGHT_TASK;
		file<<" fs pattern 2";
		if (task->typeId!=-1)
			file<<" fc rgb \""<<colours[task->typeId]<<"\"";
//		else
//			file<<" fc rgb \"cyan\"";
		file<<endl;

		file<<"set label \""<<task->id<<"\\n("<<task->duration<<")\" at ";
		file<<task->start+task->duration/2;
		file<<","<<task->processorId*HEIGHT_PER_PROCESSOR+HEIGHT_TASK/2;
		file<<" font \"Helvetica,25\""<<endl;
	}
}

void gnuplot_printer::draw_dependencies (ofstream& file)
{
	for (int i=0; i<getDependencies(); i++) {
		dependency_t* dep = getDependency(i);
		task_t* from = getTask(dep->fromTaskId);
		task_t* to = getTask(dep->toTaskId);
		file<<"set arrow "<<dep->id+1<<" filled ";
		file<<" from "<<from->start+from->duration<<",";
		file<<HEIGHT_PER_PROCESSOR*from->processorId+HEIGHT_TASK/2;
		file<<" to "<<to->start<<",";
		file<<HEIGHT_PER_PROCESSOR*to->processorId+HEIGHT_TASK/2;
		file<<"size 10,10"<<endl;
	}
}


int gnuplot_printer::print (std::string filename)
{
	ofstream file;
	file.open(filename.c_str());

	file<<"set terminal postscript eps enhanced  size 6,3.5 color"<<endl;
	file<<"set output \""<<filename<<".eps\""<<endl<<endl;

	draw_header(file);
	draw_tasks(file);
	draw_dependencies(file);
	draw_footer(file);

	file.close();

	return 1;
}

