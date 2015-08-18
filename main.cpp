#define CPPTESTING 1

#ifdef CPPTESTING
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <string.h>

int dotests(int argc, char* argv[])
{
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    CppUnit::TextUi::TestRunner runner;
    runner.addTest (  registry.makeTest()  );
    runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(), std::cerr ) );
    bool wasSucessful = runner.run();
    return wasSucessful ? 0 : 1;
}
#endif

int main( int argc, char** argv )
{
#ifdef CPPTESTING
    if ( argc > 1 )
    {
        if (0 == strcmp(argv[1], "test"))
        {
            return dotests(argc, argv);
        }
    }
#endif
    return 0;
}
