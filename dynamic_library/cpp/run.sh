#!/usr/bin/bash

rm -rf myclass.so class_user
g++ -fPIC -shared myclass.cpp -o myclass.so
g++ class_user.cc -ldl -o class_user