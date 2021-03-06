#---------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#---------------------------------------------------------------------#
#
# Test suite for pipelines, workflows and coupled runs 
# using adios_iotest
#

if(ADIOS2_HAVE_MPI)

set(mpicmd ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG})

#------------------------------------------
#  Test suite for Utils.IOTest.Pipe2
#------------------------------------------

#
#  Pipe2 InsituMPI version 
#
  set(WORKDIR ${CMAKE_CURRENT_BINARY_DIR}/Pipe2.BP)
  file(MAKE_DIRECTORY ${WORKDIR})

#  BP file version Write part
add_test(NAME Utils.IOTest.Pipe2.BP.Write
  COMMAND ${mpicmd} 2 ${PROJECT_BINARY_DIR}/bin/adios_iotest -a 1 -c ${CMAKE_CURRENT_SOURCE_DIR}/pipe2.txt -x ${CMAKE_CURRENT_SOURCE_DIR}/pipe2-BP.xml -d 2 1 --strong-scaling
  WORKING_DIRECTORY ${WORKDIR} 
)

add_test(NAME Utils.IOTest.Pipe2.BP.DumpWrite
  COMMAND ${CMAKE_COMMAND}
    -DARGS=-laD
    -DINPUT_FILE=pipe2_write.bp
    -DOUTPUT_FILE=IOTest.Pipe2.BP.Write.bpls.txt
    -P "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bpls.cmake"
  WORKING_DIRECTORY ${WORKDIR} 
)

set_property(TEST Utils.IOTest.Pipe2.BP.DumpWrite
  PROPERTY DEPENDS Utils.IOTest.Pipe2.BP.Write
)

add_test(NAME Utils.IOTest.Pipe2.BP.ValidateWrite
  COMMAND ${CMAKE_COMMAND}
    -E compare_files
    ${CMAKE_CURRENT_SOURCE_DIR}/IOTest.Pipe2.BP.Write.bpls.txt
    ${WORKDIR}/IOTest.Pipe2.BP.Write.bpls.txt
  WORKING_DIRECTORY ${WORKDIR} 
)
set_property(TEST Utils.IOTest.Pipe2.BP.ValidateWrite
  PROPERTY DEPENDS Utils.IOTest.Pipe2.BP.DumpWrite
)

#  BP file version Read part

add_test(NAME Utils.IOTest.Pipe2.BP.Read
  COMMAND ${mpicmd} 2 ${PROJECT_BINARY_DIR}/bin/adios_iotest -a 2 -c ${CMAKE_CURRENT_SOURCE_DIR}/pipe2.txt -x ${CMAKE_CURRENT_SOURCE_DIR}/pipe2-BP.xml -d 2 1 --strong-scaling
  WORKING_DIRECTORY ${WORKDIR} 
)

add_test(NAME Utils.IOTest.Pipe2.BP.DumpRead
  COMMAND ${CMAKE_COMMAND}
    -DARGS=-laD
    -DINPUT_FILE=pipe2_read.bp
    -DOUTPUT_FILE=IOTest.Pipe2.BP.Read.bpls.txt
    -P "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bpls.cmake"
  WORKING_DIRECTORY ${WORKDIR} 
)

set_property(TEST Utils.IOTest.Pipe2.BP.DumpRead
  PROPERTY DEPENDS Utils.IOTest.Pipe2.BP.Read
)

add_test(NAME Utils.IOTest.Pipe2.BP.ValidateRead
  COMMAND ${CMAKE_COMMAND}
    -E compare_files
    ${CMAKE_CURRENT_SOURCE_DIR}/IOTest.Pipe2.BP.Read.bpls.txt
    ${WORKDIR}/IOTest.Pipe2.BP.Read.bpls.txt
  WORKING_DIRECTORY ${WORKDIR} 
)
set_property(TEST Utils.IOTest.Pipe2.BP.ValidateRead
  PROPERTY DEPENDS Utils.IOTest.Pipe2.BP.DumpRead
)


