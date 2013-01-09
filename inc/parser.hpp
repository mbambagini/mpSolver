#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>


/*! \brief generic interface for parsing problem models
 *
 * This class combines the task graph and the hardware description
 *
 * author Mario Bambagini
 */
class Parser
{

public:

	/*! \brief load task graph and hardware models
	 *
	 * \param taskGraph filename containing the task graph (tasks and
	 *							 dependencies)
	 * \param platform  filename containing the hardware specifications (cores
	 *					and links)
	 * \return true if the models have been succefully loaded, false otherwise
	 */
	virtual bool load (std::string taskGraph, std::string platform) = 0;

	/*! \brief return the 'end of horizon'
	 *
	 * This value is computed as the worst case execution (max task executions
	 * and max communication delays) on a single processor
	 */
	virtual int getEoh () const = 0;

	/*! \brief return the number of tasks
	 *
	 */
	virtual int getTasks () const = 0;

	/*! \brief return the number of processors
	 *
	 */
	virtual int getProcessors () const = 0;

	/*! \brief return the number of dependencies
	 *
	 */
	virtual int getDependencies () const = 0;

	/*! \brief return the task name
	 *
	 * \param id task ID
	 */
	virtual std::string getTask (int id) const = 0;

	/*! \brief given a dependency ID, provide the two linked task IDs
	 *
	 * \param id dependency identificator
	 * \param from id of the task which has to end before the start of the other
	 * \param to id of the task which has to start after the end of the other
	 */
	virtual void getDependency(int id, int& from, int& to) const = 0;

	/*! \brief get the computation time
	 *
	 *  Return the computation time of the specified task assuming that it is 
	 *  running on the specified processor
	 * 
	 * \param taskId Task identifier
	 * \param processorId Processor identificer
	 * \return required computational time
	 */
	virtual int getComputationTime (int taskId, int processorId) const = 0;

	/*! \brief get the communication cost
	 *
	 *  Return the communication cost (the required time) to satisfy the
	 *  dependency from fromTaskId to toTaskId.
	 *  The task specified by toTaskId has to start at later than or equal to
	 *  the returned time plus the end of the task specified by fromTaskId
	 *
	 * \param fromTaskId id of the task which has to end first
	 * \param toTaskId id of the task which has to start later
	 * \param fromProcessor assuming that fromTaskId runs on this processor
	 * \param toProcessor assuming that toTaskId runs on this processor
	 * \return communication delay for such configuration
	 */
	virtual int getCommunicationCost (int fromTaskId, int toTaskId,
								  int fromProcessor, int toProcessor) const = 0;

	virtual ~Parser() {}

};

#endif //__PARSER__

