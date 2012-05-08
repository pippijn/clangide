#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <clang/AST/ASTConsumer.h>

struct PPContext;

struct ASTPrinter
  : clang::ASTConsumer
{
  ASTPrinter (PPContext &ctx);
  ~ASTPrinter ();

  void HandleTranslationUnit (clang::ASTContext &context);

  PPContext &ctx;
};

#endif /* ASTPRINTER_H */
