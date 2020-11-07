// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "TRDWorkflow/TRDTrackletTransformerSpec.h"
#include "TRDWorkflow/TRDCalibratedTrackletWriterSpec.h"
#include "TRDWorkflow/TRDTrackletReaderSpec.h"

#include "CommonUtils/ConfigurableParam.h"
#include "Framework/CompletionPolicy.h"


using namespace o2::framework;

// ------------------------------------------------------------------

void customize(std::vector<o2::framework::ConfigParamSpec>& workflowOptions)
{
  workflowOptions.push_back(ConfigParamSpec{
    "root-in", o2::framework::VariantType::Bool, true, {"enable input from ROOT file"}});
  workflowOptions.push_back(ConfigParamSpec{
    "root-out", o2::framework::VariantType::Bool, true, {"enable output to ROOT file"}});

  // std::string keyvaluehelp("Semicolon separated key=value strings ...");
  // workflowOptions.push_back(ConfigParamSpec{"configKeyValues", VariantType::String, "", {keyvaluehelp}});
}


#include "Framework/runDataProcessing.h"


WorkflowSpec defineDataProcessing(ConfigContext const& configcontext)
{
  // o2::conf::ConfigurableParam::updateFromString(configcontext.options().get<std::string>("root-in"));

  bool rootIn = configcontext.options().get<bool>("root-in");
  bool rootOut = configcontext.options().get<bool>("root-out");


  WorkflowSpec spec;

  if (rootIn) spec.emplace_back(o2::trd::getTRDTrackletReaderSpec(0));

  spec.emplace_back(o2::trd::getTRDTrackletTransformerSpec());

  if (rootOut) spec.emplace_back(o2::trd::getTRDCalibratedTrackletWriterSpec());
  

  return spec;
}
