#ifndef QDIAGNOSTICCLIENT_H
#define QDIAGNOSTICCLIENT_H

#include <clang/Basic/Diagnostic.h>
namespace clang
{
  struct DiagnosticOptions;
}

#include <QObject>

class QDiagnosticClient
  : public QObject
  , public clang::DiagnosticClient
{
  Q_OBJECT

public:
  QDiagnosticClient (clang::DiagnosticOptions const &opts, QObject *parent = 0);
  ~QDiagnosticClient ();

private:
  void HandleDiagnostic (clang::Diagnostic::Level DiagLevel, clang::DiagnosticInfo const &Info);

  clang::DiagnosticOptions const &opts;

signals:
  void diagnostic (uint pos, uint lineno, uint column, QString msg);
};

#endif /* QDIAGNOSTICCLIENT_H */
