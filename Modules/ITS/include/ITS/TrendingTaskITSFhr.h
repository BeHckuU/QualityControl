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

///
/// \file    TrendingTaskITSFhr.h
/// \author  Ivan Ravasenga on the structure from Piotr Konopka
///

#ifndef QUALITYCONTROL_TRENDINGTASKITSFHR_H
#define QUALITYCONTROL_TRENDINGTASKITSFHR_H

#include "ITS/TrendingTaskConfigITS.h"
#include "QualityControl/PostProcessingInterface.h"
#include "QualityControl/Reductor.h"

#include <TAxis.h>
#include <TColor.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TTree.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace o2::quality_control::repository
{
class DatabaseInterface;
}

namespace o2::quality_control::postprocessing
{

/// \brief  A post-processing task which trends values, stores them in a TTree
/// and produces plots.
///
/// A post-processing task which trends objects inside QC database (QCDB). It
/// extracts some values of one or multiple
/// objects using the Reductor classes, then stores them inside a TTree. One can
/// generate plots out the TTree - the
/// class exposes the TTree::Draw interface to the user. The TTree and plots are
/// stored in the QCDB. The class is
/// configured with configuration files, see Framework/postprocessing.json as an
/// example.
///
/// \author Ivan Ravasenga on the structure from Piotr Konopka
class TrendingTaskITSFhr : public PostProcessingInterface
{
 public:
  TrendingTaskITSFhr() = default;
  ~TrendingTaskITSFhr() override = default;

  void configure(std::string name,
                 const boost::property_tree::ptree& config) override;
  void initialize(Trigger, framework::ServiceRegistry&) override;
  void update(Trigger, framework::ServiceRegistry&) override;
  void finalize(Trigger, framework::ServiceRegistry&) override;

 private:
  // other functions (mainly style)
  void SetLegendStyle(TLegend* leg);
  void SetGraphStyle(TGraph* g, int col, int mkr);
  void SetGraphNameAndAxes(TGraph* g, std::string name, std::string title,
                           std::string xtitle, std::string ytitle, double ymin,
                           double ymax, std::vector<std::string> runlist);
  void PrepareLegend(TLegend* leg, int layer);

  struct MetaData {
    Int_t runNumber = 0;
  };

  void trendValues(const Trigger& t, repository::DatabaseInterface& qcdb);
  void storePlots(repository::DatabaseInterface& qcdb);
  void storeTrend(repository::DatabaseInterface& qcdb);

  TrendingTaskConfigITS mConfig;
  MetaData mMetaData;
  Int_t ntreeentries = 0;
  UInt_t mTime;
  std::vector<std::string> runlist;
  std::unique_ptr<TTree> mTrend;
  std::unordered_map<std::string, std::unique_ptr<Reductor>> mReductors;

  const int col[7] = { 1, 2, 3, 4, 5, 6, 7 };
  const int mkr[8] = { 20, 21, 22, 29, 24, 25, 26, 30 };
  static constexpr int NLAYERS = 7;
  static constexpr int NTRENDSFHR = 4;
  const int nStaves[NLAYERS] = { 12, 16, 20, 24, 30, 42, 48 };
  const std::string trendtitlesIB[NTRENDSFHR] = { "Fake-hit rate",
                                                  "Stddev Fake-hit rate", "Number of Active chips", "Occupancy" };
  const std::string trendtitlesOB[NTRENDSFHR] = { "Fake-hit rate",
                                                  "Stddev Fake-hit rate", "Number of Active lanes", "Occupancy" };
  const std::string trendnamesIB[NTRENDSFHR] = { "mean", "rms", "activechips", "occupancy" };
  const std::string trendnamesOB[NTRENDSFHR] = { "mean", "rms", "activelanes", "occupancy" };
  const std::string ytitlesIB[NTRENDSFHR] = {
    "Fake-hit rate (/event/pixel)", "Stddev Fake-hit rate (/event/pixel)", "# Active chips", "Occupancy (/event)"
  };
  const std::string ytitlesOB[NTRENDSFHR] = {
    "Fake-hit rate (/event/pixel)", "Stddev Fake-hit rate (/event/pixel)", "# Active lanes", "Occupancy (/event)"
  };
};

} // namespace o2::quality_control::postprocessing

#endif // QUALITYCONTROL_TRENDINGTASKITSFHR_H
