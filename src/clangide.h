#ifndef CLANGIDE_H
#define CLANGIDE_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTimer>

namespace Ui
{
  class ClangIDE;
}

#include "clangparser.h"
#include "qdiagnosticclient.h"
struct PPContext;

#include <clang/Frontend/DiagnosticOptions.h>

class ClangIDE
  : public QMainWindow
{
  Q_OBJECT

public:
  explicit ClangIDE (QWidget *parent = 0);
  ~ClangIDE ();

private:
  Ui::ClangIDE *ui;
  bool modifying;
  clang::DiagnosticOptions diagOpts;
  QDiagnosticClient diagClient;
  ClangParser parser;
  QStandardItemModel diagModels[2];
  QStandardItemModel *diagModel;
  QTimer parseTimer;

private slots:
  void setEditPosition (int position);
  void setEditHtml (QString const &html);
  void on_codeEdit_textChanged ();
  void on_codeEdit_cursorPositionChanged();
  void start_parsing ();
  void on_diagnostic (uint pos, uint lineno, uint column, QString msg);
  void on_diagnostics_doubleClicked (const QModelIndex &index);
  void on_parsed (bool errors);
};

#endif /* CLANGIDE_H */
