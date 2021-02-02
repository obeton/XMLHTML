#include "BuildTests.hpp"

#ifndef BUILD_UNIT_TESTS

#include "HtmlExport.hpp"
#include "XmlImport.hpp"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[])
{
    assert(argc == 2);

    try
    {
        XmlImport source(argv[1]);

        HtmlExport destination;

        if (destination.Import(source, RecordId::Cd))
        {
            destination.ExportToFile(argv[1]);
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
#endif