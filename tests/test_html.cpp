#include "../src/BuildTests.h"

#ifdef BUILD_UNIT_TESTS

#include "../thirdparty/catch2/catch.hpp"
#include "../src/XMLHTML.hpp"
#include "../thirdparty/CTML/ctml.hpp"

TEST_CASE("Catalog can be converted to HTML document string")
{
    SECTION("single record xml document returns single record catalog")
    {
        Catalog catalog(1U, CdRecord{"Mad Dogs and Englishmen", "Joe Cocker", "USA", "A&M", 980, 1970});
        std::string html = ConvertCatalogToHTML(catalog);

        pugi::xml_document doc;
        doc.load_string(html.c_str());

        pugi::xpath_node_set fields = doc.select_nodes("/html/body/table/tr/td");

        REQUIRE(fields.size() == 6);

        size_t index = 0;
        const CdRecord& cd = catalog.back();
        for (auto iter = fields.begin(); iter != fields.end(); ++iter)
        {
            pugi::xpath_node node = *iter;
            std::string value = node.node().child_value();

            switch (index)
            {
                case 0:
                REQUIRE(cd.title == value);
                break;
                case 1:
                REQUIRE(cd.artist == value);
                break;
                case 2:
                REQUIRE(cd.country == value);
                break;
                case 3:
                REQUIRE(cd.company == value);
                break;
                case 4:
                REQUIRE(cd.priceInCents == priceToInt(value));
                break;
                case 5:
                REQUIRE(cd.year == to_number<int>(value));
                break;
            }


            ++index;
        }
    }
}

#endif