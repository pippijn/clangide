#ifndef CLANGPARSER_H
#define CLANGPARSER_H

#include <QMutex>
#include <QThread>
class QDiagnosticClient;

class PPContext;

class ClangParser
  : public QThread
{
  Q_OBJECT

public:
  explicit ClangParser (QDiagnosticClient &diagClient, QObject *parent = 0);
  ~ClangParser ();

  void run ();
  void load (QString const &code);
  QString highlight (QString const &code);

signals:
  void done (bool errors);
  void highlighted (QString const &html);

private:
  void parse ();
  QString highlight (PPContext &ctx);

  QDiagnosticClient &diagClient;
  QMutex mtx;
  QString code;
  volatile bool running;
};

#endif /* CLANGPARSER_H */
