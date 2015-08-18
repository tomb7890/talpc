# note: this make file just builds the objects; to make
# use of your installed CppUnit, use Makefile.test and
# set the CPPROOT path.


objects = Parser.o FixedPoint.o main.o
CXXFLAGS= -g ${CPPINCLUDES}
TAGS=$(shell ls *.cpp)
LIBS=cppunit

aal : $(objects)
	g++  $(objects) -g -o aal -DDEBUG -l ${LIBS} ${LIBDIR}
main.o:

.PHONY : clean
clean :
	rm aal $(objects)

TAGS:	${TAGS}
	echo "making tags..."
	etags ${TAGS}
