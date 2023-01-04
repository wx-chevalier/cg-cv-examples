/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include <utility>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
class Adaptor3d_Curve;

namespace Mayo {
namespace GeomUtils {

double normalizedU(const Adaptor3d_Curve& curve, double u);
gp_Pnt d0(const Adaptor3d_Curve& curve, double u);
gp_Vec d1(const Adaptor3d_Curve& curve, double u);
std::pair<gp_Pnt, gp_Vec> d0d1(const Adaptor3d_Curve& curve, double u);

} // namespace GeomUtils
} // namespace Mayo
