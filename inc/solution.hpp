#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include <parser.hpp>

#include <string>
#include <list>

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
//		int speed;
		processor_t (int pID) { //, int pSpeed) {
			id = pID;
//			speed = pSpeed;
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
	std::list<processor_t*> processor_list;

	/*! \brief Task set */
	std::list<task_t*> task_list;

	/*! \brief Dependency set */
	std::list<dependency_t*> dependency_list;

	/*! \brief Makespan */
	int duration;

	/*! \brief Elapsed time for searching the solution */
	double requiredTime;

	/*! \brief Check dependencies */
	bool validate_deps (Parser* par);

	/*! \brief Check task durations */
	bool validate_tasks (Parser* par);

	/*! \brief Check task overlapping */
	bool validate_procs (Parser* par);

protected:

	/*! \brief return the specified processor
	 *
	 *  \param id desired processor ID
	 *  \return the desired processor
	 */
	processor_t* getProcessor (int id);

	/*! \brief return the specified dependency
	 *
	 */
	dependency_t* getDependency (int id);

	/*! \brief return the task
	 *
	 */
	task_t* getTask (int id);

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

	/*! \brief return the number of processors
	 *
	 */
	int getProcessors () const {
		return processor_list.size();
	}

	/*! \brief return the number of tasks
	 *
	 */
	int getTasks () const {
		return task_list.size();
	}

	/*! \brief return the task's start time
	 *
	 */
	int getTaskStart (int id) const {
		for (std::list<task_t*>::const_iterator it=task_list.begin(); it!=task_list.end(); it++)
			if ((*it)->id==id)
				return (*it)->start;
		return -1;
	}

	/*! \brief return the task's duration
	 *
	 */
	int getTaskDuration (int id) const {
		for (std::list<task_t*>::const_iterator it=task_list.begin(); it!=task_list.end(); it++)
			if ((*it)->id==id)
				return (*it)->duration;
		return -1;
	}

	/*! \brief return the task's finish time
	 *
	 */
	int getTaskStop (int id) const {
		for (std::list<task_t*>::const_iterator it=task_list.begin(); it!=task_list.end(); it++)
			if ((*it)->id==id)
				return (*it)->start+(*it)->duration;
		return -1;
	}

	/*! \brief return the task's processor
	 *
	 */
	int getTaskProcessor (int id) const {
		for (std::list<task_t*>::const_iterator it=task_list.begin(); it!=task_list.end(); it++)
			if ((*it)->id==id)
				return (*it)->processorId;
		return -1;
	}

	/*! \brief return the number of dependecies
	 *
	 */
	int getDependencies () const {
		return dependency_list.size();
	}

	/*! \brief add a processor
	 *	
	 *  \param id processor ID
	 *  \param speed processor speed
	 */
	void addProcessor (int id) {//, int speed) {
		processor_list.push_back(new processor_t(id));//, speed));
	}

	/*! \brief add a task */
	void addTask (int id, int start, int duration, int processorId) {
		task_list.push_back(new task_t(id, start, duration, processorId));
	}

	/*! \brief add a dependency */
	void addDependency (int id, int fromTaskId, int toTaskId, int duration) {
		dependency_list.push_back(new dependency_t(id, fromTaskId, toTaskId,
																	 duration));
	}

	/*! \brief Return the makespan, which is the shortest found
	 */
	int getSolution() const {
		return duration;
	}

	/*! \brief return the amount of time taken to find the actual solution (s)
	 */
	double getTime() {
		return requiredTime;
	}

	/*! \brief check if the actual solution is valid or not
	 *
	 */
	virtual bool validate (Parser* par);


	virtual ~Solution();
};

#endif //__SOLUTION_H__

