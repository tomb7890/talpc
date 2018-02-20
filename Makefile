LIBS=-lcppunit -ldl
CPPINCLUDES=-I${CPPROOT}/include
LIBDIR=${CPPINCLUDES} -L${CPPROOT}/src/cppunit/.libs
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
	g++  $(objects) -g -o aal -DDEBUG ${LIBS} ${LIBDIR}

main.o:

.PHONY : clean
clean :
	rm aal $(objects)


TAGS:	${TAGS}
	echo "making tags..."
	etags ${TAGS}
