/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TDF_Label.hxx>
#include <TDF_LabelSequence.hxx>

namespace Mayo {

// Provides helper functions for OpenCascade CAF related libraries
struct CafUtils {

    // Returns a string representation of tag list path to 'label'
    static const TCollection_AsciiString& labelTag(const TDF_Label& label);

    // Returns the name attribute(if any) attached to 'label'
    // Empty string is returned if no name attribute
    static const TCollection_ExtendedString& labelAttrStdName(const TDF_Label& label);

    // Is 'label' null or empty(ie no attributes)?
    static bool isNullOrEmpty(const TDF_Label& label);

    // Returns attribute of type 'AttributeType'(result may be null)?
    template<typename AttributeType>
    static opencascade::handle<AttributeType> findAttribute(const TDF_Label& label);

    // Is there an attribute of identifier 'attrGuid' attached to 'label'?
    static bool hasAttribute(const TDF_Label& label, const Standard_GUID& attrGuid);

    // Is there an attribute of type 'AttributeType' attached to 'label'?
    template<typename AttributeType> static bool hasAttribute(const TDF_Label& label);

    // Returns a TDF_LabelSequence object built from initializer list
    static TDF_LabelSequence makeLabelSequence(std::initializer_list<TDF_Label> listLabel);

};

} // namespace Mayo


#include <TDF_LabelMapHasher.hxx>
namespace std {

// Specialization of C++11 std::hash<> functor for TDF_Label
template<> struct hash<TDF_Label> {
    inline size_t operator()(const TDF_Label& lbl) const {
        return TDF_LabelMapHasher::HashCode(lbl, INT_MAX);
    }
};

} // namespace std

// --
// -- Implementation
// --

namespace Mayo {

template<typename AttributeType>
opencascade::handle<AttributeType> CafUtils::findAttribute(const TDF_Label& label)
{
    opencascade::handle<AttributeType> attr;
    label.FindAttribute(AttributeType::GetID(), attr);
    return attr;
}

template<typename AttributeType>
bool CafUtils::hasAttribute(const TDF_Label& label)
{
    return hasAttribute(label, AttributeType::GetID());
}

} // namespace Mayo

