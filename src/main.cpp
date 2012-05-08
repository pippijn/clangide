#define __STDC_LIMIT_MACROS 1
#define __STDC_CONSTANT_MACROS 1

#include "clangide.h"

#include <QApplication>
#include <QTextCodec>

int
main (int argc, char *argv[])
{
#if 0
  extern void parse ();
  parse ();
  return 0;
#endif

  QTextCodec::setCodecForCStrings (QTextCodec::codecForLocale ());

  QApplication a (argc, argv);
  ClangIDE w;

  w.show ();

  return a.exec ();
}
