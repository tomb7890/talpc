CPPINCLUDES=-I${CPPROOT}/include 
LIBS= -ldl -lpthread -lgtest -lgtest_main

objects = main.o \
 FixedPoint.o \
 FixedPointTest.o \
 Parser.o \
 ParserTest.o \
 ParserTestExtra.o \

CXXFLAGS= -g ${CPPINCLUDES} -DCPPTESTING --std=c++11  -Wall  
TAGS=$(shell ls *.cpp)

test: aal 
	./aal test --gtest_break_on_failure

aal : $(objects)
	g++  $(objects)  -o aal -DDEBUG ${LIBS} ${LIBDIR} -lgtest -lgtest_main -pthread 

main.o:

.PHONY : clean
clean :
	-rm aal $(objects)


TAGS:	${TAGS}
	echo "making tags..."
	ctags ${TAGS}


test-docker:
	docker run -it --rm -v $$(pwd):/app -w /app kost13/cpp-gtest:latest make 

hello:
	echo "Hello, world"
