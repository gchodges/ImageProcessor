#pragma once

#include <cstdint>
#include <type_traits>
#include <algorithm>

namespace ImageProcessor {

enum class ColorSpace {
    RGB,
    YCbCr,
};

// Type-aware value clamping
template<typename T>
struct ColorValueTraits {
    static T clamp(T value) {
        if constexpr (std::is_floating_point_v<T>) {
            return std::clamp(value, T(0.0), T(1.0));
        } else if constexpr (std::is_integral_v<T>) {
            return std::clamp(value, T(0), std::numeric_limits<T>::max());
        } else {
            return value;
        }
    }
};

// Forward declaration of color conversion traits
template<typename T, ColorSpace FromSpace, ColorSpace ToSpace>
struct ColorSpaceConversionTraits;

// Primary Pixel Template Class
template<typename T, ColorSpace Space>
class Pixel {
public:
    // Base conversion method that uses traits
    template<ColorSpace ToSpace>
    Pixel<T, ToSpace> convertTo() const {
        if constexpr (space == ToSpace) {
            return *this;
        } else {
            // Static dispatch based on compile-time color space
            return convertToImpl<ToSpace>(std::integral_constant<ColorSpace, Space>{});
        }
    }

private:
    template<ColorSpace ToSpace>
    Pixel<T, ToSpace> convertToImpl(std::integral_constant<ColorSpace, Space>) const {
        return ColorSpaceConversionTraits<T, Space, ToSpace>::convert(*this);
    }

    static constexpr ColorSpace space = Space;
};

// RGB Pixel Specialization
template<typename T>
class Pixel<T, ColorSpace::RGB> {
public:
    Pixel(T red = 0, T green = 0, T blue = 0) : r(red), g(green), b(blue) {}

    // Accessor methods
    T getR() const { return r; }
    T getG() const { return g; }
    T getB() const { return b; }

    void setR(T red) { r = ColorValueTraits<T>::clamp(red); }
    void setG(T green) { g = ColorValueTraits<T>::clamp(green); }
    void setB(T blue) { b = ColorValueTraits<T>::clamp(blue); }

    ColorSpace getSpace() { return space; }

    // Conversion method
    template<ColorSpace ToSpace>
    Pixel<T, ToSpace> convertTo() const {
        return ColorSpaceConversionTraits<T, ColorSpace::RGB, ToSpace>::convert(*this);
    }
private:
    T r, g, b;
    static constexpr ColorSpace space = ColorSpace::RGB;
};

// YCbCr Pixel Specialization
template<typename T>
class Pixel<T, ColorSpace::YCbCr> {
public:

    Pixel(T y_value = 0, T cb_value = 0, T cr_value = 0) 
        : y(y_value), cb(cb_value), cr(cr_value) {}

    // Accessor methods
    T getY() const { return y; }
    T getCb() const { return cb; }
    T getCr() const { return cr; }

    void setY(T y_value) { y = ColorValueTraits<T>::clamp(y_value); }
    void setCb(T cb_value) { cb = ColorValueTraits<T>::clamp(cb_value); }
    void setCr(T cr_value) { cr = ColorValueTraits<T>::clamp(cr_value); }

    ColorSpace getSpace() { return space; }

    // Conversion method
    template<ColorSpace ToSpace>
    Pixel<T, ToSpace> convertTo() const {
        return ColorSpaceConversionTraits<T, ColorSpace::YCbCr, ToSpace>::convert(*this);
    }
private:
    T y, cb, cr;
    static constexpr ColorSpace space = ColorSpace::YCbCr;
};

// Conversion Traits for RGB to YCbCr
template<typename T>
struct ColorSpaceConversionTraits<T, ColorSpace::RGB, ColorSpace::YCbCr> {
    static Pixel<T, ColorSpace::YCbCr> convert(const Pixel<T, ColorSpace::RGB>& rgb) {
        T y  = static_cast<T>(0.2126 * static_cast<double>(rgb.getR()) + 0.7152 * static_cast<double>(rgb.getG())
                                + 0.0722 * static_cast<double>(rgb.getB()));
        T cb = static_cast<T>(-0.1146 * static_cast<double>(rgb.getR()) - 0.3854 * static_cast<double>(rgb.getG())
                                + 0.5 * static_cast<double>(rgb.getB()) + 128.0f);
        T cr = static_cast<T>(0.5 * static_cast<double>(rgb.getR()) - 0.4542 * static_cast<double>(rgb.getG())
                                - 0.0458 * static_cast<double>(rgb.getB()) + 128.0f);

        y = ColorValueTraits<T>::clamp(y);
        cb = ColorValueTraits<T>::clamp(cb);
        cr = ColorValueTraits<T>::clamp(cr);

        return Pixel<T, ColorSpace::YCbCr>(y, cb, cr);
    }
};

// Conversion Traits for YCbCr to RGB
template<typename T>
struct ColorSpaceConversionTraits<T, ColorSpace::YCbCr, ColorSpace::RGB> {
    static Pixel<T, ColorSpace::RGB> convert(const Pixel<T, ColorSpace::YCbCr>& ycbcr) {
        T r = static_cast<T>(static_cast<double>(ycbcr.getY())
                                + 1.5748 * (static_cast<double>(ycbcr.getCr()) - 128.0f));
        T g = static_cast<T>(static_cast<double>(ycbcr.getY())
                                - 0.1873 * (static_cast<double>(ycbcr.getCb()) - 128.0f)
                                - 0.4681 * (static_cast<double>(ycbcr.getCr()) - 128.0f));
        T b = static_cast<T>(static_cast<double>(ycbcr.getY())
                                + 1.8556 * (static_cast<double>(ycbcr.getCb()) - 128.0f));

        // Clamp values to prevent overflow in RGB range
        r = ColorValueTraits<T>::clamp(r);
        g = ColorValueTraits<T>::clamp(g);
        b = ColorValueTraits<T>::clamp(b);

        return Pixel<T, ColorSpace::RGB>(r, g, b);
    }
};

} // namespace ImageProcessor
