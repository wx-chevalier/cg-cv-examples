/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "document.h"

#include "application.h"
#include "caf_utils.h"
#include "cpp_utils.h"
#include <TDF_ChildIterator.hxx>
#include <TDF_TagSource.hxx>
#include <XCAFDoc_DocumentTool.hxx>

namespace Mayo {

Document::Document(const ApplicationPtr& app)
    : TDocStd_Document(NameFormatBinary),
      m_app(app)
{
    TDF_TagSource::Set(this->rootLabel());
}

Document::~Document()
{
}

void Document::initXCaf()
{
    m_xcaf.setLabelMain(this->Main());
    m_xcaf.setModelTree(m_modelTree);
}

const std::string& Document::name() const
{
    return m_name;
}

void Document::setName(std::string_view name)
{
    m_name = name;
    this->signalNameChanged.send(m_name);
}

const FilePath& Document::filePath() const
{
    return m_filePath;
}

void Document::setFilePath(const FilePath& fp)
{
    m_filePath = fp;
    this->signalFilePathChanged.send(fp);
}

const char* Document::toNameFormat(Document::Format format)
{
    switch (format) {
    case Format::Binary: return Document::NameFormatBinary;
    case Format::Xml: return Document::NameFormatXml;
    }

    return nullptr;
}

const char Document::NameFormatBinary[] = "BinDocMayo";
const char Document::NameFormatXml[] = "XmlDocMayo";
const char Document::TypeName[] = "Mayo::Document";

bool Document::isXCafDocument() const
{
    return XCAFDoc_DocumentTool::IsXCAFDocument(this);
}

TDF_Label Document::rootLabel() const
{
    return this->GetData()->Root();
}

bool Document::isEntity(TreeNodeId nodeId)
{
    return m_modelTree.nodeIsRoot(nodeId);
}

int Document::entityCount() const
{
    return CppUtils::safeStaticCast<int>(m_modelTree.roots().size());
}

TDF_Label Document::entityLabel(int index) const
{
    return m_modelTree.nodeData(this->entityTreeNodeId(index));
}

TreeNodeId Document::entityTreeNodeId(int index) const
{
    return m_modelTree.roots()[index];
}

DocumentTreeNode Document::entityTreeNode(int index) const
{
    return { DocumentPtr(this), this->entityTreeNodeId(index) };
}

void Document::rebuildModelTree()
{
    m_modelTree.clear();
    const bool xcafIsNull = m_xcaf.isNull();
    if (!xcafIsNull) {
        for (const TDF_Label& label : m_xcaf.topLevelFreeShapes())
            m_xcaf.deepBuildAssemblyTree(0, label);
    }

    constexpr bool allLevels = true;
    for (TDF_ChildIterator it(this->rootLabel(), !allLevels); it.More(); it.Next()) {
        const TDF_Label childLabel = it.Value();
        if (!CafUtils::isNullOrEmpty(childLabel)
                && (xcafIsNull || childLabel != this->Main())) // Not XCAF Main label
        {
            m_modelTree.appendChild(0, childLabel);
        }
    }
}

DocumentPtr Document::findFrom(const TDF_Label& label)
{
    return DocumentPtr::DownCast(TDocStd_Document::Get(label));
}

TDF_Label Document::newEntityLabel()
{
    Handle_TDF_TagSource tagSrc = CafUtils::findAttribute<TDF_TagSource>(this->rootLabel());
    Expects(!tagSrc.IsNull());
    if (tagSrc->Get() == 0)
        this->rootLabel().NewChild(); // Reserve label 0:1 for XCAF Main()

    return this->rootLabel().NewChild();
}

TDF_Label Document::newEntityShapeLabel()
{
    return m_xcaf.shapeTool()->NewShape();
}

void Document::addEntityTreeNode(const TDF_Label& label)
{
    // Check if 'label' belongs to current document
    if (Document::findFrom(label).get() != this)
        return;

    // Check if 'label' is not already there inside model tree
    for (int i = 0; i < this->entityCount(); ++i) {
        if (this->entityLabel(i) == label)
            return;
    }

    // TODO Allow custom population of the model tree for the new entity
    const TreeNodeId nodeId = m_xcaf.deepBuildAssemblyTree(0, label);
    this->signalEntityAdded.send(nodeId);

#if 0
    // Remove 'label'
    label.ForgetAllAttributes();
    label.Nullify();
#endif
}

void Document::destroyEntity(TreeNodeId entityTreeNodeId)
{
    Expects(this->modelTree().nodeIsRoot(entityTreeNodeId));

    TDF_Label entityLabel = m_modelTree.nodeData(entityTreeNodeId);
    if (CafUtils::isNullOrEmpty(entityLabel))
        return;

    this->signalEntityAboutToBeDestroyed.send(entityTreeNodeId);
    entityLabel.ForgetAllAttributes();
    entityLabel.Nullify();
    m_modelTree.removeRoot(entityTreeNodeId);
}

void Document::BeforeClose()
{
    TDocStd_Document::BeforeClose();
    if (m_app)
        m_app->notifyDocumentAboutToClose(m_identifier);
}

void Document::ChangeStorageFormat(const TCollection_ExtendedString& newStorageFormat)
{
    // TODO: check format
    TDocStd_Document::ChangeStorageFormat(newStorageFormat);
}

} // namespace Mayo
