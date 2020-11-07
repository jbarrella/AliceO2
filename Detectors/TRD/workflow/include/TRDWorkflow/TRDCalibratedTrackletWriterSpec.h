// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef O2_TRDCALIBRATEDTRACKLETWRITER_H
#define O2_TRDCALIBRATEDTRACKLETWRITER_H

#include "TRDBase/Digit.h"
#include <SimulationDataFormat/MCTruthContainer.h>
#include "TRDBase/MCLabel.h"
#include "TRDBase/Tracklet.h"
#include <fstream>
#include <iostream>

#include "DataFormatsTRD/CalibratedTracklet.h"
#include "Framework/DataProcessorSpec.h"


namespace o2
{
namespace trd
{

framework::DataProcessorSpec getTRDCalibratedTrackletWriterSpec();


} // end namespace trd
} // end namespace o2
#endif
