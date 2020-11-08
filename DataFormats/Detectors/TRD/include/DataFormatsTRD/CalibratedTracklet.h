// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef O2_TRD_CALIBRATEDTRACKLET_H
#define O2_TRD_CALIBRATEDTRACKLET_H

namespace o2
{
namespace trd
{

class CalibratedTracklet
{
 private:
  double mx;
  double my;
  double mz;
  double mdy;

 public:
  CalibratedTracklet()
  {
  }
  CalibratedTracklet(double x, double y, double z, double dy)
  {
    mx = x;
    my = y;
    mz = z;
    mdy = dy;
  }
  ~CalibratedTracklet() = default;

  double getX() { return mx; }
  double getY() { return my; }
  double getZ() { return mz; }
  double getDy() { return mdy; }

  void setX(double x) { mx = x; }
  void setY(double y) { my = y; }
  void setZ(double z) { mz = z; }
  void setDy(double dy) { mdy = dy; }
};

} // namespace trd
} // namespace o2

#endif