#include "qdiagnosticclient.h"

#include <llvm/ADT/SmallString.h>

#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>

#include <clang/Frontend/DiagnosticOptions.h>

#include <QDebug>

using namespace clang;

QDiagnosticClient::QDiagnosticClient (DiagnosticOptions const &opts, QObject *parent)
  : QObject (parent)
  , opts (opts)
{
}

QDiagnosticClient::~QDiagnosticClient ()
{
}

void
QDiagnosticClient::HandleDiagnostic (Diagnostic::Level DiagLevel, DiagnosticInfo const &Info)
{
  DiagnosticClient::HandleDiagnostic (DiagLevel, Info);

  SourceLocation const &loc = Info.getLocation ();
  if (loc.isValid ())
    {
      SourceManager const &sm = Info.getSourceManager ();

      FileID FID = sm.getFileID (loc);
      if (FID == sm.getMainFileID ())
        {
          uint offset = sm.getFileOffset (loc);

          llvm::SmallString<100> str;
          Info.FormatDiagnostic (str);

          emit diagnostic (offset,
                           sm.getLineNumber (FID, offset),
                           sm.getColumnNumber (FID, offset),
                           str.c_str ());
        }
    }
}
