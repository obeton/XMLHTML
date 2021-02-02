#include "XmlImport.hpp"
#include <stdexcept>

XmlImport::ElementImpl::ElementImpl(pugi::xpath_node_set&& nodeSet) 
: m_nodeSet(nodeSet)
{
    m_iter = m_nodeSet.begin();
}

std::string XmlImport::ElementImpl::Get(FieldId fieldId) const
{
    pugi::xpath_node node = *m_iter;
    return node.node().child_value(GetFieldIdTag(fieldId).data());
}

void XmlImport::ElementImpl::Increment()
{
    if (IsValid())
        ++m_iter;
}

bool XmlImport::ElementImpl::IsValid() const
{
    return m_iter != m_nodeSet.end();
}

std::vector<FieldId> XmlImport::ElementImpl::GetFieldIds() const
{
    return 
    {
        FieldId::Title,
        FieldId::Artist,
        FieldId::Country,
        FieldId::Company,
        FieldId::Price,
        FieldId::Year,
    };
}

XmlImport::XmlImport(std::string_view filename)
{
    if (!m_doc.load_file(filename.data()))
    {
        throw std::runtime_error("Unable to load XML document");
    }
}
XmlImport::XmlImport(pugi::xml_document&& doc)
{
    m_doc = std::move(doc);
}

TextSource::Element XmlImport::FindFirstElement(RecordId recordId) const
{
    std::string_view select = [](RecordId recordId)
    {
        switch(recordId)
        {
            case RecordId::Catalog:
            return "/CATALOG";
            case RecordId::Cd:
            return "/CATALOG/CD";
        }

        return "";
    }(recordId);

    //Use XPath to find all relevant nodes
    pugi::xpath_node_set record = m_doc.select_nodes(select.data());

    std::unique_ptr<TextSource::ElementImpl> impl = std::make_unique<XmlImport::ElementImpl>(std::move(record));

    TextSource::Element element(std::move(impl));

    return element;
}

std::vector<RecordId> XmlImport::GetRecordIds() const
{
    return {RecordId::Catalog, RecordId::Cd};
}