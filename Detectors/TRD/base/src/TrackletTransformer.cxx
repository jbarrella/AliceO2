// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "TRDBase/TrackletTransformer.h"
#include "TMath.h"

using namespace o2::trd;

TrackletTransformer::TrackletTransformer()
{
  mgeo = o2::trd::Geometry::instance();
  mgeo->createPadPlaneArray();
}

std::vector<double> TrackletTransformer::transformToSpacePoint(int hcid, int padrow, int column, int position)
{
  int detector = hcid / 2;
  int stack = mgeo->getStack(detector);
  int layer = mgeo->getLayer(detector);
  auto padPlane = mgeo->getPadPlane(layer, stack);

  // (I=inner, O=outer) -- inner: pads [1,142] | outer: pads 0,143
  // pad dimension in rphi-direction
  double padWidth = padPlane->getWidthIPad();

  // pad dimension in z-direction
  double padLength = padPlane->getLengthIPad();
  double oPadLength = padPlane->getLengthOPad();

  int nPadRows = padPlane->getNrows();

  float driftHeight = mgeo->cdrHght();

  // 5mm below cathode plane to reduce error propogation from tracklet fit and driftV
  double x = driftHeight - 0.5;

  double y;

  if (hcid % 2 == 0) {
    // +1.5-9 to go from center pad 10 to physical MCM center | -1 for pad 0 overlap
    // outer padWidth irrelevant since point will never require a shift = outer padWidth since no other pads are beyond this
    y = (((double)position - 1024) * 1 / 75. + 1.5 - 9 - 1) * padWidth + ((double)column - 3) * padWidth * 18;
  } else {
    y = (((double)position - 1024) * 1 / 75. + 1.5 + 9 - 1) * padWidth + (double)column * padWidth * 18;
  }

  // reported at the center of padrow
  // check whether padrow 0 is at largest z or smallest z ?
  double z = ((double)padrow - nPadRows / 2 + 0.5) * padLength;

  if (padrow == nPadRows - 1) {
    z += oPadLength - padLength;
  }
  if (padrow == 0) {
    z -= oPadLength - padLength;
  }

  return {x, y, z};
}

double TrackletTransformer::calculateDy(int hcid, int slope)
{
  int detector = hcid / 2;
  int stack = mgeo->getStack(detector);
  int layer = mgeo->getLayer(detector);
  auto padPlane = mgeo->getPadPlane(layer, stack);
  double padWidth = padPlane->getWidthIPad();

  float vDrift = 1.56;
  float driftHeight = mgeo->cdrHght();
  
  // dy = slope * nTimeBins * padWidth * 1/1000;
  // nTimeBins should be number of timebins in drift region. 1 timebin is 100 nanosecond
  double dy = slope * ((driftHeight / vDrift) / 0.1) * padWidth * 1 / 1000.;

  return dy;
}

double TrackletTransformer::calibrateX(double x, double t0Correction)
{
  return x += t0Correction;
}

double TrackletTransformer::calibrateDy(double rawDy, double oldLorentzAngle, double lorentzAngle, double driftVRatio)
{
  // LOG(info) << "RAW: " << rawDy;

  // driftDistance = 3.35
  float driftDistance = mgeo->cdrHght() + mgeo->camHght();

  float cmSlope = rawDy / driftDistance;

  double calibratedDy = rawDy - (TMath::Tan(lorentzAngle) * driftDistance) + (TMath::Tan(oldLorentzAngle) * driftDistance * driftVRatio) + cmSlope * (driftDistance * (1 - driftVRatio));

  // ALTERNATIVE METHOD

  // double x_anode_hit = driftDistance*driftVRatio/cmSlope;
  // double y_anode_hit = driftDistance*driftVRatio;

  // double x_Lorentz_drift_hit = TMath::Tan(oldLorentzAngle)*driftDistance*driftVRatio - TMath::Tan(lorentzAngle)*driftDistance;
  // double y_Lorentz_drift_hit = driftDistance*driftVRatio - driftDistance;

  // double Delta_x_Lorentz_drift_hit = x_anode_hit - x_Lorentz_drift_hit;
  // double Delta_y_Lorentz_drift_hit = y_anode_hit - y_Lorentz_drift_hit;
  // double impact_angle_rec = TMath::ATan2(Delta_y_Lorentz_drift_hit,Delta_x_Lorentz_drift_hit);

  // float calibrationShift = TMath::Tan(impact_angle_rec) * driftDistance;

  // LOG(info) << "ORIGINAL: " << calibratedDy;
  // LOG(info) << "ALTERNATIVE: " << rawDy + calibrationShift;

  return calibratedDy;
}