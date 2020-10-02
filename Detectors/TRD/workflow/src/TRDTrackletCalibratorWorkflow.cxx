// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "TRDWorkflow/TRDTrackletCalibratorSpec.h"

#include "Framework/DeviceSpec.h"
#include "Framework/runDataProcessing.h"
#include "Framework/WorkflowSpec.h"
#include "Framework/ConfigParamSpec.h"

#include <cmath>
#include <string>


using namespace o2::framework;

// ------------------------------------------------------------------

void customize(std::vector<o2::framework::ConfigParamSpec>& workflowoptions)
{
  // might need this
}


/// This function is required to be implemented to define the workflow
/// specifications
WorkflowSpec defineDataProcessing(ConfigContext const& configcontext)
{
  return WorkflowSpec{
    o2::trd::getTRDTrackletCalibratorSpec()};
}
