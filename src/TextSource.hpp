#include <string>
#include <memory>
#include <vector>
#include "TextUser.hpp"
#include "Identifiers.hpp"

class TextSource
{
protected:
    class ElementImpl
    {
    public:
        virtual std::string Get(FieldId fieldId) const = 0;
        virtual void Increment() = 0;
        virtual bool IsValid() const = 0;
        virtual std::vector<FieldId> GetFieldIds() const = 0;
    };

public:
    //Iterator-like class that is incremented to visit/access all data
    //associated with a recordId
    class Element
    {
        std::unique_ptr<ElementImpl> m_elementImpl;

    public:
        Element(std::unique_ptr<ElementImpl> &&impl)
            : m_elementImpl(std::move(impl))
        {
        }
        
        Element(Element &&other) noexcept
        {
            m_elementImpl = std::move(other.m_elementImpl);
        }

        Element(const Element& other) = delete;

        void operator++()
        {
            if (IsValid())
            {
                m_elementImpl->Increment();
            }
        }

        //Get string associated with current record and given field
        std::string Get(FieldId fieldId) const
        {
            if (IsValid())
            {
                return m_elementImpl->Get(fieldId);
            }

            return "";
        }

        //Get all fieldids that element can access
        std::vector<FieldId> GetFieldIds() const
        {
            return m_elementImpl->GetFieldIds();
        }

        bool IsValid() const
        {
            return m_elementImpl && m_elementImpl->IsValid();
        }
    };

    //Get an elemented that is set to the first bit of data associated with recordId
    virtual Element FindFirstElement(RecordId recordId) const = 0;

    //Get all recordids that is supported by this TextSource
    virtual std::vector<RecordId> GetRecordIds() const = 0;

    virtual ~TextSource() = default;
};
