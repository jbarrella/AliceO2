// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

// Declaration of a transient MC label class for FIT

#ifndef ALICEO2_FIT_MCLABEL_H_
#define ALICEO2_FIT_MCLABEL_H_

#include "SimulationDataFormat/MCCompLabel.h"

namespace o2
{
namespace ft0
{
class MCLabel : public o2::MCCompLabel
{
 private:
  Int_t mDetID = -1;

 public:
  MCLabel() = default;
  MCLabel(Int_t trackID, Int_t eventID, Int_t srcID, Int_t chID)
    : o2::MCCompLabel(trackID, eventID, srcID, false), mDetID(chID) {}

  Int_t getDetID() const { return mDetID; }

  ClassDefNV(MCLabel, 1);
};
} // namespace ft0
} // namespace o2

#endif
