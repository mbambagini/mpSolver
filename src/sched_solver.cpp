#include <cstdio>
#include <cstdlib>

#include <iostream>

/*
#include <base/commandlineflags.h>
#include <base/commandlineflags.h>
#include <base/integral_types.h>
#include <base/logging.h>
#include <base/stringprintf.h>
#include <base/strtoint.h>
#include <base/file.h>
#include <base/split.h>
*/

#include <sched_solver.hpp>


using namespace std;


void SchedulingSolver::initializeVars ()
{
	for (int i=0; i<parser->getTasks(); i++) {
		cout<<"Task "<<i<<":";
		for (int j=0; j<parser->getProcessors(); j++)
			cout<<" "<<parser->getComputationTime(i, j);
		cout<<endl;
	}
	cout<<"EOH: "<<parser->getEoh()<<endl;

	for (int i=0; i<parser->getTasks(); i++) {
		tasks.push_back(MakeFixedDurationIntervalVar (
								0, //minimum start time
								parser->getEoh()*2, //maximum start time
								0,//parser->getTaskDuration(i), //duration
								false,	//not optional
								//the original version IS NOT optional
								StringPrintf("Task_%d", i))); //name
	}

	tasks_to_cpus.resize(parser->getTasks());
	cpus_to_tasks.resize(parser->getTasks());
	tasks_to_cpus_presences.resize(parser->getTasks());
	cpus_to_tasks_presences.resize(parser->getProcessors());
	for (int i=0; i<parser->getTasks(); i++) {
		tasks_to_cpus[i].resize(parser->getProcessors());
		tasks_to_cpus_presences[i].resize(parser->getProcessors());
	}
	for (int i=0; i<parser->getProcessors(); i++) {
		cpus_to_tasks[i].resize(parser->getTasks());
		cpus_to_tasks_presences[i].resize(parser->getTasks());
	}
	for (int i_task=0; i_task<parser->getTasks(); i_task++) {
		for (int i_cpu=0; i_cpu<parser->getProcessors(); i_cpu++) {
			//Copy of the task
			IntervalVar* task_copy = MakeFixedDurationIntervalVar(0,
							  parser->getEoh(),
							  parser->getComputationTime(i_task, i_cpu),
							  true, StringPrintf("copy_Task_%d", i_task+1));
			tasks_to_cpus[i_task][i_cpu] = task_copy;
			cpus_to_tasks[i_cpu][i_task] = task_copy;
			//Presences
			IntVar* tmp = MakeBoolVar();
			cpus_to_tasks_presences[i_cpu][i_task] = tmp;
			tasks_to_cpus_presences[i_task][i_cpu] = tmp;
		}
	}

	task_location.resize(parser->getTasks());
	for (int i=0; i<parser->getTasks(); i++)
		task_location[i] = MakeIntVar(0, parser->getProcessors()-1);

	task_sequences.resize(parser->getProcessors());
}



