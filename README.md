mpSolver
========

This program exploits the Contraint Programming feature of (OR-Tools
https://code.google.com/p/or-tools/) to implement an optimal solver for the
task scheduling problem.

More precisely, given the model of the hardware (number and type of processors
and communication links) and the workload (in terms of DAG - see
https://github.com/mbambagini/taskdep), the task-to-CPU assignment and 
scheduling which minimizes the makespan is found.
