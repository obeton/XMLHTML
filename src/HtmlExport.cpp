#include "HtmlExport.hpp"
#include "TextSource.hpp"

#include <filesystem>
#include <fstream>

bool HtmlExport::Import(const TextSource &source, RecordId recordId)
{
    std::vector<RecordId> recordIds = source.GetRecordIds();

    auto iter = std::find(recordIds.begin(), recordIds.end(), recordId);

    bool success = iter != recordIds.end();

    if (success)
    {
        TextSource::Element element = source.FindFirstElement(recordId);

        if (success &= element.IsValid(); success)
        {
            if (auto newDoc = std::make_unique<CTML::Document>(); newDoc)
            {
                newDoc->AppendNodeToBody(CTML::Node("table"));
                CTML::Node &table = newDoc->body().GetChildByName("table");

                std::vector<FieldId> fieldIds = element.GetFieldIds();

                //Always generate table headers
                {
                    CTML::Node rowHeader("tr");

                    for (const FieldId &id : fieldIds)
                    {
                        rowHeader.AppendChild(CTML::Node("th", GetFieldIdTag(id).data()));
                    }

                    table.AppendChild(rowHeader);
                }

                while (element.IsValid())
                {
                    CTML::Node row("tr");
                    for (const FieldId &id : fieldIds)
                    {
                        row.AppendChild(CTML::Node("td", element.Get(id)));
                    }

                    table.AppendChild(row);

                    ++element;
                }

                if (success) //overwrite current doc
                {
                    Clear();
                    m_doc = std::move(newDoc);
                }
            }
        }
    }
    
    return success;
}

std::string HtmlExport::ToString() const
{
    return m_doc ? m_doc->ToString() : "";
}

void HtmlExport::ExportToFile(std::string_view fileName) const
{
    std::string htmlCatalog = ToString();

    std::filesystem::path path(fileName);
    path.replace_extension(".html");

    if (!path.has_parent_path())
    {
        path = std::filesystem::current_path() / path;
    }

    std::ofstream fileStream(path);
    fileStream.write(htmlCatalog.c_str(), htmlCatalog.length());
}