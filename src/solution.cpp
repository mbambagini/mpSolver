#include <solution.hpp>

#include <iostream>


using namespace std;


bool Solution::validate (Parser* par) const
{
	return validate_deps(par) && validate_tasks(par) && validate_procs(par);
}

bool Solution::validate_deps (Parser* par) const
{
	bool valid = true;
	//check dependencies: it each of them is satisfied
	for (unsigned int i=0; i<dependencies.size(); i++) {
		int from = dependencies[i].fromTaskId;
		int to = dependencies[i].toTaskId;
		int total_time = tasks[from].start+tasks[from].duration+
											 par->getCommunicationCost(from, to,
								tasks[from].processorId, tasks[to].processorId);
		if (total_time>tasks[to].start) {
			cerr<<"Dependency "<<i<<" not satisfied: "<<from<<"->"<<to<<endl;
			valid = false;
		}
	}
	return valid;
}

bool Solution::validate_tasks (Parser* par) const
{
	bool valid = true;
	//check task durations: if each of them lasts for the exact amount of time
	//considering the hosting processor
	for (unsigned int i=0; i<tasks.size(); i++) {
		if (tasks[i].duration!=
							 par->getComputationTime(i, tasks[i].processorId)) {
			cerr<<"Task "<<i<<" elapses not as expected"<<endl;
			valid = false;
		}
	}
	return valid;
}

bool Solution::validate_procs (Parser* par) const
{
	bool valid = true;
	//check task overlappings: if for each processor there is any task
	//overlapping
	for (unsigned int i_p=0; i_p<processors.size(); i_p++) {
		for (unsigned int i_t_1=0; i_t_1<(tasks.size()-1); i_t_1++) {
			int location_1 = tasks[i_t_1].processorId;
			int start_1 = tasks[i_t_1].start;
			int finish_1 = start_1+tasks[i_t_1].duration;
			if (start_1==finish_1) //null duration like for sync tasks
				continue;
			for (unsigned int i_t_2=i_t_1+1; i_t_2<tasks.size(); i_t_2++) {
				if (location_1==(tasks[i_t_2].processorId)) {
					int start_2 = tasks[i_t_2].start;
					int finish_2 = start_2+tasks[i_t_2].duration;
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

