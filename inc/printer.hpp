#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <solution.hpp>

#include <fstream>

class Printer
{

public:

	virtual void print (const Solution& sol,  std::ostream& file) = 0;

};


#endif //__PRINTER_H__

