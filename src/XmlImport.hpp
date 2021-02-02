#include "TextSource.hpp"
#include "../thirdparty/pugixml/pugixml.hpp"

class XmlImport : public TextSource
{
    pugi::xml_document m_doc;

protected:
    class ElementImpl : public TextSource::ElementImpl
    {
        pugi::xpath_node_set m_nodeSet;
        pugi::xpath_node_set::const_iterator m_iter;

    public:
        ElementImpl(pugi::xpath_node_set&& nodeSet);

        virtual std::string Get(FieldId fieldId) const override;
        virtual void Increment() override;
        virtual bool IsValid() const override;
        virtual std::vector<FieldId> GetFieldIds() const override;
    };

public:
    XmlImport(std::string_view filename);
    XmlImport(pugi::xml_document &&doc);
    virtual TextSource::Element FindFirstElement(RecordId recordId) const override;
    virtual std::vector<RecordId> GetRecordIds() const override;
};