void SchedulingSolver::makeConstraints ()
{
	//Make the tasks run on a CPU in a not overlapped way
	//It also creates a sequence variable for each CPU
	for (int i=0; i<parser->getProcessors(); i++) {
		DisjunctiveConstraint* ct = MakeDisjunctiveConstraint(
								   cpus_to_tasks[i], StringPrintf("cpu_%d", i));
		task_sequences[i] = ct->MakeSequenceVar();
		AddConstraint(ct);
	}

	//Synchonize the task set with the actual mapped tasks.
	for (int i_cpu=0; i_cpu<parser->getProcessors(); i_cpu++)
		for (int i_task=0; i_task<parser->getTasks(); i_task++) {
			//STARTS_AT_START is better than STAYS_IN_SYNC because the latter
			//doesn't allow you to have a different duration (it requires a 
			//perfect synchronization). In this way the function synchronizes
			//only the starting time letting you have different execution times
//			AddConstraint(MakeIntervalVarRelation(tasks[i_task],
//						Solver::STARTS_AT_START, tasks_to_cpus[i_task][i_cpu]));
			AddConstraint(MakeIntervalVarRelation(tasks[i_task],
						Solver::ENDS_AT_END, tasks_to_cpus[i_task][i_cpu]));
		}

/*
	//Dependencies
	for (int i = 0; i < parser->getDependencies(); i++) {
		int from, to;
		parser->getDependency(i, from, to);
		IntervalVar* t1 = tasks[from];
		vector<IntVar*> commCosts_task_i(parser->getProcessors()*parser->getProcessors());
cout<<"Add constraints: "<<from<<" -> "<<to<<endl;
		for (int i_cpu_from=0;i_cpu_from<parser->getProcessors();i_cpu_from++) {
			for (int i_cpu_to=0; i_cpu_to<parser->getProcessors(); i_cpu_to++) {
				IntExpr* tmp =
					  MakeProd(tasks_to_cpus_presences[from][i_cpu_from],
										 tasks_to_cpus_presences[to][i_cpu_to]);
				IntExpr* tmp2 = MakeProd(tmp,
										  parser->getCommunicationCost(from, to,
														 i_cpu_from, i_cpu_to));
				commCosts_task_i[i_cpu_from*parser->getProcessors()+i_cpu_to] = tmp2->Var();
cout<<"\ton "<<i_cpu_from<<", "<<i_cpu_to<<": "<<parser->getCommunicationCost(from, to, i_cpu_from, i_cpu_to)<<endl;
			}
		}
		IntVar* sum_task_i = MakeSum(commCosts_task_i)->Var();
		for (int i_cpu_to=0; i_cpu_to<parser->getProcessors(); i_cpu_to++) {
			IntervalVar* t2 = tasks_to_cpus[to][i_cpu_to];
//			Constraint* prec = MakeGreaterOrEqual(t2->StartExpr()->Var(),
//							   MakeSum(t1->EndExpr(),sum_task_i)->Var());
			Constraint* prec = MakeLessOrEqual(
									  MakeSum(t1->EndExpr(), sum_task_i)->Var(),
														t2->StartExpr()->Var());
			AddConstraint(prec);
		}
	}
*/

	//Dependencies
	for (int i = 0; i < parser->getDependencies(); i++) {
		int from, to;
		parser->getDependency(i, from, to);
		IntervalVar* t1 = tasks[from];
		vector<IntVar*> commCosts;
		for (int i_from=0; i_from<parser->getProcessors(); i_from++) {
			for (int i_to=0; i_to<parser->getProcessors(); i_to++) {
				IntExpr* tmp = MakeProd(tasks_to_cpus_presences[from][i_from],
											 tasks_to_cpus_presences[to][i_to]);
				IntExpr* tmp2 = MakeProd(tmp,
										  parser->getCommunicationCost(from, to,
														 i_from, i_to));
				commCosts.push_back(tmp2->Var());
			}
		}
		IntVar* commCostVar = MakeSum(commCosts)->Var();

		vector<IntVar*> next(parser->getProcessors());
		for (int i_cpu=0; i_cpu<parser->getProcessors(); i_cpu++)
			next[i_cpu] = MakeProd(tasks_to_cpus[to][i_cpu]->StartExpr()->Var(),
									 tasks_to_cpus_presences[to][i_cpu])->Var();
		IntVar* sum = MakeSum(next)->Var();

		Constraint* prec = MakeLessOrEqual(t1->EndExpr()->Var(), MakeSum(sum, commCostVar)->Var());
		AddConstraint(prec);
	}

/*
	//Dependencies
	for (int i = 0; i < parser->getDependencies(); i++) {
		int from, to;
		parser->getDependency(i, from, to);
		IntervalVar* t1 = tasks[from];
		vector<IntVar*> next(parser->getProcessors());
		for (int i_cpu=0; i_cpu<parser->getProcessors(); i_cpu++)
			next[i_cpu] = MakeProd(tasks_to_cpus[to][i_cpu]->StartExpr()->Var(),
									 tasks_to_cpus_presences[to][i_cpu])->Var();
		IntVar* sum = MakeSum(next)->Var();
		Constraint* prec = MakeLessOrEqual(t1->EndExpr()->Var(), sum);
		AddConstraint(prec);
	}
*/

	//Force the situation in which a mapped task is performed iif the relative
	//presences matrix says that and viceversa
	for (int i_cpu=0; i_cpu<parser->getProcessors(); i_cpu++)
		for (int i_task=0; i_task<parser->getTasks(); i_task++) {
			AddConstraint(MakeEquality(tasks_to_cpus_presences[i_task][i_cpu],
						 tasks_to_cpus[i_task][i_cpu]->PerformedExpr()->Var()));
		}

	//Stress the condition in which only an instance for each task can be
	//executed
	for (int i_task=0; i_task<parser->getTasks(); i_task++)
		AddConstraint(MakeSumEquality(tasks_to_cpus_presences[i_task],1));

	//Relate task_location and tasks_to_cpus_presences:
	//when a task is mapped in a different location (information stored in
	//task_location) the relative presence in tasks_to_cpus_presences is set
	//as TRUE.
	//This causes, for the third constraint, that also the task copy in 
	//cpus_to_tasks results active (==to be performed)
	for (int i_task=0; i_task<parser->getTasks(); i_task++)
		AddConstraint(MakeMapDomain(task_location[i_task],
											  tasks_to_cpus_presences[i_task]));

}

