#pragma once
#include <memory>
#include "TextUser.hpp"
#include "../thirdparty/CTML/ctml.hpp"

class HtmlExport : public TextUser
{
    std::unique_ptr<CTML::Document> m_doc;

public:
    HtmlExport() = default;
    ~HtmlExport() = default;
    std::string ToString() const;
    virtual bool Import(const TextSource &source, RecordId id) override;
    void ExportToFile(std::string_view fileName) const;
    void Clear() { m_doc.reset();}
};