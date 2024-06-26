CPPINCLUDES=-I${CPPROOT}/include -I/usr/local/include/gtest 
LIBS= -ldl -lpthread -lgtest -lgtest_main

objects = main.o \
 FixedPoint.o \
 FixedPointTest.o \
 Parser.o \
 ParserTest.o \
 ParserTestExtra.o \

CXXFLAGS= -g ${CPPINCLUDES} -DCPPTESTING --std=c++11  -Wall
TAGS=$(shell ls *.cpp)

test: aal TAGS
	./aal test

aal : $(objects)
	g++  $(objects) -o aal -DDEBUG ${LIBS} ${LIBDIR} -lgtest -lgtest_main -pthread

main.o:

.PHONY : clean
clean :
	-rm aal $(objects)


TAGS:	${TAGS}
	echo "making tags..."
	etags ${TAGS}
