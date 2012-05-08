#include "clangparser.h"
#include "astprinter.h"
#include "ppcontext.h"

#include <llvm/Support/MemoryBuffer.h>

#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>

#include <clang/AST/ASTContext.h>

#include <clang/Parse/ParseAST.h>

#include <clang/Rewrite/HTMLRewrite.h>
#include <clang/Rewrite/Rewriter.h>

#include <QDebug>
#include <QRegExp>

using namespace clang;

ClangParser::ClangParser (QDiagnosticClient &diagClient, QObject *parent)
  : QThread (parent)
  , diagClient (diagClient)
  , running (true)
{
  start ();
}

ClangParser::~ClangParser ()
{
  running = false;
  mtx.unlock ();
  wait ();
}

void
ClangParser::run ()
{
  while (running)
    {
      mtx.lock ();
      parse ();
    }
}

void
ClangParser::load (QString const &code)
{
  this->code = code;
  mtx.unlock ();
}

void
ClangParser::parse ()
{
  if (!running || code.isEmpty ())
    return;

  QString code;
  std::swap (code, this->code);
  QByteArray const &incode = code.toUtf8 ();

  // Initialise parsing context
  PPContext ctx (diagClient);

  // Add input file
  llvm::MemoryBuffer *buffer = llvm::MemoryBuffer::getMemBuffer (incode.data (), "<code>");

  ctx.sm.createMainFileIDForMemBuffer (buffer);

  // Parse it
  ASTPrinter prn (ctx);

  ctx.diagClient.BeginSourceFile (ctx.opts, &ctx.pp);
  ParseAST (ctx.pp, &prn, ctx.astCtx);
  ctx.diagClient.EndSourceFile ();

  emit done (ctx.diags.hasErrorOccurred ());
  if (this->code.isEmpty ())
    emit highlighted (highlight (ctx));
}

QString
ClangParser::highlight (PPContext &ctx)
{
  // Lex it and generate html highlighting
  Rewriter R (ctx.sm, ctx.opts);
  FileID FID = ctx.sm.getMainFileID ();

  html::SyntaxHighlight (R, FID, ctx.pp);
  html::HighlightMacros (R, FID, ctx.pp);
  html::EscapeText (R, FID, false, false);

  RewriteBuffer const &rewbuf = R.getEditBuffer (FID);
  QRegExp re ("<span class='expansion'>.*</span>");
  re.setMinimal (true);
  return QString::fromStdString (std::string (rewbuf.begin (), rewbuf.end ())).remove (re);
}

QString
ClangParser::highlight (QString const &code)
{
  if (code.isEmpty ())
    return code;

  // Initialise parsing context
  PPContext ctx (diagClient);

  QByteArray incode = code.toUtf8 ();

  // Add input file
  llvm::MemoryBuffer *buffer = llvm::MemoryBuffer::getMemBuffer (incode.data (), "<code>");

  ctx.sm.createMainFileIDForMemBuffer (buffer);

  return highlight (ctx);
}
