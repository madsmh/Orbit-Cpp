sources  = main.cpp
sources += propertiesfile.cpp
sources += trajectory.cpp
sources += planetdata.cpp
sources += system.cpp

planets: $(sources)
	g++ -g -Wall $(sources) -o planets
