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

#include "TTree.h"

#include "FairLogger.h"
#include "TRDSimulation/Detector.h"
#include "TRDBase/Geometry.h"
#include "TRDBase/Calibrations.h"
#include "DetectorsCommonDataFormats/NameConf.h"
#include "DetectorsCommonDataFormats/DetID.h"

#include "TRDBase/SimParam.h"
#include "TRDBase/CommonParam.h"
#include "DataFormatsTRD/CalibratedTracklet.h"
#include "DataFormatsTRD/Constants.h"


// #include "TRDBase/CoordinateTransformer.h"


using namespace o2::framework;
using SubSpecificationType = o2::framework::DataAllocator::SubSpecificationType;

using namespace o2;
using namespace trd;


void TRDTrackletCalibratorSpec::init(o2::framework::InitContext& ic)
{
  LOG(info) << "initializing tracklet calibrator";

  // CoordinateTransformer transformer;
}


void TRDTrackletCalibratorSpec::run(o2::framework::ProcessingContext& pc)
{
  LOG(info) << "running tracklet calibrator";

  // TH1D* h1d_position = new TH1D("position", "position", 256, 0, 2048);
  // TCanvas* h1d_position_can = new TCanvas("position", "position", 10, 10, 800, 600);

  std::vector<CalibratedTracklet> calibratedTracklets;
  std::vector<Tracklet64> tracklets = pc.inputs().get<std::vector<Tracklet64>>("inTracklets");
  // std::vector<TriggerRecord> triggerRec = pc.inputs().get<std::vector<TriggerRecord>>("triggerRecord");

  // for (int reci=0; reci < triggerRec.size(); reci++)
  // {
  //   LOG(info) << triggerRec[reci].getFirstEntry() << " | " << triggerRec[reci].getNumberOfObjects();
  // }

  // LOG(info) << tracklets.size();

  int nTimeBins = 24;

  auto geo = o2::trd::Geometry::instance();
  geo->createPadPlaneArray();

  for (int itracklet; itracklet < 550; ++itracklet) {

    Tracklet64* tracklet = &tracklets[itracklet];

    // LOG(info) << triggerRec[0].getFirstEntry() << " | " << triggerRec[0].getNumberOfObjects();

    // for (int reci=0; reci < triggerRec.size(); reci++)
    // {
    //   LOG(info) << triggerRec[reci].getFirstEntry() << " | " << triggerRec[reci].getNumberOfObjects();
    // }
    

    double vDrift = 1.56;

    uint64_t hcid = tracklet->getHCID();
    uint64_t padrow = tracklet->getPadRow();        // 0-15
    uint64_t column = tracklet->getColumn();        // 0-3 | always 0 at the moment
    uint64_t position = tracklet->getPosition();    // 0-2048 | units:pad-widths | granularity=1/75 (measured from center pad 10) 1024 is 0/cetner of pad 10
    uint64_t slope = tracklet->getSlope();          // 0-127 | units:pads/timebin | granularity=1/1000


    // LOG(info) << "padrow: " << padrow;
    // LOG(info) << "column: " << column;
    // LOG(info) << "position: " << position;
    // LOG(info) << "slope: " << slope;


    std::vector<double> spacePoint = transformer.transformToSpacePoint(hcid, padrow, column, position);
    double dy = transformer.calculateDy(hcid, slope);

    // h1d_position->Fill(position);

    // LOG(info) << "spx: " << spacePoint[0] << " | " << "spy: " << spacePoint[1] << " | " << "spz: " << spacePoint[2] << " | " << "spdy: " << dy;


    // CALIBRATE dy and x

    float t0Correction = -0.1;

    float oldLorentzAngle = 0.16;
    float lorentzAngle = -0.14;

    float driftVRatio = 1.1;

    // float driftDepth = 3.35;
    float driftDepth = geo->cdrHght() + geo->camHght();


    spacePoint[0] += t0Correction;

    // LOG(info) << "dy: " << dy;

    float cmSlope = dy/driftDepth;

    dy += - (TMath::Tan(lorentzAngle) * driftDepth) \
          + (TMath::Tan(oldLorentzAngle) * driftDepth * driftVRatio) \
          + cmSlope * (driftDepth * (1 - driftVRatio));


    calibratedTracklets.emplace_back(spacePoint[0], spacePoint[1], spacePoint[2], dy);


    //--------------------------------------------------------------------------------------------------------------

    // LOG(info) << "dy: " << dy;
    // LOG(info);s


    // ALTERNATIVE METHOD

    // Double_t x_anode_hit = driftDepth*driftVRatio/slope;
    // Double_t y_anode_hit = driftDepth*driftVRatio;

    // // cout << "(x,y) anode hit: " << x_anode_hit << "  " << y_anode_hit << endl;

    // Double_t x_Lorentz_drift_hit = TMath::Tan(oldLorentzAngle)*driftDepth*driftVRatio - TMath::Tan(lorentzAngle)*driftDepth;
    // Double_t y_Lorentz_drift_hit = driftDepth*driftVRatio - driftDepth;

    // // cout << "(x,y) lorentz drift: " << x_Lorentz_drift_hit << "  " << y_Lorentz_drift_hit << endl;

    // Double_t impact_angle_track = TMath::ATan2(y_anode_hit,x_anode_hit);

    // Double_t Delta_x_Lorentz_drift_hit = x_anode_hit - x_Lorentz_drift_hit;
    // Double_t Delta_y_Lorentz_drift_hit = y_anode_hit - y_Lorentz_drift_hit;
    // Double_t impact_angle_rec = TMath::ATan2(Delta_y_Lorentz_drift_hit,Delta_x_Lorentz_drift_hit);

    // float calibrationShift = TMath::Tan(impact_angle_rec) * driftDepth;


    // GLOBAL TRANSFORM

    // auto toRad = [] (float d) {return (d/360) * (2*M_PI);};
    // //calculate roation amount based on sector number
    // float theta = toRad(sector*20 + 10);

    // float global_X = local_X*cos(theta) - local_Y*sin(theta);
    // float global_Y = local_X*sin(theta) + local_Y*cos(theta);
  }
  // h1d_position_can->cd();
  // h1d_position->Draw();
  // h1d_position_can->Print("hist.png");
  pc.outputs().snapshot(Output{"TRD", "CALIBRATEDTRACKLETS", 0, Lifetime::Timeframe}, calibratedTracklets);
}


o2::framework::DataProcessorSpec o2::trd::getTRDTrackletCalibratorSpec()
{
  LOG(info) << "getting TRDTrackletCalibratorSpec";
  return DataProcessorSpec{
    "TRDTRACKLETCALIBRATOR",
    Inputs{InputSpec{"inTracklets", "TRD", "TRACKLETS", 0},
          //  InputSpec{"triggerRecord", "TRD", "TRKTRGRD", 0}
          //  InputSpec{"triggerRecord", "TRD", "TRIGGERREC", 0}
           },
    Outputs{OutputSpec{"TRD", "CALIBRATEDTRACKLETS", 0, Lifetime::Timeframe}},    
    // Inputs{},
    // Outputs{},
    AlgorithmSpec{adaptFromTask<TRDTrackletCalibratorSpec>()},
    Options{}};
}

// } //end namespace trd
// } //end namespace o2
