#include "../src/BuildTests.hpp"

#ifdef BUILD_UNIT_TESTS
#include "../thirdparty/catch2/catch.hpp"
#include "../src/XmlImport.hpp"
#include "../src/Identifiers.hpp"

TEST_CASE("Catalog can be loaded from XML document")
{
    SECTION("empty record xml document acts invalid")
    {
        pugi::xml_document doc;

        XmlImport source(std::move(doc));

        std::vector<RecordId> recordIds = source.GetRecordIds();
        REQUIRE(recordIds.end() != std::find(recordIds.begin(), recordIds.end(), RecordId::Cd));

        TextSource::Element element = source.FindFirstElement(RecordId::Cd);

        REQUIRE(element.GetFieldIds().size() == 6);

        REQUIRE(!element.IsValid());
    }

    SECTION("single record xml document returns single record catalog")
    {
        constexpr char c_title[] = "Mad Dogs and Englishment";
        constexpr char c_artist[] = "Joe Cocker";        
        constexpr char c_country[] = "USA";
        constexpr char c_price[] = "9.80";
        constexpr char c_company[] = "A&M";
        constexpr char c_year[] = "1970";

        pugi::xml_document doc;

        {
            pugi::xml_node catalogNode = doc.append_child(GetRecordIdTag(RecordId::Catalog).data());

            pugi::xml_node cdNode = catalogNode.append_child(GetRecordIdTag(RecordId::Cd).data());

            pugi::xml_node title = cdNode.append_child(GetFieldIdTag(FieldId::Title).data());
            title.append_child(pugi::node_pcdata).set_value(c_title);

            pugi::xml_node artist = cdNode.append_child(GetFieldIdTag(FieldId::Artist).data());
            artist.append_child(pugi::node_pcdata).set_value(c_artist);

            pugi::xml_node country = cdNode.append_child(GetFieldIdTag(FieldId::Country).data());
            country.append_child(pugi::node_pcdata).set_value(c_country);

            pugi::xml_node company = cdNode.append_child(GetFieldIdTag(FieldId::Company).data());
            company.append_child(pugi::node_pcdata).set_value(c_company);

            pugi::xml_node price = cdNode.append_child(GetFieldIdTag(FieldId::Price).data());
            price.append_child(pugi::node_pcdata).set_value(c_price);

            pugi::xml_node year = cdNode.append_child(GetFieldIdTag(FieldId::Year).data());
            year.append_child(pugi::node_pcdata).set_value(c_year);
        }

        XmlImport source(std::move(doc));

        TextSource::Element element = source.FindFirstElement(RecordId::Cd);

        REQUIRE(element.GetFieldIds().size() == 6);

        REQUIRE(element.IsValid());
        REQUIRE(element.Get(FieldId::Title) == c_title);
        REQUIRE(element.Get(FieldId::Artist) == c_artist);
        REQUIRE(element.Get(FieldId::Country) == c_country);
        REQUIRE(element.Get(FieldId::Company) == c_company);
        REQUIRE(element.Get(FieldId::Price) == c_price);
        REQUIRE(element.Get(FieldId::Year) == c_year);

        REQUIRE(element.Get(FieldId::Unknown) == "");
    }
}

#endif