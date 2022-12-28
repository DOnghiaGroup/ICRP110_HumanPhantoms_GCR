# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata"
  "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix/src/ICRPdata-build"
  "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix"
  "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix/tmp"
  "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix/src/ICRPdata-stamp"
  "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix/src"
  "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix/src/ICRPdata-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix/src/ICRPdata-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ben/Desktop/ICRP110_HumanPhantoms_GCR/build/ICRPdata-prefix/src/ICRPdata-stamp${cfgdir}") # cfgdir has leading slash
endif()
