/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>
#include <Quantity_NameOfColor.hxx>
#include <Image_PixMap.hxx>

#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QGradient>
#include <QtGui/QPixmap>
class QScreen;

#include <type_traits>

namespace Mayo {
namespace QtGuiUtils {

// Color conversion
QColor toQColor(const Quantity_Color& c);
QColor toQColor(const Quantity_ColorRGBA& c);
QColor toQColor(Quantity_NameOfColor c);

template <typename OtherColorType>
OtherColorType toColor(const QColor& c);

template <Quantity_TypeOfColor OtherColorType>
Quantity_Color toColor(const QColor& c);

Quantity_Color toPreferredColorSpace(const QColor& c);

// Image conversion
QPixmap toQPixmap(const Image_PixMap& pixmap);

// Returns linear interpolated color between 'a' and 'b' at parameter 't'
QColor lerp(const QColor& a, const QColor& b, double t);

// Returns color at normalized pos 't' from linear gradient
// Note: assumes that gradient stops array contains keys 0 and 1
QColor linearColorAt(const QGradient& gradient, double t);

// Returns version of 'gradient' having stop keys 0 and 1
QGradient gradientEndsAdded(const QGradient& gradient);

QGradient subGradient(const QGradient& gradient, double t1, double t2);

// Returns corresponding width in pixels proportional to 'screen' width resolution
// 'screenRatio' must be within [0,1]
int screenPixelWidth(double screenRatio, const QScreen* screen = nullptr);
int screenPixelHeight(double screenRatio, const QScreen* screen = nullptr);
QSize screenPixelSize(double widthRatio, double heightRatio, const QScreen* screen = nullptr);

// Fluent-like helper to change font properties
class FontChange {
public:
    FontChange(const QFont& font);

    FontChange& size(int size);
    FontChange& adjustSize(int offset);
    FontChange& scalePointSizeF(double f);
    FontChange& bold(bool on = true);
    FontChange& fixedPitch(bool on = true);

    constexpr operator const QFont&() const { return m_font; }

private:
    QFont m_font;
};

// --
// -- Implementation
// --

template <typename OtherColorType>
OtherColorType toColor(const QColor& c) {
    if constexpr(std::is_same_v<OtherColorType, Quantity_Color>) {
        return Quantity_Color(c.redF(), c.greenF(), c.blueF(), Quantity_TOC_RGB);
    }
    if constexpr(std::is_same_v<OtherColorType, Quantity_ColorRGBA>) {
        return Quantity_ColorRGBA(c.redF(), c.greenF(), c.blueF(), c.alphaF());
    }
    else if constexpr(std::is_same_v<OtherColorType, Quantity_NameOfColor>) {
        return QtGuiUtils::toColor<Quantity_Color>(c).Name();
    }
}

template <Quantity_TypeOfColor OtherColorType>
Quantity_Color toColor(const QColor& c)
{
    return Quantity_Color(c.redF(), c.greenF(), c.blueF(), OtherColorType);
}

} // namespace QtGuiUtils
} // namespace Mayo
