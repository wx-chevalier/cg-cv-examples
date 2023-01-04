/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "io_occ_brep.h"

#include "../base/application_item.h"
#include "../base/caf_utils.h"
#include "../base/document.h"
#include "../base/filepath_conv.h"
#include "../base/occ_progress_indicator.h"
#include "../base/task_progress.h"
#include "../base/tkernel_utils.h"

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TDataStd_Name.hxx>

namespace Mayo {
namespace IO {

bool OccBRepReader::readFile(const FilePath& filepath, TaskProgress* progress)
{
    m_shape.Nullify();
    m_baseFilename = filepath.stem();
    BRep_Builder brepBuilder;
    Handle_Message_ProgressIndicator indicator = new OccProgressIndicator(progress);
    return BRepTools::Read(
                m_shape,
                filepath.u8string().c_str(),
                brepBuilder,
                TKernelUtils::start(indicator));
}

TDF_LabelSequence OccBRepReader::transfer(DocumentPtr doc, TaskProgress* /*progress*/)
{
    if (m_shape.IsNull())
        return {};

    const Handle_XCAFDoc_ShapeTool shapeTool = doc->xcaf().shapeTool();
    const TDF_Label labelShape = shapeTool->NewShape();
    shapeTool->SetShape(labelShape, m_shape);
    TDataStd_Name::Set(labelShape, filepathTo<TCollection_ExtendedString>(m_baseFilename));
    return CafUtils::makeLabelSequence({ labelShape });
}

bool OccBRepWriter::transfer(Span<const ApplicationItem> appItems, TaskProgress* /*progress*/)
{
    m_shape = TopoDS_Shape();

    std::vector<TopoDS_Shape> vecShape;
    vecShape.reserve(appItems.size());
    for (const ApplicationItem& item : appItems) {
        if (item.isDocument()) {
            for (const TDF_Label& label : item.document()->xcaf().topLevelFreeShapes())
                vecShape.push_back(XCaf::shape(label));
        }
        else if (item.isDocumentTreeNode()) {
            const TDF_Label labelNode = item.documentTreeNode().label();
            vecShape.push_back(XCaf::shape(labelNode));
        }
    }

    if (vecShape.size() > 1) {
        TopoDS_Compound cmpd;
        BRep_Builder builder;
        builder.MakeCompound(cmpd);
        for (const TopoDS_Shape& subShape : vecShape)
            builder.Add(cmpd, subShape);

        m_shape = cmpd;
    }
    else if (vecShape.size() == 1) {
        m_shape = vecShape.front();
    }

    return true;
}

bool OccBRepWriter::writeFile(const FilePath& filepath, TaskProgress* progress)
{
    Handle_Message_ProgressIndicator indicator = new OccProgressIndicator(progress);
    return BRepTools::Write(m_shape, filepath.u8string().c_str(), TKernelUtils::start(indicator));
}

} // namespace IO
} // namespace Mayo
