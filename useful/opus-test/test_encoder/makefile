CXX			:= g++

CXXFLAGS	:= 

LDFLAGS		:= -fPIC

LIBDIR		:= -L../opus/libs

INCDIR		:= -I../opus/include \
			   -I../opus

LIBS		:= $(LIBDIR) -lopus
	   
TARGET		:= opusencoder_test

			   
SRC			:= $(wildcard ../opus/*.cpp *.cpp)
OBJS		:= $(patsubst %.cpp,%.o,$(SRC))

$(TARGET) : $(OBJS)
	$(CXX)  -Wl,-soname,$(TARGET) $(LDFLAGS) -Wl,--no-undefined $(CXXFLAGS) $^ $(LIBS) -o $@
	

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCDIR) $(LDFLAGS) $< -o $@
	
clean:
	rm -rf $(OBJS) $(TARGET)
	

