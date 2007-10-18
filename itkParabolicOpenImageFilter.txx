#ifndef __itkParabolicOpenImageFilter_txx
#define __itkParabolicOpenImageFilter_txx

#include "itkProgressAccumulator.h"

namespace itk
{

template <typename TInputImage, typename TOutputImage>
void
ParabolicOpenImageFilter<TInputImage, TOutputImage>
::GenerateData(void)
{

  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  InputImageConstPointer inputImage;
  unsigned long Bounds[ImageDimension];
  typename TInputImage::SizeType BoundsSize;
  if (this->m_SafeBorder) 
    {
    // need to compute some image statistics and determine the padding
    // extent. This will almost certainly be an over estimate
    m_StatsFilt->SetInput(this->GetInput());
    m_StatsFilt->Update();
    InputPixelType range = m_StatsFilt->GetMaximum() - m_StatsFilt->GetMinimum();
    typename MorphFilterType::RadiusType Sigma = m_MorphFilt->GetSigma();
    for (unsigned s = 0; s < ImageDimension;s++)
      {
      if (m_MorphFilt->GetUseImageSpacing())
	{
	RealType image_scale = this->GetInput()->GetSpacing()[s];
	Bounds[s] = (unsigned long)ceil(sqrt(2*(Sigma[s]/image_scale)*range));
	BoundsSize[s] = Bounds[s];
	}
      else
	{
	Bounds[s] = (unsigned long)ceil(sqrt(2*Sigma[s]*range));
	BoundsSize[s] = Bounds[s];
	}
      }

    m_PadFilt->SetPadLowerBound(Bounds);
    m_PadFilt->SetPadUpperBound(Bounds);
    m_PadFilt->SetConstant(NumericTraits<InputPixelType>::max());
    m_PadFilt->SetInput(m_StatsFilt->GetOutput());
    progress->RegisterInternalFilter(m_PadFilt, 0.1f );
    inputImage = m_PadFilt->GetOutput();
    }
  else
    {
    inputImage = this->GetInput();
    }

  m_MorphFilt->SetInput(inputImage);
  progress->RegisterInternalFilter(m_MorphFilt, 0.8f);

  if (this->m_SafeBorder) 
    {
    // crop
    m_CropFilt->SetInput(m_MorphFilt->GetOutput());
    m_CropFilt->SetUpperBoundaryCropSize(BoundsSize);
    m_CropFilt->SetLowerBoundaryCropSize(BoundsSize);
    progress->RegisterInternalFilter(m_CropFilt, 0.1f );
    m_CropFilt->Update();
    this->GraftOutput( m_CropFilt->GetOutput() );
    }
  else
    {
    m_MorphFilt->Update();
    this->GraftOutput( m_MorphFilt->GetOutput() );
    std::cout << "Finished grafting" << std::endl;
    }

}


///////////////////////////////////
template<typename TInputImage, typename TOutputImage>
void
ParabolicOpenImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  os << indent << "SafeBorder: " << m_SafeBorder << std::endl;
}
} //namespace itk

#endif
