#ifndef __itkDisplacedDetectorImageFilter_h
#define __itkDisplacedDetectorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkThreeDCircularProjectionGeometry.h"
#include "rtkConfiguration.h"

/** \class DisplacedDetectorImageFilter
 *
 * Weighting of image projections to handle off-centered panels
 * in tomography reconstruction. Based on [Wang, Med Phys, 2002].
 *
 * Note that the filter does nothing if the panel shift is less than 10%
 * of its size. Otherwise, it does the weighting described in the publication
 * and zero pads the data on the nearest side to the center.
 * Therefore, the InPlace capability depends on the displacement.
 * It can only be inplace if there is no displacement, it can not otherwise.
 * The GenerateOutputInformation method takes care of properly setting this up.
 *
 * By default, it computes the minimum and maximum offsets from the complete geometry object.
 * When an independent projection has to be processed, these values have to be set by the user from
 * a priori knowledge of the detector displacements.
 *
 * \author Simon Rit
 */
namespace itk
{

template<class TInputImage, class TOutputImage=TInputImage>
class ITK_EXPORT DisplacedDetectorImageFilter :
  public InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef DisplacedDetectorImageFilter Self;

  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;

  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                                     InputImageType;
  typedef TOutputImage                                    OutputImageType;
  typedef typename OutputImageType::RegionType            OutputImageRegionType;
  typedef itk::Image<typename TOutputImage::PixelType, 1> WeightImageType;

  typedef ThreeDCircularProjectionGeometry GeometryType;
  typedef GeometryType::Pointer            GeometryPointer;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(DisplacedDetectorImageFilter, ImageToImageFilter);

  /** Get / Set the object pointer to projection geometry */
  itkGetMacro(Geometry, GeometryPointer);
  itkSetMacro(Geometry, GeometryPointer);

  /**
   * Get / Set the minimum and maximum offsets of the detector along the weighting direction.
   */
  void SetOffsets(double minOffset, double maxOffset);
  itkGetMacro(MinimumOffset, double);
  itkGetMacro(MaximumOffset, double);

protected:
  DisplacedDetectorImageFilter();

  ~DisplacedDetectorImageFilter(){
  }

  virtual void GenerateInputRequestedRegion();

  virtual void GenerateOutputInformation();

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, ThreadIdType threadId );

private:
  DisplacedDetectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);               //purposely not implemented

  /** RTK geometry object */
  GeometryPointer m_Geometry;

  /**
   * Minimum and maximum offsets of the detector along the weighting direction, i.e. x.
   * If a priori known, these values can be given as input. Otherwise, they are computed from the
   * complete geometry.
   */
  double m_MinimumOffset;
  double m_MaximumOffset;

  /**
   * Flag used to know if the user has entered the min/max values of the detector offset.
   */
  bool m_OffsetsSet;

  /** Superior and inferior position of the detector along the weighting direction, i.e. x.
   * The computed value account for the x projection offset of the geometry.
   */
  double m_InferiorCorner;
  double m_SuperiorCorner;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDisplacedDetectorImageFilter.txx"
#endif

#endif