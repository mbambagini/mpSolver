#include <parser_standard.hpp>

#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include <hardware.pb.h>
#include <taskgraph.pb.h>


using namespace std;


bool StandardParser::load (std::string taskGraph, std::string platform)
{
	TaskGraph::Graph sw;
	Hardware::Platform hw;

	fstream input_sw(taskGraph.c_str(), ios::in | ios::binary);
	if (!sw.ParseFromIstream(&input_sw))
		return false;
	fstream input_hw(platform.c_str(), ios::in | ios::binary);
	if (!hw.ParseFromIstream(&input_hw))
		return false;
	input_hw.close();
	input_sw.close();

	//TASKS
	tasks.resize(sw.tasks_size());
	computationTimes.resize(sw.tasks_size());
	eoh = 0;
	for (int i=0; i<sw.tasks_size(); i++) {
		tasks[i] = sw.tasks(i).name();
		computationTimes[i].resize(hw.processors_size());
		int maxWcet = 0;
		for (int j=0; j<hw.processors_size(); j++) {
			computationTimes[i][j] =
				 trunc((double)(sw.tasks(i).wcet())/(hw.processors(j).speed()));
			maxWcet = max(maxWcet, computationTimes[i][j]);
		}
		eoh += maxWcet;
	}

	//PROCESSORS
	nOfProcessors = hw.processors_size();

	//DEPENDENCIES
	dependencies.resize(sw.dependencies_size());
	for (unsigned int i=0; i<dependencies.size(); i++) {

		int from = -1, to = -1;
		for (unsigned int j=0; j<tasks.size() && (from==-1 || to==-1); j++) {
			if (from==-1 && tasks[j].compare(sw.dependencies(i).fromtask())==0)
				from = j;
			if (to==-1 && tasks[j].compare(sw.dependencies(i).totask())==0)
				to = j;
		}
		if (from==-1 || to==-1)
			continue;
		dependencies[i].from = from;
		dependencies[i].to = to;
		dependencies[i].maxCommunicationCost = 0;
		dependencies[i].communicationCosts.resize(nOfProcessors);
		for (int j=0; j<nOfProcessors; j++)
			dependencies[i].communicationCosts[j].resize(nOfProcessors);
		for (int j=0; j<hw.links_size(); j++) {
			int val = trunc((double)(sw.dependencies(i).exchangeddata())/
														 (hw.links(j).speed()));
			int id1 = hw.links(j).id1();
			int id2 = hw.links(j).id2();
			dependencies[i].communicationCosts[id1][id2] = val;
			dependencies[i].communicationCosts[id2][id1] = val;
			dependencies[i].maxCommunicationCost = max(val,
										  dependencies[i].maxCommunicationCost);
		}

		eoh += dependencies[i].maxCommunicationCost;
	}

	return true;
}

void StandardParser::getDependency (int id, int& from, int& to) const
{
	from = dependencies[id].from;
	to = dependencies[id].to;
}

int StandardParser::getComputationTime (int taskId, int processorId) const
{
	return computationTimes[taskId][processorId];
}

int StandardParser::getCommunicationCost (int fromTaskId, int toTaskId,
									   int fromProcessor, int toProcessor) const
{
	for (unsigned int i=0; i<dependencies.size(); i++) {
		if (dependencies[i].from==fromTaskId && dependencies[i].to==toTaskId)
			return
				 dependencies[i].communicationCosts[fromProcessor][toProcessor];
	}
	return 0;
}

int StandardParser::getCommunicationCost (int fromTaskId, int toTaskId) const
{
	for (unsigned int i=0; i<dependencies.size(); i++)
		if (dependencies[i].from==fromTaskId && dependencies[i].to==toTaskId)
			return dependencies[i].maxCommunicationCost;
	return 0;
}

/*
StandardParser::~StandardParser()
{
	delete[] taskColours;
	delete[] processorTypes;

	for (int i=0; i<nOfTasks; i++)
		delete[] computationTimes[i];
	delete[] computationTimes;

	for (int i=0; i<nOfDependencies; i++) {
		for (int j=0; j<nOfProcessors; j++)
			delete[] dependencies[i].communicationCosts[j];
		delete[] dependencies[i].communicationCosts;
	}
	delete[] dependencies;
}
*/

