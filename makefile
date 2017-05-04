LIBS = -fconcepts

enrollment_system : enrsys.cpp
	g++ $(CFLAGS) $(LIBS) -o $@ $<
