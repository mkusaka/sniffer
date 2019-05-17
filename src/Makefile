ARTIFUCT = sniffer

CXX = g++

CXXFLAGS = \
					-g \
					-std=c++11 \

CPPFLAGS = \
					-I/usr/include \
					-I/usr/local/include \
					-I$(CURDIR)/arg_option \
					-I$(CURDIR)/config \
					-I$(CURDIR)/interfaces/notifier \
					-I$(CURDIR)/interfaces/sniffer \

LDFLAGS = \
					-L/usr/local/lib64/boost/ \

LDLIBS = \
				-lboost_program_options \
				-ldl \
				-pthread \
				-llog4cpp \

CXXSRC = \
				./arg_option/arg_option.cc \
				./config/config.cc \
				./main.cc \

build: $(CXXSRC)
	$(CXX) \
	$(CXXFLAGS) \
	$(CPPFLAGS) \
	$(LDFLAGS) $(LDLIBS) \
	-o $(ARTIFUCT) $(CXXSRC)

clean:
	rm $(ARTIFUCT)
