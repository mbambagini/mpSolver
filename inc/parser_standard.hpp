#ifndef __PARSER_STANDARD_H__
#define __PARSER_STANDARD_H__

#include <parser.hpp>
#include <vector>

/*! \brief parser for my standard serialization format
 *
 * \todo the actual implementation is not robust at all, so be careful!
 *
 * \author Mario Bambagini
 */
class StandardParser : public Parser
{

	//tasks
	std::vector<std::string> tasks; /*<! Number of tasks to generate */
//	int nOfTaskColours; /*<! Number of task colours (a colour indicates a group
//				 of tasks, which need to be allocated onto the same processor */
//	int nOfColouredTasks; /*<! Number of coloured tasks */
//	int *taskColours; /*<! Data structure with the task colour definition */
	std::vector<std::vector<int> > computationTimes; /*<! Computation times for
												  each task on each processor */

	int eoh; /*<! end of horizon */

	//processors
	int nOfProcessors;  /*<! Number of processors to generate */

	//dependencies
	struct dependency {
		int from;
		int to;
		std::vector<std::vector<int> > communicationCosts;
		int maxCommunicationCost;
	};
	std::vector<struct dependency> dependencies; /*<! The actual dependecies
																  among tasks */

public:

	bool load (std::string taskGraph, std::string platform);

	int getTasks () const {
		return tasks.size();
	}

	int getEoh () const {
		return eoh;
	}

	int getProcessors () const {
		return nOfProcessors;
	}

//	int getProcessor (int id) const {
//		return processorTypes[id];
//	}

//	int getColours () const {
//		return nOfTaskColours;
//	}

//	int getColour (int taskId) const {
//		return taskColours[taskId];
//	}

	int getDependencies () const {
		return dependencies.size();
	}

	void getDependency (int id, int& from, int& to) const;

	int getComputationTime (int taskId, int processorId) const;

	int getCommunicationCost (int fromTaskId, int toTaskId) const;

	int getCommunicationCost (int fromTaskId, int toTaskId, int fromProcessor,
														 int toProcessor) const;


	virtual ~StandardParser() {};
};

#endif //__PARSER_STANDARD_H__

