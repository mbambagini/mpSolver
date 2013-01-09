#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <solution.hpp>

#include <fstream>

/*! \brief Generic class for printing the resulting schedule
 *
 * \author Mario Bambagini
 */
class Printer
{

public:

	/*! \brief Print the schedule in a file, according to the class algorithm
	 *
	 * \param sol solution to file
	 * \param file file stream to be used
 	 */
	virtual void print (const Solution& sol,  std::ostream& file) = 0;

};

#endif //__PRINTER_H__

