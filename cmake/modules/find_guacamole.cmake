##############################################################################
# search paths
##############################################################################
SET(GUACAMOLE_INCLUDE_SEARCH_DIRS
  ${GUACAMOLE_INCLUDE_DIRS}
  ${GUACAMOLE_INCLUDE_SEARCH_DIR}
  ${CMAKE_SOURCE_DIR}/../guacamole/install/include
  /opt/guacamole/current/guacamole/install/include
)

SET(GUACAMOLE_LIBRARY_SEARCH_DIRS
  ${GUACAMOLE_LIBRARY_DIRS}
  ${GUACAMOLE_LIBRARY_SEARCH_DIR}
  ${CMAKE_SOURCE_DIR}/../guacamole/install/lib
  /opt/guacamole/current/guacamole/install/lib
)

##############################################################################
# feedback to provide user-defined paths to search for guacamole
##############################################################################
MACRO (request_guacamole_search_directories)

    IF ( NOT GUACAMOLE_INCLUDE_DIRS AND NOT GUACAMOLE_LIBRARY_DIRS )
        SET(GUACAMOLE_INCLUDE_SEARCH_DIR "Please provide guacamole include path." CACHE PATH "path to guacamole headers.")
        SET(GUACAMOLE_LIBRARY_SEARCH_DIR "Please provide guacamole library path." CACHE PATH "path to guacamole libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole.")
    ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS AND NOT GUACAMOLE_LIBRARY_DIRS )

    IF ( NOT GUACAMOLE_INCLUDE_DIRS )
        SET(GUACAMOLE_INCLUDE_SEARCH_DIR "Please provide guacamole include path." CACHE PATH "path to guacamole headers.")
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole headers.")
    ELSE ( NOT GUACAMOLE_INCLUDE_DIRS )
        UNSET(GUACAMOLE_INCLUDE_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS )

    IF ( NOT GUACAMOLE_LIBRARY_DIRS )
        SET(GUACAMOLE_LIBRARY_SEARCH_DIR "Please provide guacamole library path." CACHE PATH "path to guacamole libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole libraries.")
    ELSE ( NOT GUACAMOLE_LIBRARY_DIRS )
        UNSET(GUACAMOLE_LIBRARY_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_LIBRARY_DIRS )

ENDMACRO (request_guacamole_search_directories)

##############################################################################
# search
##############################################################################
message(STATUS "-- checking for guacamole")

IF ( NOT GUACAMOLE_INCLUDE_DIRS )

    SET(_GUACAMOLE_FOUND_INC_DIRS "")
    SET(_NOT_FOUND true)

    FOREACH(_SEARCH_DIR ${GUACAMOLE_INCLUDE_SEARCH_DIRS})
        IF (${_NOT_FOUND})
            FIND_PATH(_CUR_SEARCH
                    NAMES gua/guacamole.hpp
                    PATHS ${_SEARCH_DIR}
                    NO_DEFAULT_PATH)
            IF (_CUR_SEARCH)
                SET(_GUACAMOLE_FOUND_INC_DIRS ${_CUR_SEARCH})
                SET(_NOT_FOUND false)
            ENDIF(_CUR_SEARCH)
            SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
        ENDIF (${_NOT_FOUND})
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_INCLUDE_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_FOUND_INC_DIRS)
        request_guacamole_search_directories()
    ENDIF (NOT _GUACAMOLE_FOUND_INC_DIRS)

    IF ( _GUACAMOLE_FOUND_INC_DIRS)
        SET(GUACAMOLE_INCLUDE_DIRS ${_GUACAMOLE_FOUND_INC_DIRS} CACHE PATH "guacamole include path.")
    ENDIF ( _GUACAMOLE_FOUND_INC_DIRS)

ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS )


