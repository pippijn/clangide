#include "astprinter.h"
#include "ppcontext.h"

#include <clang/AST/ASTContext.h>


using namespace clang;

ASTPrinter::ASTPrinter (PPContext &ctx)
  : ctx (ctx)
{
}

ASTPrinter::~ASTPrinter ()
{
}

void
ASTPrinter::HandleTranslationUnit (clang::ASTContext &context)
{
}
