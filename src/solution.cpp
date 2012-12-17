#include <solution.hpp>

#include <iostream>


using namespace std;

Solution::processor_t* Solution::getProcessor (int id)
{
	list<processor_t*>::iterator i;
	for (i=processor_list.begin(); i!=processor_list.end(); i++)
		if ((*i)->id==id)
			return (*i);
	return NULL;
}

Solution::task_t* Solution::getTask (int id)
{
	list<task_t*>::iterator i;
	for (i=task_list.begin(); i!=task_list.end(); i++)
		if ((*i)->id==id)
			return (*i);
	return NULL;
}

Solution::dependency_t* Solution::getDependency (int id)
{
	list<dependency_t*>::iterator i;
	for (i=dependency_list.begin(); i!=dependency_list.end(); i++)
		if ((*i)->id==id)
			return (*i);
	return NULL;
}

bool Solution::validate (Parser* par)
{
	return validate_deps(par) && validate_tasks(par) && validate_procs(par);
}

bool Solution::validate_deps (Parser* par)
{
	bool valid = true;
	//check dependencies: it each of them is satisfied
	for (int i=0; i<par->getDependencies(); i++) {
		int from, to;
		par->getDependency(i, from, to);
		task_t* task = getTask(from);
		int total_time = task->start+task->duration+
                            par->getCommunicationCost(from, to,
                            getTask(from)->processorId,
                            getTask(to)->processorId);
		if (total_time>(getTask(to)->start)) {
			cerr<<"Dependency "<<i<<" not satisfied, "<<from<<"->"<<to<<endl;
			valid = false;
		}
	}
	return valid;
}

bool Solution::validate_tasks (Parser* par)
{
	bool valid = true;
	//check task durations: if each of them lasts for the exact amount of time
	//considering the hosting processor
	for (int i=0; i<getTasks(); i++) {
		task_t* task = getTask(i);
		if (task->duration!=par->getComputationTime(task->id,
														   task->processorId)) {
			cerr<<"Task "<<i<<" elapses not as expected"<<endl;
			valid = false;
		}
	}
	return valid;
}

bool Solution::validate_procs (Parser* par)
{
	bool valid = true;
	//check task overlappings: if for each processor there is any task
	//overlapping
	for (int i_p=0; i_p<getProcessors(); i_p++) {
		for (int i_t_1=0; i_t_1<(getTasks()-1); i_t_1++) {
			int location_1 = getTask(i_t_1)->processorId;
			int start_1 = getTask(i_t_1)->start;
			int finish_1 = start_1+getTask(i_t_1)->duration;
			if (start_1==finish_1)
				continue;
			for (int i_t_2=i_t_1+1; i_t_2<getTasks(); i_t_2++) {
				if (location_1==(getTask(i_t_2)->processorId)) {
					int start_2 = getTask(i_t_2)->start;
					int finish_2 = start_2+getTask(i_t_2)->duration;
					if (start_2==finish_2)
						continue;
					if (! ((start_1<start_2 && finish_1<=finish_2) ||
						  (start_1>=start_2 && finish_1>finish_2))) {
						cerr<<"Task "<<i_t_1<<" and "<<i_t_2;
						cerr<<" are overlapped"<<endl;
						valid = false;
					}
				}
			}
		}
	}
	return valid;
}

Solution::~Solution ()
{
/*
	for (list<processor_t*>::iterator i=processor_list.begin();
												   i!=processor_list.end(); i++)
		delete (*i);

	for (list<task_t*>::iterator i=task_list.begin(); i!=task_list.end(); i++)
		delete (*i);

	for (list<dependency_t*>::iterator i=dependency_list.begin();
												  i!=dependency_list.end(); i++)
		delete (*i);
*/
}

