#ifndef __GNUPLOT_PRINTER_H__
#define __GNUPLOT_PRINTER_H__

#include <printer.hpp>

/*! \brief Generate a Gnuplot script representing the schedule
 *
 * \author Mario Bambagini
 */
class GnuplotPrinter : public Printer
{

	/*! \brief Print the script header
	 */
	void draw_header (const Solution& sol, std::ostream& file);

	/*! \brief Print the script footer
	 */
	void draw_footer (const Solution& sol, std::ostream& file);

	/*! \brief Print tasks
	 */
	void draw_tasks (const Solution& sol, std::ostream& file);

	/*! \brief Print dependency arrows
	 */
	void draw_dependencies (const Solution& sol, std::ostream& file);

public:

	void print (const Solution& sol,  std::ostream& file);

};

#endif //GNUPLOT_PRINTER

