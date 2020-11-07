// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.


#include "TRDWorkflow/TRDCalibratedTrackletWriterSpec.h"
#include "DataFormatsTRD/CalibratedTracklet.h"

#include "Framework/DataProcessorSpec.h"
#include "DPLUtils/MakeRootTreeWriterSpec.h"
#include "Framework/InputSpec.h"

#include "TRDBase/Digit.h"
#include <SimulationDataFormat/MCTruthContainer.h>
#include "TRDBase/MCLabel.h"
#include "DataFormatsTRD/TriggerRecord.h"
#include "DataFormatsTRD/Tracklet64.h"

#include <fstream>
#include <iostream>

using namespace o2::framework;

namespace o2
{
namespace trd
{

template <typename T>
using BranchDefinition = framework::MakeRootTreeWriterSpec::BranchDefinition<T>;

o2::framework::DataProcessorSpec getTRDCalibratedTrackletWriterSpec()
{
  using MakeRootTreeWriterSpec = framework::MakeRootTreeWriterSpec;

  return MakeRootTreeWriterSpec("ctracklet-writer",
                                "trdcalibratedtracklets.root",
                                "ctracklets",
                                1,
                                BranchDefinition<std::vector<CalibratedTracklet>>{InputSpec{"ctracklets", "TRD", "CALIBRATEDTRACKLETS"}, "CTracklets"})();
                                // BranchDefinition<std::vector<o2::trd::TriggerRecord>>{InputSpec{"tracklettrigs", "TRD", "TRKTRGRD"}, "TrackTrg"})();
};

} // end namespace trd
} // end namespace o2
