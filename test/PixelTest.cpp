#include <gtest/gtest.h>
#include "../include/Pixel.hpp"

using namespace ImageProcessor;

// Pixel Class Unit Tests
class PixelTest : public ::testing::Test {
protected:
};

// Test Pixel Constructors and Basic Functionality
TEST_F(PixelTest, Constructor) {
    // Default constructor (RGB space)
    Pixel<uint8_t, ColorSpace::RGB> defaultPixel;
    EXPECT_EQ(defaultPixel.getR(), 0);
    EXPECT_EQ(defaultPixel.getG(), 0);
    EXPECT_EQ(defaultPixel.getB(), 0);
    EXPECT_EQ(defaultPixel.getSpace(), ColorSpace::RGB);

    // Test set/get
    defaultPixel.setR(10);
    defaultPixel.setG(100);
    defaultPixel.setB(91);
    EXPECT_EQ(defaultPixel.getR(), 10);
    EXPECT_EQ(defaultPixel.getG(), 100);
    EXPECT_EQ(defaultPixel.getB(), 91);

    // Default constructor (YCbCr space)
    Pixel<uint8_t, ColorSpace::YCbCr> defaultPixelYCbCr;
    EXPECT_EQ(defaultPixelYCbCr.getY(), 0);
    EXPECT_EQ(defaultPixelYCbCr.getCb(), 0);
    EXPECT_EQ(defaultPixelYCbCr.getCr(), 0);
    EXPECT_EQ(defaultPixelYCbCr.getSpace(), ColorSpace::YCbCr);

    // Test set/get
    defaultPixelYCbCr.setY(11);
    defaultPixelYCbCr.setCb(254);
    defaultPixelYCbCr.setCr(100);
    EXPECT_EQ(defaultPixelYCbCr.getY(), 11);
    EXPECT_EQ(defaultPixelYCbCr.getCb(), 254);
    EXPECT_EQ(defaultPixelYCbCr.getCr(), 100);

    // Parameterized constructor (RGB space)
    Pixel<uint8_t, ColorSpace::RGB> rgbPixel(100, 150, 200);
    EXPECT_EQ(rgbPixel.getR(), 100);
    EXPECT_EQ(rgbPixel.getG(), 150);
    EXPECT_EQ(rgbPixel.getB(), 200);
    EXPECT_EQ(rgbPixel.getSpace(), ColorSpace::RGB);

    // Parameterized constructor (YCbCr space)
    Pixel<uint8_t, ColorSpace::YCbCr> ycbcrPixel(100, 150, 200);
    EXPECT_EQ(ycbcrPixel.getY(), 100);
    EXPECT_EQ(ycbcrPixel.getCb(), 150);
    EXPECT_EQ(ycbcrPixel.getCr(), 200);
    EXPECT_EQ(ycbcrPixel.getSpace(), ColorSpace::YCbCr);
}

// Test Color Space Conversion
TEST_F(PixelTest, ColorSpaceConversion) {
    // RGB to YCbCr conversion test
    Pixel<uint8_t, ColorSpace::RGB> rgbPixel(100, 150, 200);
    auto ycbcrPixel = rgbPixel.convertTo<ColorSpace::YCbCr>();

    // Verify conversion (these values are approximate)
    EXPECT_NEAR(ycbcrPixel.getY(), 142, 5);
    EXPECT_NEAR(ycbcrPixel.getCb(), 158, 5);
    EXPECT_NEAR(ycbcrPixel.getCr(), 100, 5);
    EXPECT_EQ(ycbcrPixel.getSpace(), ColorSpace::YCbCr);
}

// Test Conversion from YCbCr to RGB
TEST_F(PixelTest, YCbCrToRGBConversion) {
    Pixel<uint8_t, ColorSpace::YCbCr> ycbcrPixel(142, 158, 100);
    auto rgbPixel = ycbcrPixel.convertTo<ColorSpace::RGB>();

    EXPECT_NEAR(rgbPixel.getR(), 100, 5);
    EXPECT_NEAR(rgbPixel.getG(), 150, 5);
    EXPECT_NEAR(rgbPixel.getB(), 200, 5);
    EXPECT_EQ(rgbPixel.getSpace(), ColorSpace::RGB);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
