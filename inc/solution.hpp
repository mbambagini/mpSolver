#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include <parser.hpp>

#include <string>
#include <vector>

/*! \brief This class stores a schedule
 *
 * \author Mario Bambagini
 */
class Solution {

protected:

	/*! \brief 
	 *
	 */
	struct processor_t {
		int id;
		processor_t (int pID) {
			id = pID;
		}
	};

	/*! \brief task execution
	 *
	 */
	struct task_t {
		int id;
		int start;
		int duration;
		int processorId;
		task_t (int tID, int tStart, int tDur, int tProc) {
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
		int id;
		int fromTaskId;
		int toTaskId;
		int overhead;
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

	/*! \brief Makespan */
	int duration;

	/*! \brief Elapsed time for searching the solution */
	double requiredTime;

	/*! \brief Check dependencies */
	bool validate_deps (Parser* par) const;

	/*! \brief Check task durations */
	bool validate_tasks (Parser* par) const;

	/*! \brief Check task overlapping */
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
/*
		for (std::list<task_t*>::const_iterator it=task_list.begin();
													  it!=task_list.end(); it++)
			if ((*it)->id==id)
				return (*it)->duration;
		return -1;
*/
	}

	/*! \brief return the task's finish time
	 *
	 */
	int getTaskStop (int id) const {
		return tasks[id].start+tasks[id].duration;
/*
		for (std::list<task_t*>::const_iterator it=task_list.begin();
													  it!=task_list.end(); it++)
			if ((*it)->id==id)
				return (*it)->start+(*it)->duration;
		return -1;
*/
	}

	/*! \brief return the task's processor
	 *
	 */
	int getTaskProcessor (int id) const {
		return tasks[id].processorId;
/*
		for (std::list<task_t*>::const_iterator it=task_list.begin();
													  it!=task_list.end(); it++)
			if ((*it)->id==id)
				return (*it)->processorId;
		return -1;
*/
	}

	/*! \brief return the number of dependecies
	 *
	 */
	int getDependencies () const {
		return dependencies.size();
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

	/*! \brief return the amount of time taken to find the actual solution (s)
	 */
	double getTime() const {
		return requiredTime;
	}

/***************************** INFORMATION SETTING ****************************/

	/*! \brief add a processor
	 *	
	 *  \param id processor ID
	 *  \param speed processor speed
	 */
	void addProcessor (int id) {//, int speed) {
		processors.push_back(processor_t(id));//, speed));
	}

	/*! \brief add a task */
	void addTask (int id, int start, int duration, int processorId) {
		tasks.push_back(task_t(id, start, duration, processorId));
	}

	/*! \brief add a dependency */
	void addDependency (int id, int fromTaskId, int toTaskId, int duration) {
		dependencies.push_back(dependency_t(id, fromTaskId, toTaskId, duration));
	}


/********************************* VALIDATION *********************************/


	/*! \brief check if the actual solution is valid or not
	 *
	 */
	virtual bool validate (Parser* par) const;

};

#endif //__SOLUTION_H__

