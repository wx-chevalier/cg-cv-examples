/****************************************************************************
** Copyright (c) 2022, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "../base/signal.h"
#include "../measure/measure_display.h"
#include "../measure/measure_tool.h"

#include <QtWidgets/QWidget>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Mayo {

class GuiDocument;

class WidgetMeasure : public QWidget {
    Q_OBJECT
public:
    WidgetMeasure(GuiDocument* guiDoc, QWidget* parent = nullptr);
    ~WidgetMeasure();

    void setMeasureOn(bool on);

    static void addTool(std::unique_ptr<IMeasureTool> tool);

signals:
    void sizeAdjustmentRequested();

private:
    void onMeasureTypeChanged(int id);
    void onMeasureUnitsChanged();

    static MeasureType toMeasureType(int comboBoxId);
    static LengthUnit toMeasureLengthUnit(int comboBoxId);
    static AngleUnit toMeasureAngleUnit(int comboBoxId);
    static AreaUnit toMeasureAreaUnit(int comboBoxId);

    MeasureType currentMeasureType() const;
    MeasureDisplayConfig currentMeasureDisplayConfig() const;

    void onGraphicsSelectionChanged();

    void updateMessagePanel();

    using IMeasureDisplayPtr = std::unique_ptr<IMeasureDisplay>;
    void eraseMeasureDisplay(const IMeasureDisplay* measure);

    // Provides link between GraphicsOwner and IMeasureDisplay object
    struct GraphicsOwner_MeasureDisplay {
        GraphicsOwnerPtr gfxOwner;
        const IMeasureDisplay* measureDisplay = nullptr;
    };
    void addLink(const GraphicsOwnerPtr& owner, const IMeasureDisplayPtr& measure);
    void eraseLink(const GraphicsOwner_MeasureDisplay* link);
    const GraphicsOwner_MeasureDisplay* findLink(const GraphicsOwnerPtr& owner) const;

    // -- Attributes
    class Ui_WidgetMeasure* m_ui = nullptr;
    GuiDocument* m_guiDoc = nullptr;
    std::vector<GraphicsOwnerPtr> m_vecSelectedOwner;
    std::vector<IMeasureDisplayPtr> m_vecMeasureDisplay;
    std::vector<GraphicsOwner_MeasureDisplay> m_vecLinkGfxOwnerMeasure;
    IMeasureTool* m_tool = nullptr;
    QString m_errorMessage;
    SignalConnectionHandle m_connGraphicsSelectionChanged;
};

} // namespace Mayo
