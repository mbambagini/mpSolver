#ifndef __GNUPLOT_PRINTER_H__
#define __GNUPLOT_PRINTER_H__

#include <solution.hpp>

#include <fstream>

/*! \brief Generate a Gnuplot script to represent the scheduling
 *
 * \author Mario Bambagini
 */
class gnuplot_printer : public solution {

	/*! \brief Print the script header
	 */
	void draw_header (std::ofstream& file);

	/*! \brief Print the script footer
	 */
	void draw_footer (std::ofstream& file);

	/*! \brief Print tasks
	 */
	void draw_tasks (std::ofstream& file);

	/*! \brief Print dependency arrows
	 */
	void draw_dependencies (std::ofstream& file);

public:

	int print (std::string filename);

};

#endif //GNUPLOT_PRINTER

