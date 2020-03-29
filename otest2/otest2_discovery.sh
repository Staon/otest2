#!/bin/bash

echo | ( c++ -Wp,-v -x c++ -fsyntax-only - 2>&1 ) | grep '^[ ]' | cut -d\  -f2
