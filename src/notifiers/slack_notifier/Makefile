CXX = g++

ARTIFUCT = slack_notifier.so
CXXSRC = \
					slack_client/slack_client.cc \
					slack_notifier.cc \
					../../config/config.cc \

CXXFLAGS = \
					-fPIC \
					-g \
					-shared \
					-std=c++11 \

CPPFLAGS = \
					-I$(CURDIR)/../../config \
					-I$(CURDIR)/../../interfaces/notifier \

LDFLAGS =

LDLIBS = \
					-lcurl \
					-llog4cpp \

build: $(CXXSRC)
	$(CXX) \
	$(CXXFLAGS) \
	$(CPPFLAGS) \
	$(LDFLAGS) $(LDLIBS) \
	-o $(ARTIFUCT) $(CXXSRC)

clean:
	rm $(ARTIFUCT)
