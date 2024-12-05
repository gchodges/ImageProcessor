# Design Document: Image Processing Software

## Overview
This document outlines the design and architecture of an image processing software that will support file conversion and compression for multiple image file types. Additionally, image processing functionality will be added to support rotation, reflection, sharpening, blurring, and resizing of images. Colorspace conversion will also be supported.

---

## Goals and Requirements

### Functional Requirements:
- Convert images between color spaces
- Support multiple precision modes (8-bit, 16-bit).
- Maintain high accuracy during conversions.
- Support clamping and precision handling to minimize errors.
- Support compression from BMP to more sophisticated image file types (.jpg, .png, etc)
- Allow image modification through the command line

### Non-functional Requirements:
- Efficient performance for large image files.
- Modular and extensible architecture.
- Compliance with color space standards (e.g., BT.601, BT.709).

---

## Key Components

### 1. **Pixel Class**
   A templated class to handle individual pixels for both RGB and YCbCr color spaces. This design allows for multiple precision modes and added color spaces through template class specialization.

#### Current Features:
- Support for different data types (`uint8_t`, `uint16_t`, `float`).
- Functions for color space conversion utilizing traits using static dispatch for compile-time implementation selection and type-safety.

### 2. **Color Space Conversion**
   Functions to perform conversion between RGB and YCbCr using BT.709 standards.

#### Conversion Formulae:
- **RGB to YCbCr (BT.601):**
  ```
  Y  = 0.2126R + 0.7152G + 0.0722B
  Cb = -0.1146R - 0.3854G + 0.5B + 128
  Cr = 0.5R - 0.4542G - 0.0458B + 128
  ```

- **YCbCr to RGB (BT.601):**
  ```
  R = Y + 1.5748(Cr - 128)
  G = Y - 0.1873(Cb - 128) - 0.4681(Cr - 128)
  B = Y + 1.8556(Cb - 128)
  ```

#### Precision Handling:
- Use `float` `uint16_t` or `double` for intermediate computations.
- Cast to appropriate integer type (`uint8_t`, `uint16_t`) after clamping results.

### 3. **Image Class**
   A class to represent and manipulate an entire image.

#### Planned Features:
- Storage for pixel data.
- Batch conversion between color spaces.

### 4. **Precision Modes**
   Provide support for 8-bit and 16-bit precision.

#### Benefits:
- 8-bit: Standard for most image processing tasks (e.g., JPEG compression).
- 16-bit: Higher precision, necessary for intermediate calculations and reducing errors during multiple transformations.



## Workflow

1. **Image Loading:**
   - Load image data into `Image` class.
   - Detect color space and precision.

2. **Color Space Conversion:**
   - Use `Pixel::convertTo` for per-pixel transformation.
   - Batch process all pixels in the image.

3. **Image Manipulation:**
   - Perform additional transformations as needed (e.g., DCT, quantization for compression).

4. **Image Saving:**
   - Save the processed image in the desired format and precision.

---

## Optimization Strategies
- Use SIMD (Single Instruction, Multiple Data) for vectorized pixel processing.
- Parallelize image processing using multi-threading or GPU acceleration.
- Cache intermediate results for repeated operations.

---

## Testing Plan
- Validate conversions against standard reference values
- Test clamping and rounding mechanisms for edge cases.
- Compare precision loss for 8-bit and 16-bit modes across multiple transformations.

---

## Future Enhancements
- Add support for additional color spaces (e.g., CMYK, etc).
- Implement compression algorithms (e.g., JPEG pipeline with DCT and quantization).
- Utilize Basys 3 FPGA to create a DCT and quantization hardware accelerator

---

## Conclusion
This design provides a modular, robust foundation for precise and efficient color space conversion and image manipulation. Modular components and flexibility in precision ensure the software is well-suited for a variety of image processing applications, including high-quality compression workflows.

