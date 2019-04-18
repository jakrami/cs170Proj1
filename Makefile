CXX=g++

CXXFLAGS= -std=c++11

simple_shell: simple_shell.cpp
	g++ -std=c++11 -o simple_shell simple_shell.cpp
