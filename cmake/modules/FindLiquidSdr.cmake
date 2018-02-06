#
# Copyright 2012-2013 The Iris Project Developers. See the
# COPYRIGHT file at the top-level directory of this distribution
# and at http://www.softwareradiosystems.com/iris/copyright.html.
#
# This file is part of the Iris Project.
#
# Iris is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# Iris is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# A copy of the GNU Lesser General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#

# - Try to find liquiddsp - https://github.com/jgaeddert/liquid-dsp
# Once done this will define
#  LIQUIDDSP_FOUND - System has liquiddsp
#  LIQUIDDSP_INCLUDE_DIRS - The liquiddsp include directories
#  LIQUIDDSP_LIBRARIES - The libraries needed to use liquiddsp

find_path(LIQUIDDSP_INCLUDE_DIR 
            NAMES liquid/liquid.h
            HINTS $ENV{LIQUIDDSP_DIR}/include
            PATHS /usr/local/include
                  /usr/include )

find_library(LIQUIDDSP_LIBRARY 
            NAMES liquid
            HINTS $ENV{LIQUIDDSP_DIR}/lib
            PATHS /usr/local/lib
                  /usr/lib)

set(LIQUIDDSP_LIBRARIES ${LIQUIDDSP_LIBRARY} )
set(LIQUIDDSP_INCLUDE_DIRS ${LIQUIDDSP_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIQUIDDSP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(liquiddsp  DEFAULT_MSG
                                  LIQUIDDSP_LIBRARY LIQUIDDSP_INCLUDE_DIR)

mark_as_advanced(LIQUIDDSP_INCLUDE_DIR LIQUIDDSP_LIBRARY )