#
math(EXPR HAVE_2_PROCS "${MPIEXEC_MAX_NUMPROCS} - 1")
if (${HAVE_2_PROCS}) 

  #
  #  Pipe2 InsituMPI version 
  #
  set(WORKDIR ${CMAKE_CURRENT_BINARY_DIR}/Pipe2.InSituMPI)
  file(MAKE_DIRECTORY ${WORKDIR})

  add_test(NAME Utils.IOTest.Pipe2.InSituMPI
    COMMAND ${mpicmd} 1 ${PROJECT_BINARY_DIR}/bin/adios_iotest -a 1 -c ${CMAKE_CURRENT_SOURCE_DIR}/pipe2.txt -x ${CMAKE_CURRENT_SOURCE_DIR}/pipe2-InSituMPI.xml -d 1 1 --strong-scaling :  ${MPIEXEC_NUMPROC_FLAG} 1 ${PROJECT_BINARY_DIR}/bin/adios_iotest -a 2 -c ${CMAKE_CURRENT_SOURCE_DIR}/pipe2.txt -x ${CMAKE_CURRENT_SOURCE_DIR}/pipe2-InSituMPI.xml -d 1 1 --strong-scaling 
    WORKING_DIRECTORY ${WORKDIR} 
  )

  add_test(NAME Utils.IOTest.Pipe2.DumpInSituMPI
    COMMAND ${CMAKE_COMMAND}
      -DARGS=-laD
      -DINPUT_FILE=pipe2_read.bp
      -DOUTPUT_FILE=IOTest.Pipe2.InSituMPI.bpls.txt
      -P "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bpls.cmake"
    WORKING_DIRECTORY ${WORKDIR} 
  )

  set_property(TEST Utils.IOTest.Pipe2.DumpInSituMPI
    PROPERTY DEPENDS Utils.IOTest.Pipe2.InSituMPI
  )

  add_test(NAME Utils.IOTest.Pipe2.ValidateInSituMPI
    COMMAND ${CMAKE_COMMAND}
      -E compare_files
      ${CMAKE_CURRENT_SOURCE_DIR}/IOTest.Pipe2.InSituMPI.bpls.txt
      ${WORKDIR}/IOTest.Pipe2.InSituMPI.bpls.txt
    WORKING_DIRECTORY ${WORKDIR} 
  )
  set_property(TEST Utils.IOTest.Pipe2.ValidateInSituMPI
    PROPERTY DEPENDS Utils.IOTest.Pipe2.DumpInSituMPI
  )


  #
  #  Pipe2 SST version 
  #
  if(ADIOS2_HAVE_SST)
	set(WORKDIR ${CMAKE_CURRENT_BINARY_DIR}/Pipe2.SST)
	file(MAKE_DIRECTORY ${WORKDIR})
	
	add_test(NAME Utils.IOTest.Pipe2.SST
	  COMMAND ${mpicmd} 1 ${PROJECT_BINARY_DIR}/bin/adios_iotest -a 1 -c ${CMAKE_CURRENT_SOURCE_DIR}/pipe2.txt -x ${CMAKE_CURRENT_SOURCE_DIR}/pipe2-SST.xml -d 1 1 --strong-scaling :  ${MPIEXEC_NUMPROC_FLAG} 1 ${PROJECT_BINARY_DIR}/bin/adios_iotest -a 2 -c ${CMAKE_CURRENT_SOURCE_DIR}/pipe2.txt -x ${CMAKE_CURRENT_SOURCE_DIR}/pipe2-SST.xml -d 1 1 --strong-scaling 
	  WORKING_DIRECTORY ${WORKDIR} 
	)
	
	add_test(NAME Utils.IOTest.Pipe2.DumpSST
	  COMMAND ${CMAKE_COMMAND}
	    -DARGS=-laD
	    -DINPUT_FILE=pipe2_read.bp
	    -DOUTPUT_FILE=IOTest.Pipe2.SST.bpls.txt
	    -P "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/bpls.cmake"
	  WORKING_DIRECTORY ${WORKDIR} 
	)
	
	set_property(TEST Utils.IOTest.Pipe2.DumpSST
	  PROPERTY DEPENDS Utils.IOTest.Pipe2.SST
	)
	
	add_test(NAME Utils.IOTest.Pipe2.ValidateSST
	  COMMAND ${CMAKE_COMMAND}
	    -E compare_files
	    ${CMAKE_CURRENT_SOURCE_DIR}/IOTest.Pipe2.SST.bpls.txt
	    ${WORKDIR}/IOTest.Pipe2.SST.bpls.txt
	  WORKING_DIRECTORY ${WORKDIR} 
	)
	set_property(TEST Utils.IOTest.Pipe2.ValidateSST
	  PROPERTY DEPENDS Utils.IOTest.Pipe2.DumpSST
	)
  endif(ADIOS2_HAVE_SST)

else (${HAVE_2_PROCS}) 
message(WARNING "We don't have enough (2) processes to run insitu test fwith adios_iotest")
endif (${HAVE_2_PROCS}) 


#------------------------------------------
#  Test suite for Utils.IOTest.Coupling2
#------------------------------------------

endif(ADIOS2_HAVE_MPI)

