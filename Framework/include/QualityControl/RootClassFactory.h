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
//

///
/// \author Piotr Konopka
/// \file RootClassFactory.h
///
#ifndef QUALITYCONTROL_ROOTCLASSFACTORY_H
#define QUALITYCONTROL_ROOTCLASSFACTORY_H

#include <string>
// O2
#include <Common/Exceptions.h>
// ROOT
#include <TClass.h>
#include <TROOT.h>
#include <TSystem.h>
// Boost
#include <boost/filesystem/path.hpp>
// QC
#include "QualityControl/QcInfoLogger.h"

namespace bfs = boost::filesystem;

namespace o2::quality_control::core
{

namespace root_class_factory
{

using FatalException = AliceO2::Common::FatalException;
using errinfo_details = AliceO2::Common::errinfo_details;

void loadLibrary(const std::string& moduleName);

template <typename T>
T* create(const std::string& moduleName, const std::string& className)
{
  T* result = nullptr;

  loadLibrary(moduleName);

  // Get the class and instantiate
  ILOG(Info, Devel) << "Loading class " << className << ENDM;
  TClass* cl = TClass::GetClass(className.c_str());
  std::string tempString("Failed to instantiate Quality Control Module");
  if (!cl) {
    tempString += " because no dictionary for class named \"";
    tempString += className;
    tempString += "\" could be retrieved";
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(tempString));
  }
  ILOG(Info, Devel) << "Instantiating class " << className << " (" << cl << ")" << ENDM;
  result = static_cast<T*>(cl->New());
  if (!result) {
    BOOST_THROW_EXCEPTION(FatalException() << errinfo_details(tempString));
  }
  ILOG(Info, Devel) << "QualityControl Module " << moduleName << " loaded " << ENDM;

  return result;
}

} // namespace root_class_factory

} // namespace o2::quality_control::core

#endif // QUALITYCONTROL_ROOTCLASSFACTORY_H