IF ( NOT GUACAMOLE_LIBRARY_DIRS )

    SET(_GUACAMOLE_FOUND_LIB_DIR "")
    SET(_GUACAMOLE_POSTFIX "")
    SET(_NOT_FOUND true)

    FOREACH(_SEARCH_DIR ${GUACAMOLE_LIBRARY_SEARCH_DIRS})
        IF (${_NOT_FOUND})
            IF (UNIX)
                FIND_PATH(_CUR_SEARCH
                        NAMES libguacamole.so
                        PATHS ${_SEARCH_DIR}
                        NO_DEFAULT_PATH)
            ELSEIF(WIN32)
                FIND_PATH(_CUR_SEARCH
                        NAMES guacamole.lib
                        PATHS ${_SEARCH_DIR}
                        PATH_SUFFIXES debug release
                        NO_DEFAULT_PATH)
            ENDIF(UNIX)
            IF (_CUR_SEARCH)
                SET(_GUACAMOLE_FOUND_LIB_DIR ${_SEARCH_DIR})
                SET(_NOT_FOUND false)
            ENDIF(_CUR_SEARCH)
            SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
        ENDIF (${_NOT_FOUND})
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_LIBRARY_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_FOUND_LIB_DIR)
        request_guacamole_search_directories()
    ELSE (NOT _GUACAMOLE_FOUND_LIB_DIR)
		    SET(GUACAMOLE_LIBRARY_DIRS ${_GUACAMOLE_FOUND_LIB_DIR} CACHE PATH "The guacamole library directory")
    ENDIF (NOT _GUACAMOLE_FOUND_LIB_DIR)

    SET(_GUACAMOLE_LIBRARIES "")

    FOREACH(_LIB_DIR ${_GUACAMOLE_FOUND_LIB_DIR})
		    IF (UNIX)
			    file(GLOB_RECURSE _GUACAMOLE_LIBRARIES_ABSOLUTE ${_LIB_DIR}/*.so)
		    ELSEIF(WIN32)
			    file(GLOB_RECURSE _GUACAMOLE_LIBRARIES_ABSOLUTE ${_LIB_DIR}/*.lib)
		    ENDIF(UNIX)
    ENDFOREACH(_LIB_DIR ${_GUACAMOLE_FOUND_LIB_DIR})

    IF (_GUACAMOLE_FOUND_LIB_DIR)
        FOREACH (_GUACAMOLE_LIB_PATH ${_GUACAMOLE_LIBRARIES_ABSOLUTE})
            LIST(APPEND _GUACAMOLE_LIBRARIES ${_GUACAMOLE_LIB_PATH})
        ENDFOREACH(_GUACAMOLE_LIB_PATH)
        SET(GUACAMOLE_LIBRARIES ${_GUACAMOLE_LIBRARIES} CACHE STRING "guacamole libraries")
    ENDIF (_GUACAMOLE_FOUND_LIB_DIR)

ENDIF ( NOT GUACAMOLE_LIBRARY_DIRS )

##############################################################################
# verify
##############################################################################
IF ( NOT GUACAMOLE_INCLUDE_DIRS OR NOT GUACAMOLE_LIBRARY_DIRS )
    request_guacamole_search_directories()
ELSE ( NOT GUACAMOLE_INCLUDE_DIRS OR NOT GUACAMOLE_LIBRARY_DIRS )
    UNSET(GUACAMOLE_INCLUDE_SEARCH_DIR CACHE)
    UNSET(GUACAMOLE_LIBRARY_SEARCH_DIR CACHE)
    MESSAGE(STATUS "--  found matching guacamole version")
    MESSAGE(STATUS "Found guacamole libraries: ${GUACAMOLE_LIBRARIES}")
ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS OR NOT GUACAMOLE_LIBRARY_DIRS )

##############################################################################
# find libraries
##############################################################################
IF (GUACAMOLE_INCLUDE_DIRS AND GUACAMOLE_LIBRARY_DIRS)

  set (GUACAMOLE_PLUGINS_SEARCH_INCLUDE_DIR
     ${GUACAMOLE_INCLUDE_DIRS}
       /opt/guacamole/current/guacamole/install/include
      )

  set (GUACAMOLE_PLUGINS_SEARCH_LIBRARY_DIR 
     ${GUACAMOLE_INCLUDE_DIRS}../lib
     ${GUACAMOLE_LIBRARY_DIRS}
     /opt/guacamole/current/guacamole/install/lib
     )
ENDIF()