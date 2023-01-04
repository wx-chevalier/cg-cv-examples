/****************************************************************************
** Copyright (c) 2022, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "test_measure.h"

#include "../src/base/geom_utils.h"
#include "../src/base/unit_system.h"
#include "../src/measure/measure_tool_brep.h"

#include <BRepAdaptor_Curve.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomConvert_ApproxCurve.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeEllipse.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include <QtCore/QtDebug>

namespace Mayo {

namespace {

bool compareCircle(const gp_Circ& lhs, const gp_Circ& rhs, double tolerance = Precision::Confusion())
{
    return lhs.Location().IsEqual(rhs.Location(), tolerance)
            && lhs.Axis().Direction().IsEqual(rhs.Axis().Direction(), tolerance)
            && std::abs(lhs.Radius() - rhs.Radius()) < tolerance;
}

} // namespace

void TestMeasure::BRepVertexPosition_test()
{
    const gp_Pnt pnt(154.5, 0.87, -487.64);
    const TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(pnt);
    const gp_Pnt pntRes = MeasureToolBRep::brepVertexPosition(vertex);
    QVERIFY(pntRes.IsEqual(pnt, Precision::Confusion()));
}

void TestMeasure::BRepCircle_Regular_test()
{
    const gp_Pnt pntCenter{ 75.5, 0.8, 2548.16 };
    const gp_Dir dirNormal{ 1, 1, 1 };
    const double radius = 58.;
    const GC_MakeCircle makeCircle(gp_Ax2(pntCenter, dirNormal), radius);
    const TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(makeCircle.Value(), 0, 1.57);
    const MeasureCircle circleRes = MeasureToolBRep::brepCircle(edge);
    QVERIFY(circleRes.pntAnchor.IsEqual(GeomUtils::d0(BRepAdaptor_Curve(edge), 0), Precision::Confusion()));
    QVERIFY(circleRes.isArc);
    QVERIFY(compareCircle(circleRes.value, makeCircle.Value()->Circ()));
}

void TestMeasure::BRepCircle_Ellipse_test()
{
    const gp_Pnt pntCenter{ -57.4, 4487.56, 1.8 };
    const gp_Dir dirNormal{ 1, 0, 1 };
    const double radius = 95.;
    const GC_MakeEllipse makeEllipse(gp_Ax2(pntCenter, dirNormal), radius, radius);
    const TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(makeEllipse.Value(), 0, 2.27);
    const MeasureCircle circleRes = MeasureToolBRep::brepCircle(edge);
    QVERIFY(circleRes.pntAnchor.IsEqual(GeomUtils::d0(BRepAdaptor_Curve(edge), 0), Precision::Confusion()));
    QVERIFY(circleRes.isArc);
    QVERIFY(compareCircle(circleRes.value, gp_Circ(gp_Ax2(pntCenter, dirNormal), radius)));
}

void TestMeasure::BRepCircle_PseudoCircle_test()
{
    const gp_Pnt pntCenter{ 41.85, 1547.27, 45.89 };
    const gp_Dir dirNormal{ 0.5, 1.25, 0.8 };
    const double radius = 25.48;
    const GC_MakeCircle makeCircle(gp_Ax2(pntCenter, dirNormal), radius);
    GeomConvert_ApproxCurve approxCircle(makeCircle.Value(), Precision::Approximation(), GeomAbs_C1, 2048, 8);
    QVERIFY(approxCircle.IsDone());
    QVERIFY(approxCircle.HasResult());
    const TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(approxCircle.Curve(), 0, 2.98);
    const MeasureCircle circleRes = MeasureToolBRep::brepCircle(edge);
    QVERIFY(circleRes.pntAnchor.IsEqual(GeomUtils::d0(BRepAdaptor_Curve(edge), 0), Precision::Confusion()));
    QVERIFY(circleRes.isArc);
    QVERIFY(compareCircle(circleRes.value, makeCircle.Value()->Circ(), Precision::Approximation()));
}

void TestMeasure::BRepMinDistance_TwoPoints_test()
{
    const gp_Pnt pnt1{ 41.85, 1547.27, 45.89 };
    const gp_Pnt pnt2{ -57.4, 4487.56, 1.8 };
    const TopoDS_Shape shape1 = BRepBuilderAPI_MakeVertex(pnt1);
    const TopoDS_Shape shape2 = BRepBuilderAPI_MakeVertex(pnt2);
    const MeasureMinDistance minDist = MeasureToolBRep::brepMinDistance(shape1, shape2);
    QVERIFY(minDist.pnt1.IsEqual(pnt1, Precision::Confusion()));
    QVERIFY(minDist.pnt2.IsEqual(pnt2, Precision::Confusion()));
    QCOMPARE(UnitSystem::millimeters(minDist.value).value, pnt1.Distance(pnt2));
}

void TestMeasure::BRepMinDistance_TwoBoxes_test()
{
    const gp_Pnt box1_min{ 5, 5, 5 };
    const gp_Pnt box1_max{ 20, 7, 7 };
    const gp_Pnt box2_min{ 40, 5, 5 };
    const gp_Pnt box2_max{ 55, 7, 7 };
    const TopoDS_Shape shape1 = BRepPrimAPI_MakeBox(box1_min, box1_max);
    const TopoDS_Shape shape2 = BRepPrimAPI_MakeBox(box2_min, box2_max);
    const MeasureMinDistance minDist = MeasureToolBRep::brepMinDistance(shape1, shape2);
    QCOMPARE(UnitSystem::millimeters(minDist.value).value, std::abs(box1_max.X() - box2_min.X()));
    QCOMPARE(UnitSystem::millimeters(minDist.value).value, minDist.pnt1.Distance(minDist.pnt2));
}

void TestMeasure::BRepAngle_TwoLinesIntersect_test()
{
    const TopoDS_Shape shape1 = BRepBuilderAPI_MakeEdge(gp_Lin(gp::Origin(), gp::DX()));
    const TopoDS_Shape shape2 = BRepBuilderAPI_MakeEdge(gp_Lin(gp::Origin(), gp::DY()));
    const MeasureAngle angle = MeasureToolBRep::brepAngle(shape1, shape2);
    QVERIFY(angle.pntCenter.IsEqual(gp::Origin(), Precision::Confusion()));
    QCOMPARE(angle.value, 90. * Quantity_Degree);
}

void TestMeasure::BRepAngle_TwoLinesParallelError_test()
{
    const TopoDS_Shape shape1 = BRepBuilderAPI_MakeEdge(gp_Lin(gp::Origin(), gp::DX()));
    const TopoDS_Shape shape2 = BRepBuilderAPI_MakeEdge(gp_Lin({ 0, 5, 5 }, gp::DX()));
    QVERIFY_EXCEPTION_THROWN(MeasureToolBRep::brepAngle(shape1, shape2), IMeasureError);
}

} // namespace Mayo
