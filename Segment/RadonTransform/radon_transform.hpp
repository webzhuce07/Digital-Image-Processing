#include "opencv2/core.hpp"

using namespace cv;

  namespace radon {

    /**
     * Flags for range of angles
     * AR_x_y : theta lies in [x, y] ( both x and y inclusive )
     */

    enum RadonAngleRange { AR_1_45 = 1,
                           AR_46_89 = 2,
                           AR_90 = 4,
                           AR_91_135 = 8,
                           AR_136_179 = 16,
                           AR_180 = 32
                        };

    /**
     * Specifies binary operations
     * The enum specifies four binary operations,
     * 				viz. Minimum, Maximum, Average and sum
     * of the Radon transformed images.
     */

    enum RadonOp { RT_MIN = 1,
                   RT_MAX = 2,
                   RT_AVE = 4,
                   RT_SUM = 8
                 };

    /**
     * [radonTransform Computes the Radon transform of the given image]
     * @param src        [Input source image]
     * @param dst        [Output image]
     * @param angleRange [One of the enums, RadonAngleRange, defaults to 63 ( 1 to 180 degrees)]
     * @param operation  [One of the enums, RadonOp, defaults to RT_SUM ( SUM OPERATION )]
     * @param opDst      [Output image of the operation perfomed, defaults to noArray()]
     */
    CV_EXPORTS void radonTransform( InputArray src,
                                    OutputArray dst,
                                    int angleRange = 63,
                                    int operation = RT_SUM,
                                    OutputArray opDst = noArray()
                                  );
  }
