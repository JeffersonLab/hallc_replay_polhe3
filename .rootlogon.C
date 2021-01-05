#include <fmt/core.h>

void _rootlogon() {
  // ensure we have format loaded. Should be able to move to C++20 fmt library
  // some point in the future
  R__LOAD_LIBRARY(libfmt);

  // check if we requested a special build directory through the environment,
  const char*       build_dir_from_env = gSystem->Getenv("ROOT_BUILD_TMPDIR");
  const std::string build_dir = build_dir_from_env ? build_dir_from_env : "$HOME/.root_build_dir";

  // ensure the build directory exists to avoid crashes due a directory
  // note: in principle this allows us to execute arbitrary code, but seeing
  //       as this can only be manipulated through the shell environment in the
  //       first place this should not be a safety concern.
  const std::string mkdir = fmt::format("mkdir -p {}", build_dir);
  gSystem->Exec(mkdir.c_str());

  // Communicate what build directory we are using
  fmt::print("---> Setting ACLiC build directory to: {}\n\n", build_dir);
  gSystem->SetBuildDir(build_dir.c_str());

  // Ensure we have our environment setup
  gROOT->ProcessLine(".include /usr/local/include/hcana");
  gROOT->ProcessLine(".include SCRIPTS/include");
  gROOT->ProcessLine(".include .");

  // Some basic plotting setup
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);

  // Load extra ROOT libraries, TODO check if this is still
  // needed in the newer ROOT versions we are using.
  gSystem->Load("libTree");
  gSystem->Load("libTreePlayer");
  gSystem->Load("libHist");
}
