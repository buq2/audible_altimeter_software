OPTION(USE_MCU "If true, project will be compiled for MCU and not for AMD64 (qt)" OFF)

IF (NOT USE_MCU)
    ADD_SUBDIRECTORY(qt)
ELSE()
    ADD_SUBDIRECTORY(xmega)
ENDIF()
