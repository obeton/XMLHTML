
#pragma once

#include "Identifiers.hpp"

class TextSource;

class TextUser
{
public:
    virtual ~TextUser() = default;

    //Given source, grab relevant data from it and format into own representation
    virtual bool Import(const TextSource& source, RecordId recordId) = 0;
};