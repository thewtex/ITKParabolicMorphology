/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkParabolicOpenImageFilter_h
#define itkParabolicOpenImageFilter_h

#include "itkParabolicOpenCloseSafeBorderImageFilter.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * \class ParabolicOpenImageFilter
 * \brief Class for morphological opening
 * operations  with parabolic structuring elements.
 *
 * This filter provides options for padded borders
 *
 * This filter is threaded.
 *
 * Core methods described in the InsightJournal article:
 * "Morphology with parabolic structuring elements"
 *
 * https://hdl.handle.net/1926/1370
 *
 * \sa itkParabolicOpenCloseImageFilter
 *
 * \ingroup ParabolicMorphology
 *
 * \author Richard Beare, Department of Medicine, Monash University,
 * Australia.  <Richard.Beare@monash.edu>
 **/

template <typename TInputImage, typename TOutputImage = TInputImage>
class ITK_TEMPLATE_EXPORT ParabolicOpenImageFilter
  : public ParabolicOpenCloseSafeBorderImageFilter<TInputImage, true, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(ParabolicOpenImageFilter);

  /** Standard class type alias. */
  using Self = ParabolicOpenImageFilter;
  using Superclass = ParabolicOpenCloseSafeBorderImageFilter<TInputImage, true, TOutputImage>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Pixel Type of the input image */
  using InputImageType = TInputImage;
  using OutputImageType = TOutputImage;
  using PixelType = typename TInputImage::PixelType;
  using RealType = typename NumericTraits<PixelType>::RealType;
  using ScalarRealType = typename NumericTraits<PixelType>::ScalarRealType;
  using OutputPixelType = typename TOutputImage::PixelType;

  /** Smart pointer type alias support.  */
  using InputImagePointer = typename TInputImage::Pointer;
  using InputImageConstPointer = typename TInputImage::ConstPointer;

  /** a type to represent the "kernel radius" */
  using RadiusType = typename itk::FixedArray<ScalarRealType, TInputImage::ImageDimension>;

  /** Image dimension. */
  static constexpr unsigned int ImageDimension = TInputImage::ImageDimension;
  /** Define the image type for internal computations
      RealType is usually 'double' in NumericTraits.
      Here we prefer float in order to save memory.  */
protected:
  ParabolicOpenImageFilter() = default;
  ~ParabolicOpenImageFilter() override = default;
  //   void PrintSelf(std::ostream& os, Indent indent) const;
};
} // end namespace itk

#endif
