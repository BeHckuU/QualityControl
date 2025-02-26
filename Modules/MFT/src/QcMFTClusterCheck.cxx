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
/// \file   QcMFTClusterCheck.cxx
/// \author Tomas Herman
/// \author Guillermo Contreras
/// \author Katarina Krizkova Gajdosova
/// \author Diana Maria Krupova

// Fair
#include <fairlogger/Logger.h>
// ROOT
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TList.h>
// Quality Control
#include "MFT/QcMFTClusterCheck.h"
#include "QualityControl/MonitorObject.h"
#include "QualityControl/Quality.h"

using namespace std;

namespace o2::quality_control_modules::mft
{

void QcMFTClusterCheck::configure() {}

Quality QcMFTClusterCheck::check(std::map<std::string, std::shared_ptr<MonitorObject>>* moMap)
{
  Quality result = Quality::Null;

  for (auto& [moName, mo] : *moMap) {

    (void)moName;

    if (mo->getName() == "mClusterOccupancy") {
      auto* hChipOccupancy = dynamic_cast<TH1F*>(mo->getObject());

      float den = hChipOccupancy->GetBinContent(0); // normalisation stored in the uderflow bin

      for (int iBin = 0; iBin < hChipOccupancy->GetNbinsX(); iBin++) {
        if (hChipOccupancy->GetBinContent(iBin + 1) == 0) {
          hChipOccupancy->Fill(937); // number of chips with zero clusters stored in the overflow bin
        }
        float num = hChipOccupancy->GetBinContent(iBin + 1);
        float ratio = (den > 0) ? (num / den) : 0.0;
        hChipOccupancy->SetBinContent(iBin + 1, ratio);
      }
    }

    if (mo->getName() == "mClusterPatternIndex") {
      auto* hChipPattern = dynamic_cast<TH1F*>(mo->getObject());

      float den = hChipPattern->GetBinContent(0); // normalisation stored in the uderflow bin

      for (int iBin = 0; iBin < hChipPattern->GetNbinsX(); iBin++) {
        float num = hChipPattern->GetBinContent(iBin + 1);
        float ratio = (den > 0) ? (num / den) : 0.0;
        hChipPattern->SetBinContent(iBin + 1, ratio);
      }
    }

    if (mo->getName() == "mClusterOccupancySummary") {
      auto* histogram = dynamic_cast<TH2F*>(mo->getObject());

      float den = histogram->GetBinContent(0, 0); // normalisation stored in the uderflow bin

      for (int iBinX = 0; iBinX < histogram->GetNbinsX(); iBinX++) {
        for (int iBinY = 0; iBinY < histogram->GetNbinsY(); iBinY++) {
          float num = histogram->GetBinContent(iBinX + 1, iBinY + 1);
          float ratio = (den > 0) ? (num / den) : 0.0;
          histogram->SetBinContent(iBinX + 1, iBinY + 1, ratio);
        }
      }
    }
  }
  return result;
}

std::string QcMFTClusterCheck::getAcceptedType() { return "TH1"; }

void QcMFTClusterCheck::beautify(std::shared_ptr<MonitorObject> mo, Quality checkResult)
{
  if (mo->getName() == "mClusterOccupancy") {
    auto* histogram = dynamic_cast<TH1F*>(mo->getObject());

    if (checkResult == Quality::Good) {
      LOG(info) << "Quality::Good";
      histogram->SetLineColor(kGreen + 2);
      TLatex* tl = new TLatex(350, 1.05 * histogram->GetMaximum(), "#color[418]{Dummy check status: Good!}");
      histogram->GetListOfFunctions()->Add(tl);
      tl->Draw();
    } else if (checkResult == Quality::Bad) {
      LOG(info) << "Quality::Bad";
      histogram->SetLineColor(kRed + 1);
      TLatex* tl = new TLatex(350, 1.05 * histogram->GetMaximum(), "#color[633]{Dummy check status: Bad!}");
      histogram->GetListOfFunctions()->Add(tl);
      tl->Draw();
    } else if (checkResult == Quality::Medium) {
      LOG(info) << "Quality::Medium";
      histogram->SetLineColor(kOrange);
      TLatex* tl = new TLatex(350, 1.05 * histogram->GetMaximum(), "#color[800]{Dummy check status: Medium!}");
      histogram->GetListOfFunctions()->Add(tl);
      tl->Draw();
    }
  }
}

} // namespace o2::quality_control_modules::mft
