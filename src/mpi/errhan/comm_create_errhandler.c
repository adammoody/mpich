/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Comm_create_errhandler */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Comm_create_errhandler = PMPI_Comm_create_errhandler
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Comm_create_errhandler  MPI_Comm_create_errhandler
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Comm_create_errhandler as PMPI_Comm_create_errhandler
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Comm_create_errhandler(MPI_Comm_errhandler_function * comm_errhandler_fn,
                               MPI_Errhandler * errhandler)
    __attribute__ ((weak, alias("PMPI_Comm_create_errhandler")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Comm_create_errhandler
#define MPI_Comm_create_errhandler PMPI_Comm_create_errhandler
#endif

/*@
   MPI_Comm_create_errhandler - Create a communicator error handler

Input Parameters:
. comm_errhandler_fn - user defined error handling procedure (function)

Output Parameters:
. errhandler - MPI error handler (handle)

Error Handler:
   The error handler function should be of the form

   void MPI_Comm_errhandler_function(MPI_Comm *comm, int *rc);

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_OTHER
@*/
int MPI_Comm_create_errhandler(MPI_Comm_errhandler_function * comm_errhandler_fn,
                               MPI_Errhandler * errhandler)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_COMM_CREATE_ERRHANDLER);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_COMM_CREATE_ERRHANDLER);

    /* Validate parameters, especially handles needing to be converted */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ARGNULL(comm_errhandler_fn, "comm_errhandler_fn", mpi_errno);
            MPIR_ERRTEST_ARGNULL(errhandler, "errhandler", mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    mpi_errno = MPIR_Comm_create_errhandler_impl(comm_errhandler_fn, errhandler);
    if (mpi_errno)
        goto fn_fail;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_COMM_CREATE_ERRHANDLER);
    MPID_THREAD_CS_EXIT(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_comm_create_errhandler",
                                 "**mpi_comm_create_errhandler %p %p", comm_errhandler_fn,
                                 errhandler);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}
