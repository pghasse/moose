/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef MOOSE_H
#define MOOSE_H

// libMesh includes
#include "libmesh/perf_log.h"
#include "libmesh/parallel.h"
#include "libmesh/libmesh_common.h"
#include "XTermConstants.h"

#include <string>

using namespace libMesh;

class ActionFactory;
class Factory;

/**
 * Testing a condition on a local CPU that need to be propagated across all processes.
 *
 * If the condition 'cond' is satisfied, it gets propagated across all processes, so the parallel code take the same path (if that is requires).
 */
#define parallel_if (cond)                       \
    bool __local_bool__ = (cond);               \
    Parallel::max<bool>(__local_bool__);        \
    if (__local_bool__)

/**
 * Wrap all fortran function calls in this.
 */
#ifdef __bg__ // On Blue Gene Architectures there is no underscore
  #define FORTRAN_CALL(name) name
#else  // One underscore everywhere else
  #define FORTRAN_CALL(name) name ## _
#endif

// forward declarations
class Syntax;
class FEProblem;


namespace Moose
{

/**
 * Perflog to be used by applications.
 * If the application prints this in the end they will get performance info.
 */
extern PerfLog perf_log;

/**
 * PerfLog to be used during setup.  This log will get printed just before the first solve. */
extern PerfLog setup_perf_log;

/**
 * Variable indicating whether we will enable FPE trapping for this run.
 */
extern bool _trap_fpe;

/**
 * Variable indicating whether Console coloring will be turned on (default: true).
 */
extern bool _color_console;

/**
 * Variable to toggle any warning into an error (includes deprecated code warnings)
 */
extern bool _warnings_are_errors;

/**
 * Variable to toggle only deprecated warnings as errors.
 */
extern bool _deprecated_is_error;

/**
 * Variable to turn on exceptions during mooseError() and mooseWarning(), should
 * only be used with MOOSE unit.
 */
extern bool _throw_on_error;

/**
 * Macros for coloring any output stream (_console, std::ostringstream, etc.)
 */
#define COLOR_BLACK   (Moose::_color_console ? XTERM_BLACK : "")
#define COLOR_RED     (Moose::_color_console ? XTERM_RED : "")
#define COLOR_GREEN   (Moose::_color_console ? XTERM_GREEN : "")
#define COLOR_YELLOW  (Moose::_color_console ? XTERM_YELLOW : "")
#define COLOR_BLUE    (Moose::_color_console ? XTERM_BLUE : "")
#define COLOR_MAGENTA (Moose::_color_console ? XTERM_MAGENTA : "")
#define COLOR_CYAN    (Moose::_color_console ? XTERM_CYAN : "")
#define COLOR_WHITE   (Moose::_color_console ? XTERM_WHITE : "")
#define COLOR_DEFAULT (Moose::_color_console ? XTERM_DEFAULT : "")

/**
 * Import libMesh::out, and libMesh::err for use in MOOSE.
 */
using libMesh::out;
using libMesh::err;

/**
 * Register objects that are in MOOSE
 */
void registerObjects(Factory & factory);
void addActionTypes(Syntax & syntax);
void registerActions(Syntax & syntax, ActionFactory & action_factory);

void setSolverDefaults(FEProblem & problem);

/**
 * Swap the libMesh MPI communicator out for ours.
 */
MPI_Comm swapLibMeshComm(MPI_Comm new_comm);

void enableFPE(bool on = true);

// MOOSE Requires PETSc to run, this CPP check will cause a compile error if PETSc is not found
#ifndef LIBMESH_HAVE_PETSC
  #error PETSc has not been detected, please ensure your environment is set up properly then rerun the libmesh build script and try to compile MOOSE again.
#endif

} // namespace Moose

#endif /* MOOSE_H */
