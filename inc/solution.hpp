#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include <parser.hpp>

#include <string>
#include <vector>


/*! \brief This class stores the resulting schedule
 *
 * \author Mario Bambagini
 */
class Solution {

protected:

	/*! \brief available processor
	 *
	 */
	struct processor_t {
		int id; /*!< processor ID */
		processor_t (int pID) {
			id = pID;
		}
	};

	/*! \brief task execution
	 *
	 */
	struct task_t {
		std::string name; /*!< task name */
		int id; /*!< task ID */
		int start; /*!< task start time */
		int duration; /*!< task duration */
		int processorId; /*!< processor ID where the task is allocated on */
		task_t (std::string tname, int tID, int tStart, int tDur, int tProc) {
			name = tname;
			id = tID;
			start = tStart;
			duration = tDur;
			processorId = tProc;
		}
	};

	/*! \brief dependency
	 *
	 */
	struct dependency_t {
		int id; /*!< dependency ID */
		int fromTaskId; /*!< source task ID */
		int toTaskId; /*!< destination task ID */
		int overhead; /*!< communication overhead */
		dependency_t (int dID, int dFrom, int dTo, int dOverhead) {
			id = dID;
			fromTaskId = dFrom;
			toTaskId = dTo;
			overhead = dOverhead;
		}
	};

private:

	/*! \brief Processor set */
	std::vector<processor_t> processors;

	/*! \brief Task set */
	std::vector<task_t> tasks;

	/*! \brief Dependency set */
	std::vector<dependency_t> dependencies;

	/*! \brief Final makespan */
	int duration;

	/*! \brief Elapsed time for searching the solution */
	double requiredTime;

	/*! \brief Check dependency correctness
	 *
	 *  \return true if dependencies are satisfied, false otherwise
	 */
	bool validate_deps (Parser* par) const;

	/*! \brief Check task durations
	 *
	 *  \return true if task durations are correct, false otherwise
	 */
	bool validate_tasks (Parser* par) const;

	/*! \brief Check task overlapping
	 *
	 *  \return true if there are not task overlappings, false otherwise
	 */
	bool validate_procs (Parser* par) const;

public:

	/*! \brief Constructor
	 *
	 *  \param sol shortest makespan
	 *  \param t required time to find the solution
	 */
	Solution (int sol, double t) {
		duration = sol;
		requiredTime = t;
	}

	/*! \brief Destructor
	 *
	 */
	virtual ~Solution() {}


/**************************** INFORMATION RETRIEVAL ***************************/

	/*! \brief return the number of processors
	 *
	 */
	int getProcessors () const {
		return processors.size();
	}

	/*! \brief return the number of tasks
	 *
	 */
	int getTasks () const {
		return tasks.size();
	}

	/*! \brief return the number of dependecies
	 *
	 */
	int getDependencies () const {
		return dependencies.size();
	}

	/*! \brief return the task's name
	 *
	 */
	std::string getTaskName (int id) const {
		return tasks[id].name;
	}

	/*! \brief return the task's start time
	 *
	 */
	int getTaskStart (int id) const {
		return tasks[id].start;
	}

	/*! \brief return the task's duration
	 *
	 */
	int getTaskDuration (int id) const {
		return tasks[id].duration;
	}

	/*! \brief return the task's finish time
	 *
	 */
	int getTaskStop (int id) const {
		return tasks[id].start+tasks[id].duration;
	}

	/*! \brief return the task's processor
	 *
	 */
	int getTaskProcessor (int id) const {
		return tasks[id].processorId;
	}

	/*! \brief return the number of dependecies
	 *
	 */
	int getDependencyFrom (int id) const {
		return dependencies[id].fromTaskId;
	}

	/*! \brief return the number of dependecies
	 *
	 */
	int getDependencyTo (int id) const {
		return dependencies[id].toTaskId;
	}

	/*! \brief Return the makespan, which is the shortest found
	 */
	int getSolution() const {
		return duration;
	}

	/*! \brief return the amount of time taken to find the actual solution
	 */
	double getTime() const {
		return requiredTime;
	}


/***************************** INFORMATION SETTING ****************************/

	/*! \brief add a processor
	 *	
	 *  \param id processor ID
	 */
	void addProcessor (int id) {
		processors.push_back(processor_t(id));
	}

	/*! \brief add a task */
	void addTask (std::string name, int id, int start, int duration,
															  int processorId) {
		tasks.push_back(task_t(name, id, start, duration, processorId));
	}

	/*! \brief add a dependency */
	void addDependency (int id, int fromTaskId, int toTaskId, int duration) {
		dependencies.push_back(dependency_t(id, fromTaskId, toTaskId, duration));
	}


/********************************* VALIDATION *********************************/

	/*! \brief check if the actual solution is valid or not
	 *
	 *  \return it returns true if everything is correct, false otherwise
	 */
	virtual bool validate (Parser* par) const {
		return validate_deps(par) && validate_tasks(par) && validate_procs(par);
	}

};


#endif //__SOLUTION_H__