bool SchedulingSolver::solve () 
{
	if (parser==NULL)
		return false;

//--------------------------------- VARIABLES ---------------------------------
	initializeVars();

//-------------------------------- CONSTRAINTS --------------------------------
	makeConstraints();

//----------------------------------- SOLVE -----------------------------------
	std::vector<SearchMonitor*> monitors;

	//---------------------------- OBJECT FUNCTION ----------------------------

	// Objective: minimize the makespan (maximum end times of all tasks)
	// of the problem.

	//Create an Int variable for each task to model the task termination
	std::vector<IntVar*> task_ends;
	for (int i = 0; i<parser->getTasks(); i++)
		for (int j = 0; j<parser->getProcessors(); j++)
			task_ends.push_back(MakeProd(tasks_to_cpus[i][j]->EndExpr()->Var(),
										 tasks_to_cpus_presences[i][j])->Var());

	IntVar* objective_var = MakeMax(task_ends)->Var();
	OptimizeVar* const objective_monitor = MakeMinimize(objective_var,1);
	monitors.push_back(objective_monitor);

	//---------------------------- DecisionBuilder ----------------------------

	std::vector<DecisionBuilder*> db;

	db.push_back(MakePhase(task_location, Solver::CHOOSE_MIN_SIZE_HIGHEST_MIN,
												  Solver::ASSIGN_CENTER_VALUE));

	db.push_back(MakePhase(tasks, Solver::INTERVAL_DEFAULT));

	db.push_back(MakePhase(task_sequences,
										Solver::CHOOSE_MIN_SLACK_RANK_FORWARD));

	db.push_back(MakePhase(objective_var, Solver::INT_VAR_DEFAULT,
													 Solver::ASSIGN_MIN_VALUE));

	// The main decision builder (ranks all tasks, then fixes the
	// objective_variable).
	DecisionBuilder* const main_phase = Compose(db);

	//---------------------------------- Log ----------------------------------

	SearchMonitor* const search_log = MakeSearchLog(100, objective_monitor);
	monitors.push_back(search_log);

	//-------------------------------- Limits ---------------------------------

//	SearchLimit* limit = MakeTimeLimit(2000000000);

	//------------------------------ Container --------------------------------

	Assignment* solution = MakeAssignment();
	solution->Add(task_location);
	for (int i=0; i<parser->getTasks(); i++) {
		solution->Add(tasks_to_cpus_presences[i]);
		solution->Add(tasks_to_cpus[i]);
	}
	solution->Add(tasks);
	solution->Add(objective_var);
	collector = MakeLastSolutionCollector(solution);
	monitors.push_back(collector);

	//------------------------------- Search ----------------------------------

	bool res = Solve(main_phase, monitors);
	makespan = collector->Value(0, objective_var);
	return res;
}



Solution SchedulingSolver::getResult ()
{
	if (collector==NULL || collector->solution_count()==0) {
		cout<<"Error 1"<<endl;
		return Solution(-1,-1);
	}

	Solution s(makespan, wall_time());
	for (int i=0; i<parser->getProcessors(); i++) {
		 s.addProcessor(i);
	}

	for (int i=0; i<parser->getTasks(); i++) {
		int i_cpu = collector->Value(0, task_location[i]);
		s.addTask (i, collector->StartValue(0, tasks_to_cpus[i][i_cpu]),
				collector->DurationValue(0, tasks_to_cpus[i][i_cpu]), i_cpu);

cout<<"Task "<<i<<": ("<<collector->StartValue(0, tasks_to_cpus[i][i_cpu]);
cout<<", "<<collector->DurationValue(0, tasks_to_cpus[i][i_cpu])+collector->StartValue(0, tasks_to_cpus[i][i_cpu]);
cout<<") on "<<i_cpu<<endl;

//		s.addTask (i, parser->getColour(i),
//				collector->StartValue(0, tasks_to_cpus[i][i_cpu]),
//				collector->DurationValue(0, tasks_to_cpus[i][i_cpu]),
//				i_cpu);
	}

	for (int i = 0; i < parser->getDependencies(); i++) {
		int from, to;
		parser->getDependency(i, from, to);
		s.addDependency(i, from, to, 0);
	}

	return s;
}

