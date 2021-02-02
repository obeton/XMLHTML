#include "BuildTests.h"

#ifndef BUILD_UNIT_TESTS

#include "XMLHTML.hpp"
#include <fstream>
#include <filesystem>

int main(int argc, char *argv[])
{
    assert(argc == 2);
  
    pugi::xml_document doc;

    if (doc.load_file(argv[1]))
    {
        Catalog catalog = LoadCatalogFromXML(doc);
        std::string htmlCatalog = ConvertCatalogToHTML(catalog);
        
        std::filesystem::path path(argv[1]);
        path.replace_extension(".html");

        if (!path.has_parent_path())
        {
            path = std::filesystem::current_path() / path;
        }

        std::ofstream fileStream(path);
        fileStream.write(htmlCatalog.c_str(), htmlCatalog.length());
    }

    return 0;
}
#endif