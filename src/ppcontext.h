#ifndef PPCONTEXT_H
#define PPCONTEXT_H

#include "qdiagnosticclient.h"

#include <string>

#include <llvm/Config/config.h>
#include <llvm/Support/raw_ostream.h>

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/FileSystemOptions.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/TargetOptions.h>

#include <clang/Lex/HeaderSearch.h>
#include <clang/Lex/Preprocessor.h>

#include <clang/AST/ASTContext.h>

class QDiagnosticClient;

struct PPContext
{
  PPContext (QDiagnosticClient &diagClient);
  ~PPContext ();

  QDiagnosticClient &diagClient;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagIds;
  clang::Diagnostic diags;
  clang::TargetOptions targetOptions;
  clang::TargetInfo *target;
  clang::FileSystemOptions fsOptions;
  clang::FileManager fm;
  clang::SourceManager sm;
  clang::HeaderSearch headers;
  clang::LangOptions opts;
  clang::Preprocessor pp;
  clang::ASTContext astCtx;
};

#endif /* PPCONTEXT_H */
