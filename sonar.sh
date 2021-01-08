#!/bin/bash

# -- set the Sonar token (it can be found in the Sonar project)
if [ -z "$SONAR_TOKEN" ] ; then
  echo "The SONAR_TOKEN variable is not set"
  exit -1
fi

# -- the build wrapper and the scanner can be downloaded from the SonarCloud project
#    (directly from the OTest2 project)

# -- firstly do the clean build with the wrapper
cd build
make clean
build-wrapper-linux-x86-64 --out-dir ../bw-output make

# -- then run the sonar analysis (other parameters are in the sonar-project.properties file)
cd ..
sonar-scanner -Dsonar.cfamily.build-wrapper-output=bw-output

