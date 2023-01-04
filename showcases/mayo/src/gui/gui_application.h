/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "../base/application_ptr.h"
#include "../base/application_item_selection_model.h"
#include "../base/span.h"
#include "../graphics/graphics_object_driver.h"
#include "gui_document.h"

#include <memory>

namespace Mayo {

class GuiDocument;

class GuiApplication {
public:
    GuiApplication(const ApplicationPtr& app);
    ~GuiApplication();

    // Not copyable
    GuiApplication(const GuiApplication&) = delete;
    GuiApplication& operator=(const GuiApplication&) = delete;

    const ApplicationPtr& application() const;

    Span<GuiDocument*> guiDocuments();
    Span<GuiDocument* const> guiDocuments() const;
    GuiDocument* findGuiDocument(const DocumentPtr& doc) const;

    ApplicationItemSelectionModel* selectionModel() const;

    void addGraphicsObjectDriver(GraphicsObjectDriverPtr ptr);
    void addGraphicsObjectDriver(std::unique_ptr<GraphicsObjectDriver> ptr);
    Span<const GraphicsObjectDriverPtr> graphicsObjectDrivers() const;
    GraphicsObjectPtr createGraphicsObject(const TDF_Label& label) const;

    // Whether a GuiDocument object is automatically created once a Document is added in Application
    bool automaticDocumentMapping() const;
    void setAutomaticDocumentMapping(bool on);

    // Signals
    mutable Signal<GuiDocument*> signalGuiDocumentAdded;
    mutable Signal<GuiDocument*> signalGuiDocumentErased;

protected:
    void onDocumentAdded(const DocumentPtr& doc);
    void onDocumentAboutToClose(const DocumentPtr& doc);

private:
    friend class GuiDocument;
    void connectApplicationItemSelectionChanged(bool on);

    struct Private;
    Private* const d = nullptr;
};

} // namespace Mayo
