#include "../src/BuildTests.h"

#ifdef BUILD_UNIT_TESTS

#include "../thirdparty/catch2/catch.hpp"
#include "../src/XMLHTML.hpp"

TEST_CASE("Properly convert prices between dollar decimal string and integer cents")
{
    REQUIRE(priceToString(42) == std::string("0.42"));
    REQUIRE(priceToInt(std::string("0.42")) == 42);

    REQUIRE(priceToString(980) == std::string("9.80"));
    REQUIRE(priceToInt(std::string("9.80")) == 980);
}

TEST_CASE("Catalog can be loaded from XML document")
{
    SECTION("empty xml document returns empty catalog")
    {
        pugi::xml_document doc;
        REQUIRE(LoadCatalogFromXML(doc).empty());
    }

    SECTION("single record xml document returns single record catalog")
    {
        constexpr char c_title[] = "Mad Dogs and Englishment";
        constexpr char c_artist[] = "Joe Cocker";        
        constexpr char c_country[] = "USA";
        constexpr int c_price = 980;
        constexpr char c_company[] = "A&M";
        constexpr char c_year = 1970;

        pugi::xml_document doc;

        {
            pugi::xml_node catalogNode = doc.append_child(ElementTag::c_catalog);

            pugi::xml_node cdNode = catalogNode.append_child(ElementTag::c_cd);

            pugi::xml_node title = cdNode.append_child(ElementTag::c_title);
            title.append_child(pugi::node_pcdata).set_value(c_title);

            pugi::xml_node artist = cdNode.append_child(ElementTag::c_artist);
            artist.append_child(pugi::node_pcdata).set_value(c_artist);

            pugi::xml_node country = cdNode.append_child(ElementTag::c_country);
            country.append_child(pugi::node_pcdata).set_value(c_country);

            pugi::xml_node company = cdNode.append_child(ElementTag::c_company);
            company.append_child(pugi::node_pcdata).set_value(c_company);

            pugi::xml_node price = cdNode.append_child(ElementTag::c_price);
            price.append_child(pugi::node_pcdata).set_value(priceToString(c_price).c_str());

            pugi::xml_node year = cdNode.append_child(ElementTag::c_year);
            year.append_child(pugi::node_pcdata).set_value(to_string(c_year).c_str());
        }

        Catalog catalog  = LoadCatalogFromXML(doc);

        REQUIRE(catalog.size() == 1);
        const CdRecord& cd = catalog.front();

        REQUIRE(cd.title == c_title);
        REQUIRE(cd.artist == c_artist);
        REQUIRE(cd.country == c_country);
        REQUIRE(cd.priceInCents == c_price);
        REQUIRE(cd.year == c_year);
    }
}

#endif