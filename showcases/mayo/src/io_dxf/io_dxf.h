/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "../base/io_reader.h"
#include <TopoDS_Shape.hxx>
#include <unordered_map>
#include <string>
#include <vector>

namespace Mayo {
namespace IO {

// Reader for DXF file format based on FreeCad's CDxfRead
class DxfReader : public Reader {
public:
    bool readFile(const FilePath& filepath, TaskProgress* progress) override;
    TDF_LabelSequence transfer(DocumentPtr doc, TaskProgress* progress) override;

    struct Parameters {
        double scaling = 1.;
        bool importAnnotations = true;
        bool groupLayers = true;
        std::string fontNameForTextObjects = "Arial";
    };
    Parameters& parameters() { return m_params; }
    const Parameters& constParameters() const { return m_params; }

    static std::unique_ptr<PropertyGroup> createProperties(PropertyGroup* parentGroup);
    void applyProperties(const PropertyGroup* params) override;

private:
    class Properties;
    class Internal;

    struct Entity {
        int aci = 0;
        TopoDS_Shape shape;
    };
    std::unordered_map<std::string, std::vector<Entity>> m_layers;
    Parameters m_params;
};

class DxfFactoryReader : public FactoryReader {
public:
    Span<const Format> formats() const override;
    std::unique_ptr<Reader> create(Format format) const override;
    std::unique_ptr<PropertyGroup> createProperties(Format format, PropertyGroup* parentGroup) const override;
};

} // namespace IO
} // namespace Mayo
