#include "ppcontext.h"
#include "qdiagnosticclient.h"

#include <llvm/Support/Host.h>

#include <clang/Frontend/FrontendOptions.h>
#include <clang/Frontend/HeaderSearchOptions.h>
#include <clang/Frontend/PreprocessorOptions.h>
#include <clang/Frontend/Utils.h>

static clang::TargetOptions createTargetOptions ()
{
  clang::TargetOptions options;
  options.Triple = LLVM_HOSTTRIPLE;
  return options;
}

static clang::LangOptions createLangOptions ()
{
  clang::LangOptions options;
  options.Bool = true;
  options.GNUKeywords = true;
  options.CPlusPlus = true;
  return options;
}

PPContext::PPContext (QDiagnosticClient &diagClient)
  : diagClient (diagClient)
  , diagIds (new clang::DiagnosticIDs)
  , diags (diagIds, &diagClient, false)
  , targetOptions (createTargetOptions ())
  , target (clang::TargetInfo::CreateTargetInfo (diags, targetOptions))
  , fm (fsOptions)
  , sm (diags, fm)
  , headers (fm)
  , opts (createLangOptions ())
  , pp (diags, opts, *target, sm, headers)
  , astCtx (opts, sm, *target, pp.getIdentifierTable (), pp.getSelectorTable (), pp.getBuiltinInfo (), 10000)
{
  pp.getBuiltinInfo ().InitializeBuiltins (pp.getIdentifierTable (), opts);

  clang::PreprocessorOptions ppo;

  // Add header search directories
  clang::HeaderSearchOptions hso;
  hso.AddPath (".", clang::frontend::Quoted, true, false, false);
  hso.AddPath ("/usr/include", clang::frontend::System, true, false, false);
  hso.AddPath ("/usr/lib/clang/3.0/include", clang::frontend::System, true, false, false);

  hso.AddPath ("/opt/QtSDK/Desktop/Qt/473/gcc/mkspecs/linux-g++-64", clang::frontend::Angled, true, false, false);
  hso.AddPath ("/opt/QtSDK/Desktop/Qt/473/gcc/include", clang::frontend::Angled, true, false, false);
  hso.AddPath ("/opt/QtSDK/Desktop/Qt/473/gcc/include/QtGui", clang::frontend::Angled, true, false, false);
  hso.AddPath ("/opt/QtSDK/Desktop/Qt/473/gcc/include/QtCore", clang::frontend::Angled, true, false, false);

  clang::FrontendOptions feo;

  InitializePreprocessor (pp, ppo, hso, feo);
}

PPContext::~PPContext ()
{
  delete target;
}
