#pragma once

#include <vector>
#include <string>
#include <charconv>
#include <array>
#include <cmath>
#include <cassert>
#include "../thirdparty/pugixml/pugixml.hpp"

struct CdRecord
{
   //All text fields will be stored as stl strings. Based off of the sample cds given, the majority of the text elements
   //are smaller than the small buffer optimization for std::string (15 chars), and thus won't allocate
   //For now the assumption is that all text will be ascii and won't use multibyte characters
   //To reduce struct size, we could use some form of mapping for fields that aren't particularly unique between cd records, such as Artist, Country, Company
   std::string title;
   std::string artist;
   std::string country;
   std::string company;

   int priceInCents;
   int year;
};

template <typename Number>
std::string to_string(Number number)
{
   constexpr size_t nSizeOfArray = [] {
      if constexpr (std::is_floating_point_v<Number>)
      {
         return 32U;
      }
      else if constexpr (!std::is_floating_point_v<Number>) //integer
      {
         return 16U;
      }
   }();

   constexpr auto lambdaToChars = [](std::array<char, nSizeOfArray> &arrChar, const Number &number) {
      if constexpr (std::is_floating_point_v<Number>)
      {
         return std::to_chars(arrChar.data(), arrChar.data() + arrChar.size(), number, std::chars_format::fixed, 2);
      }
      else if constexpr (!std::is_floating_point_v<Number>) //integer
      {
         return std::to_chars(arrChar.data(), arrChar.data() + arrChar.size(), number);
      }
   };

   std::array<char, nSizeOfArray> arrChar;

   auto [ptr, ec] = lambdaToChars(arrChar, number);

   assert(ec != std::errc::value_too_large);

   *ptr = '\0';

   return std::string(arrChar.data());
}

template <typename Number>
Number to_number(const std::string &strNumber)
{
   Number number{};
   std::from_chars(strNumber.data(), strNumber.data() + strNumber.size(), number);
   return number;
}

inline std::string priceToString(int priceInCents)
{
   return to_string(static_cast<double>(priceInCents) / 100.0);
}

inline int priceToInt(const std::string &priceInDollars)
{
   return static_cast<int>(std::round(to_number<double>(priceInDollars) * 100.0));
}

using Catalog = std::vector<CdRecord>;

namespace ElementTag
{
   constexpr char c_catalog[] = "CATALOG";
   constexpr char c_cd[] = "CD";
   constexpr char c_title[] = "TITLE";
   constexpr char c_artist[] = "ARTIST";   
   constexpr char c_country[] = "COUNTRY";
   constexpr char c_company[] = "COMPANY";
   constexpr char c_price[] = "PRICE";
   constexpr char c_year[] = "YEAR";
} // namespace ElementTag

Catalog LoadCatalogFromXML(const pugi::xml_document &doc);

std::string ConvertCatalogToHTML(const Catalog& catalog);
