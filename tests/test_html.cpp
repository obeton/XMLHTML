#include "../src/BuildTests.hpp"

#ifdef BUILD_UNIT_TESTS

#include "../thirdparty/catch2/catch.hpp"
#include "../thirdparty/pugixml/pugixml.hpp"
#include "../src/HtmlExport.hpp"
#include "../src/TextSource.hpp"

constexpr char c_title[] = "Mad Dogs and Englishment";
constexpr char c_artist[] = "Joe Cocker";
constexpr char c_country[] = "USA";
constexpr char c_price[] = "9.80";
constexpr char c_company[] = "A&M";
constexpr char c_year[] = "1970";

class MockImport : public TextSource
{
protected:
    class ElementImpl : public TextSource::ElementImpl
    {
        bool valid = true;

    public:
        ElementImpl() = default;

        virtual std::string Get(FieldId fieldId) const override
        {
            switch (fieldId)
            {
            case FieldId::Title:
                return c_title;
            case FieldId::Artist:
                return c_artist;
            case FieldId::Country:
                return c_country;
            case FieldId::Company:
                return c_company;
            case FieldId::Price:
                return c_price;
            case FieldId::Year:
                return c_year;
            default:
            {
            }
            }

            return "";
        }
        virtual void Increment() override { valid = false;}
        virtual bool IsValid() const override {return valid;}
        virtual std::vector<FieldId> GetFieldIds() const override
        {
            return {
                FieldId::Title,
                FieldId::Artist,
                FieldId::Country,
                FieldId::Company,
                FieldId::Price,
                FieldId::Year,
            };
        }
    };

public:
    virtual TextSource::Element FindFirstElement(RecordId recordId) const override
    {
        std::unique_ptr<TextSource::ElementImpl> impl = std::make_unique<MockImport::ElementImpl>();
        TextSource::Element element(std::move(impl));
        return element;
    }
    virtual std::vector<RecordId> GetRecordIds() const override
    {
        return {RecordId::Cd};
    }
};

TEST_CASE("Catalog can be converted to HTML document string")
{
    MockImport source;

    HtmlExport destination;

    SECTION("using invalid record id fails")
    {
        REQUIRE(false == destination.Import(source, RecordId::Catalog));
    }

    SECTION("single record xml document returns single record catalog")
    {
        REQUIRE(destination.Import(source, RecordId::Cd));

        pugi::xml_document doc;
        doc.load_string(destination.ToString().c_str());

        pugi::xpath_node_set fields = doc.select_nodes("/html/body/table/tr/td");

        REQUIRE(fields.size() == 6);

        size_t index = 0;
        
        for (auto iter = fields.begin(); iter != fields.end(); ++iter)
        {
            pugi::xpath_node node = *iter;
            std::string value = node.node().child_value();

            switch (index)
            {
                case 0:
                REQUIRE(c_title == value);
                break;
                case 1:
                REQUIRE(c_artist == value);
                break;
                case 2:
                REQUIRE(c_country == value);
                break;
                case 3:
                REQUIRE(c_company == value);
                break;
                case 4:
                REQUIRE(c_price == value);
                break;
                case 5:
                REQUIRE(c_year == value);
                break;
            }

            ++index;
        }

        REQUIRE(std::distance(fields.begin(), fields.end())== 6); //only 6 elements in node
    }
}

#endif