#ifndef __SCHED_SOLVER__
#define __SCHED_SOLVER__

#include <vector>
#include <solution.hpp>
#include <parser.hpp>

#include <constraint_solver/constraint_solver.h>


using namespace operations_research;


/*! \brief concreate implementation of the solver
 *
 * This class implements a CSP solver (it derives from generic_solver and
 * Solver).
 *
 * \author Mario Bambagini
 */
//class sched_solver: public generic_solver, private Solver
class SchedulingSolver: public Solver
{

	/*! \brief Original task set */
	std::vector<IntervalVar*> tasks;

	//Other variables to model the effective tasks mapped in to the CPU.
	//tasks_to_cpus and cpus_to_tasks are the copies of the original tasks but
	//they ARE optionals.
	//tasks_to_cpus_presences and cpus_to_tasks_presences indicate if a task
	//is actually mapped into a node.
	//Of course the dual data structures share the cells. They are also a 
	//different way for reading the same information.
	//Such a redundancy is not really needed, but it simplifies the writing of
	//the constraints
	std::vector<std::vector<IntervalVar*> > tasks_to_cpus;
	std::vector<std::vector<IntervalVar*> > cpus_to_tasks;
	std::vector<std::vector<IntVar*> > tasks_to_cpus_presences;
	std::vector<std::vector<IntVar*> > cpus_to_tasks_presences;

	/*! The actual task allocation: this vector indicates, for each task, where
	 * it is allocated (which CPU hosts it)
	 */
	std::vector<IntVar*> task_location;

	/*! \brief variable used from the module which calls the shots */
	std::vector<SequenceVar*> task_sequences;

	Parser* parser;

	//solution makespan
	int makespan;

	//Solution collector
	SolutionCollector* collector;


	//initialize all the internal variables
	void initializeVars ();

	//add all the required constraints
	void makeConstraints ();

	int howOftenToPrint;

public:

	SchedulingSolver(): Solver("Scheduler") {
		collector = NULL;
		makespan = -1;
		parser = NULL;
		howOftenToPrint = -1;
	}

	void setPrintRate (int rate) {
		howOftenToPrint = rate;
	}

	void setParser (Parser* p) {
		parser = p;
	}

	bool solve ();

	Solution getResult ();

	int getMakespan () {
		return makespan;
	}

	~SchedulingSolver() {} //not implemented yet because I have no idea about how
					 //OR-Tools' resources work
};

#endif //__SCHED_SOLVER__

