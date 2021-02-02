#pragma once
#include <string_view>

enum class RecordId
{
    Unknown,
    Catalog,
    Cd,
};

constexpr std::string_view GetRecordIdTag(RecordId id)
{
    switch(id)
    {
        case RecordId::Catalog: return "CATALOG";
        case RecordId::Cd: return "CD";
        default:
        {            
        }
    }

    return "";
}

enum class FieldId : uint32_t
{
    Unknown,
    Title,
    Artist,
    Country,
    Company,
    Price,
    Year,
};

constexpr std::string_view GetFieldIdTag(FieldId id)
{
    switch(id)
    {
        case FieldId::Title: return "TITLE";
        case FieldId::Artist: return "ARTIST";
        case FieldId::Country: return "COUNTRY";
        case FieldId::Company: return "COMPANY";
        case FieldId::Price: return "PRICE";
        case FieldId::Year: return "YEAR";
        default:
        {            
        }
    }

    return "";
}

constexpr FieldId GetFieldId(std::string_view tag)
{
 if (tag < GetFieldIdTag(FieldId::Price))
 {
     if (tag < GetFieldIdTag(FieldId::Country))
     {
         if (tag == GetFieldIdTag(FieldId::Company)) return FieldId::Company;

         return FieldId::Artist;
     }
     else if (tag == GetFieldIdTag(FieldId::Country)) return FieldId::Country;
 }
 else if (tag < GetFieldIdTag(FieldId::Year))
 {
     if (tag < GetFieldIdTag(FieldId::Title)) return FieldId::Price;

     return FieldId::Title;
 }
 else return FieldId::Year;
}