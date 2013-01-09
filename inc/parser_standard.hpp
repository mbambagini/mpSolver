#ifndef __PARSER_STANDARD_H__
#define __PARSER_STANDARD_H__

#include <parser.hpp>
#include <vector>


/*! \brief Parse from protobuf models
 *
 * \todo the actual implementation is not robust at all, so be careful!
 *
 * \author Mario Bambagini
 */
class StandardParser : public Parser
{

	//tasks
	std::vector<std::string> tasks; /*<! Tasks */
	std::vector<std::vector<int> > computationTimes; /*<! Computation times for
												  each task on each processor */

	int eoh; /*<! end of horizon */

	//processors
	int nOfProcessors;  /*<! Number of processors to generate */

	//dependencies
	struct dependency {
		int from; /*<! from task ID */
		int to; /*<! to task ID */
		std::vector<std::vector<int> > communicationCosts; /*<! communication
						  delay for each possible allocation of the two tasks */
	};
	std::vector<struct dependency> dependencies; /*<! The actual dependecies
																  among tasks */

public:

	bool load (std::string taskGraph, std::string platform);

	int getTasks () const {
		return tasks.size();
	}

	std::string getTask (int id) const {
		return tasks[id];
	}

	int getEoh () const {
		return eoh;
	}

	int getProcessors () const {
		return nOfProcessors;
	}

	int getDependencies () const {
		return dependencies.size();
	}

	void getDependency (int id, int& from, int& to) const;

	int getComputationTime (int taskId, int processorId) const;

	int getCommunicationCost (int fromTaskId, int toTaskId, int fromProcessor,
														 int toProcessor) const;

	virtual ~StandardParser() {};
};

#endif //__PARSER_STANDARD_H__

