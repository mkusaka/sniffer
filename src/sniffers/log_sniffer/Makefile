CXX = g++

ARTIFUCT = log_sniffer.so
CXXSRC = \
					counter/counter.cc \
					file_watcher/file_watcher.cc \
					log_sniffer.cc \
					../../config/config.cc \

CXXFLAGS = \
					-fPIC \
					-g \
					-shared \
					-std=c++11 \

CPPFLAGS = \
					-I../../config \
					-I../../interfaces/notifier \
					-I../../interfaces/sniffer \

LDFLAGS =

LDLIBS = \
					-lcurl \
					-llog4cpp \

build: $(CXXSRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o $(ARTIFUCT) $(CXXSRC)

clean:
	rm $(ARTIFUCT)
