/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "../base/document.h"
#include "../base/global.h"
#include "../base/signal.h"
#include "../base/tkernel_utils.h"
#include "../graphics/graphics_object_driver.h"
#include "../graphics/graphics_scene.h"
#include "v3d_view_camera_animation.h"

#include <Aspect_TypeOfTriedronPosition.hxx>
#include <Bnd_Box.hxx>
#include <V3d_View.hxx>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Mayo {

class ApplicationItem;
class GuiApplication;
class V3dViewCameraAnimation;

// Provides the link between Base::Document and graphical representations
class GuiDocument {
public:
    GuiDocument(const DocumentPtr& doc, GuiApplication* guiApp);
    ~GuiDocument();

    // Not copyable
    GuiDocument(const GuiDocument&) = delete;
    GuiDocument& operator=(const GuiDocument&) = delete;

    const DocumentPtr& document() const { return m_document; }

    GuiApplication* guiApplication() const { return m_guiApp; }

    const Handle_V3d_View& v3dView() const { return m_v3dView; }
    GraphicsScene* graphicsScene() { return &m_gfxScene; }
    const Bnd_Box& graphicsBoundingBox() const { return m_gfxBoundingBox; }

    // Executes callback 'fn' on all graphics objects associated to tree node 'nodeId'
    // This also includes all children(deep node traversal)
    void foreachGraphicsObject(TreeNodeId nodeId, const std::function<void(GraphicsObjectPtr)>& fn) const;

    // Finds the tree node id associated to graphics object
    TreeNodeId nodeFromGraphicsObject(const GraphicsObjectPtr& gfxObject) const;

    // Toggles selected status of an application item(doesn't affect Application's selection model)
    void toggleItemSelected(const ApplicationItem& appItem);

    // Executes action associated to a 3D sensitive item
    bool processAction(const GraphicsOwnerPtr& gfxOwner);

    // -- Display mode
    int activeDisplayMode(const GraphicsObjectDriverPtr& driver) const;
    void setActiveDisplayMode(const GraphicsObjectDriverPtr& driver, int mode);

    // -- Visible state of document's tree nodes
    CheckState nodeVisibleState(TreeNodeId nodeId) const;
    void setNodeVisible(TreeNodeId nodeId, bool on);

    // -- Exploding
    double explodingFactor() const { return m_explodingFactor; }
    void setExplodingFactor(double t); // Must be in [0,1]

    // -- Visibility of trihedron at world origin
    bool isOriginTrihedronVisible() const;
    void toggleOriginTrihedronVisibility();

    // -- Camera animation
    V3dViewCameraAnimation* viewCameraAnimation() const { return m_cameraAnimation; }
    void setViewCameraOrientation(V3d_TypeOfOrientation projection);
    void runViewCameraAnimation(const V3dViewCameraAnimation::ViewFunction& fnViewChange);
    void stopViewCameraAnimation();

    // -- View trihedron
    enum class ViewTrihedronMode {
        None,
        V3dViewZBuffer,
        AisViewCube // Requires OpenCascade >= v7.4.0
    };
    ViewTrihedronMode viewTrihedronMode() const { return m_viewTrihedronMode; }
    void setViewTrihedronMode(ViewTrihedronMode mode);

    Aspect_TypeOfTriedronPosition viewTrihedronCorner() const { return m_viewTrihedronCorner; }
    void setViewTrihedronCorner(Aspect_TypeOfTriedronPosition corner);

    int aisViewCubeBoundingSize() const;
    static bool isAisViewCubeObject(const GraphicsObjectPtr& gfxObject);

    // -- Background
    struct GradientBackground {
        Quantity_Color color1;
        Quantity_Color color2;
        Aspect_GradientFillMethod fillStyle;
    };
    static const GradientBackground& defaultGradientBackground();
    static void setDefaultGradientBackground(const GradientBackground& gradientBkgnd);

    // Signals
    using MapVisibilityByTreeNodeId = std::unordered_map<TreeNodeId, CheckState>;
    mutable Signal<const MapVisibilityByTreeNodeId&> signalNodesVisibilityChanged;
    mutable Signal<const Bnd_Box&> signalGraphicsBoundingBoxChanged;
    mutable Signal<ViewTrihedronMode> signalViewTrihedronModeChanged;
    mutable Signal<Aspect_TypeOfTriedronPosition> signalViewTrihedronCornerChanged;

    // -- Implementation
private:
    void onDocumentEntityAdded(TreeNodeId entityTreeNodeId);
    void onDocumentEntityAboutToBeDestroyed(TreeNodeId entityTreeNodeId);
    void onGraphicsSelectionChanged();

    void mapEntity(TreeNodeId entityTreeNodeId);
    void unmapEntity(TreeNodeId entityTreeNodeId);

    struct GraphicsEntity {
        struct Object {
            Object(const GraphicsObjectPtr& p) : ptr(p) {}
            GraphicsObjectPtr ptr;
            gp_Trsf trsfOriginal;
            Bnd_Box bndBox;
        };

        TreeNodeId treeNodeId;
        std::vector<Object> vecObject;
        std::unordered_map<TreeNodeId, GraphicsObjectPtr> mapTreeNodeGfxObject;
        std::unordered_map<GraphicsObjectPtr, TreeNodeId> mapGfxObjectTreeNode;
        Bnd_Box bndBox;
    };

    const GraphicsEntity* findGraphicsEntity(TreeNodeId entityTreeNodeId) const;

    void v3dViewTrihedronDisplay(Aspect_TypeOfTriedronPosition corner);

    GuiApplication* m_guiApp = nullptr;
    DocumentPtr m_document;
    GraphicsScene m_gfxScene;
    Handle_V3d_View m_v3dView;
    Handle_AIS_InteractiveObject m_aisOriginTrihedron;

    V3dViewCameraAnimation* m_cameraAnimation = nullptr;
    ViewTrihedronMode m_viewTrihedronMode = ViewTrihedronMode::None;
    Aspect_TypeOfTriedronPosition m_viewTrihedronCorner = Aspect_TOTP_LEFT_UPPER;
    Handle_AIS_InteractiveObject m_aisViewCube;

    std::vector<GraphicsEntity> m_vecGraphicsEntity;
    Bnd_Box m_gfxBoundingBox;

    std::unordered_map<GraphicsObjectDriverPtr, int> m_mapGfxDriverDisplayMode;
    std::unordered_map<TreeNodeId, CheckState> m_mapTreeNodeCheckState;

    double m_explodingFactor = 0.;
};

} // namespace Mayo
