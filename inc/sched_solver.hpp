#ifndef __SCHED_SOLVER__
#define __SCHED_SOLVER__

#include <vector>
#include <solution.hpp>
#include <parser.hpp>

#include <constraint_solver/constraint_solver.h>


/*! \brief concreate Constraint Programming solver
 *
 * This class implements a CP solver (it derives from generic_solver and
 * Solver).
 *
 * \author Mario Bambagini
 */
class SchedulingSolver: public operations_research::Solver
{

	/*! \brief Original task set */
	std::vector<operations_research::IntervalVar*> tasks;

	//Other variables to model efficiently the tasks mapped on the CPUs.
	//tasks_to_cpus and cpus_to_tasks are the copies of the original tasks but
	//they ARE optionals.
	//tasks_to_cpus_presences and cpus_to_tasks_presences indicate if a task
	//is actually mapped into a node.
	//Of course the dual data structures share the cells. They are only a 
	//different way for reading the same information.
	//Such a redundancy is not really needed, but it simplifies the constraint
	//writing
	std::vector<std::vector<operations_research::IntervalVar*> > tasks_to_cpus;
	std::vector<std::vector<operations_research::IntervalVar*> > cpus_to_tasks;
	std::vector<std::vector<operations_research::IntVar*> >
														tasks_to_cpus_presences;
	std::vector<std::vector<operations_research::IntVar*> >
														cpus_to_tasks_presences;

	/*! The actual task allocation: this vector indicates, for each task, where
	 * it is allocated (which CPU hosts it)
	 */
	std::vector<operations_research::IntVar*> task_location;

	/*! \brief variable used from the module which calls the shots */
	std::vector<operations_research::SequenceVar*> task_sequences;

	/*! \brief this variable contains the problem information */
	Parser* parser;

	/*! \brief solution makespan */
	int makespan;

	/*! \brief Solution collector */
	operations_research::SolutionCollector* collector;

	/*! \brief initialize all the internal variables */
	void initializeVars ();

	/*! \brief create all the constraints */
	void makeConstraints ();

	/*! \brief define how often print output information
	 * 
	 * an output line is printed every howOftenToPrint branches
	 */
	int howOftenToPrint;

	/*! \brief search time limit (seconds)
	 * 
	 * The maximum amount time that the searching routine can last 
	 */
	int timeLimit;

public:

	/*! \brief Constructor
	 *
	 */
	SchedulingSolver(): Solver("Scheduler") {
		collector = NULL;
		makespan = -1;
		parser = NULL;
		howOftenToPrint = -1;
		timeLimit = -1;
	}

	/*! \brief set after how many branches print information
	 *
	 * \param branches after how many branches to print the searching state
	 */
	void setPrintRate (int branches) {
		howOftenToPrint = branches;
	}

	/*! \brief search time limit
	 *
	 * \param timeLimit maximum amount of time that the solver can last
	 */
	void setTimeLimit (int timeLimit) {
		this->timeLimit = timeLimit;
	}

	/*! \brief set the parser to use
	 *
	 */
	void setParser (Parser* p) {
		parser = p;
	}

	/*! \brief run the solving process
	 *
	 * \return true if a solution can be read, false otherwise
	 */
	bool solve ();

	/*! \brief get the solution
	 *
	 * \return the solution representing the schedule
	 */
	Solution getResult ();

	/*! \brief return the solution makespan
	 *
	 */
	int getMakespan () {
		return makespan;
	}

	/*! \brief Destructor
	 *
	 */
	virtual ~SchedulingSolver () {};

};


#endif //__SCHED_SOLVER__

