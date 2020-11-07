// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.


#include "TRDBase/CoordinateTransformer.h"

using namespace o2::trd;


CoordinateTransformer::CoordinateTransformer()
{
    mgeo = o2::trd::Geometry::instance();
    mgeo->createPadPlaneArray();
}


std::vector<double> CoordinateTransformer::transformToSpacePoint(int hcid, int padrow, int column, int position)
{
    int idet = hcid/2;
    int ilayer = (idet % 30) % 6;
    int istack = (idet % 30) / 6;
    auto padPlane = mgeo->getPadPlane(ilayer, istack);

    // inner: pads [1,142] | outer: pads 0,143
    double padWidth = padPlane->getWidthIPad();             // pad dimension in rphi-direction
    
    double padLength = padPlane->getLengthIPad();           // pad dimension in z-direction (I=inner, O=outer)
    double oPadLength = padPlane->getLengthOPad();
    
    int nPadRows = padPlane->getNrows();
   
    float driftHeight = mgeo->cdrHght();


    double x = driftHeight - 0.5;           // 5mm below cathode plane

    double y;
    if (hcid % 2 == 0)
    {                                       // +1.5-9 to go from center pad 10 to physical MCM center | -1 for pad 0 overlap
      y = (((double)position - 1024) * 1/75. + 1.5 - 9 - 1) * padWidth + ((double)column - 3) * padWidth * 18;     // upper edge of pad 71 at y = 0
    }
    else
    {
      y = (((double)position - 1024) * 1/75. + 1.5 + 9 - 1) * padWidth + (double)column * padWidth * 18;
    }
    
    double z = ((double)padrow - nPadRows/2 + 0.5) * padLength;          // make this the middle of the pad. this is a lower bound on z. z is between padrow*padLength and (padrow+1)*padLength
    if (padrow == nPadRows-1)
    {
      z += oPadLength - padLength;
    }
    if (padrow == 0)
    {
      z -= oPadLength - padLength;
    }

    return {x, y, z};
}


double CoordinateTransformer::calculateDy(int hcid, int slope)
{
    int idet = hcid/2;
    int ilayer = (idet % 30) % 6;
    int istack = (idet % 30) / 6;
    auto padPlane = mgeo->getPadPlane(ilayer, istack);

    float driftHeight = mgeo->cdrHght();
    float vDrift = 1.56;
    double padWidth = padPlane->getWidthIPad();

    // nTimeBins should be number of timebins in drift region. 1 timebin is 100 nanosecond
    // double dy = slope * nTimeBins * padWidth * 1/1000;
    double dy = slope * ((driftHeight / vDrift) / 0.1) * padWidth * 1/1000.;

    return dy;
}