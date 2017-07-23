sources  = main.cpp
sources += propertiesfile.cpp
sources += trajectory.cpp
sources += planetdata.cpp
sources += system.cpp
headers  = body.h
headers += planetdata.h
headers += propertiesfile.h
headers += system.h
headers += trajectory.h
headers += vector3.h

planets: $(sources) $(headers)
	g++ -g -Wall $(sources) -o planets
