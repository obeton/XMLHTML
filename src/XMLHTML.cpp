#include "XMLHTML.hpp"
#include "../thirdparty/CTML/ctml.hpp"

Catalog LoadCatalogFromXML(const pugi::xml_document& doc)
{
    //Use XPath to get all nodes named "CD" that are children of "CATALOG"
    pugi::xpath_node_set cds = doc.select_nodes("/CATALOG/CD");

    Catalog catalog;

    for (auto iter = cds.begin(); iter != cds.end(); ++iter)
    {
        //Always add, we will validate later
        CdRecord& cd = catalog.emplace_back();

        pugi::xpath_node cdNode = *iter;
        cd.title = cdNode.node().child_value(ElementTag::c_title);
        cd.artist = cdNode.node().child_value(ElementTag::c_artist);
        cd.country = cdNode.node().child_value(ElementTag::c_country);
        cd.priceInCents = priceToInt(cdNode.node().child_value(ElementTag::c_price));
        cd.year = to_number<int>(cdNode.node().child_value(ElementTag::c_year));
    }
    
    return catalog;
}

std::string ConvertCatalogToHTML(const Catalog& catalog)
{
    CTML::Document doc;
    doc.AppendNodeToBody(CTML::Node("table"));
    CTML::Node &table = doc.body().GetChildByName("table");
    table.SetAttribute("id", "CD catalog");

    //Always generate table headers
    {
        CTML::Node rowHeader("tr");
        rowHeader.AppendChild(CTML::Node("th", ElementTag::c_title));
        rowHeader.AppendChild(CTML::Node("th", ElementTag::c_artist));
        rowHeader.AppendChild(CTML::Node("th", ElementTag::c_country));
        rowHeader.AppendChild(CTML::Node("th", ElementTag::c_company));
        rowHeader.AppendChild(CTML::Node("th", ElementTag::c_price));
        rowHeader.AppendChild(CTML::Node("th", ElementTag::c_year));

        table.AppendChild(rowHeader);
    }

    for (const CdRecord &cd : catalog)
    {
        CTML::Node row("tr");
        row.AppendChild(CTML::Node("td", cd.title));
        row.AppendChild(CTML::Node("td", cd.artist));
        row.AppendChild(CTML::Node("td", cd.country));
        row.AppendChild(CTML::Node("td", cd.company));
        row.AppendChild(CTML::Node("td", priceToString(cd.priceInCents)));
        row.AppendChild(CTML::Node("td", to_string(cd.year)));

        table.AppendChild(row);
    }

    return doc.ToString();
}
