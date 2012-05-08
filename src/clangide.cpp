#include "clangide.h"
#include "ui_clangide.h"

#include "astprinter.h"
#include "ppcontext.h"

#include <clang/Parse/ParseAST.h>

#include <QDebug>
#include <QFile>

using namespace clang;
using namespace llvm;

static QString
slurp (char const *filename)
{
  QFile file (filename);
  file.open (QIODevice::ReadOnly);
  uchar *mem = file.map (0, file.size ());
  QString data (QByteArray ((char const *)mem, file.size ()));
  file.unmap (mem);
  return data;
}

static QString const css = "<style>" + slurp ("../clangide/codeEdit.css") + "</style>";

ClangIDE::ClangIDE (QWidget *parent)
  : QMainWindow (parent)
  , ui (new Ui::ClangIDE)
  , modifying (false)
  , diagClient (diagOpts)
  , parser (diagClient)
  , diagModel (&diagModels[0])
{
  ui->setupUi (this);
  ui->codeEdit->setHtml (css);

  parseTimer.setSingleShot (true);

  connect (&diagClient, SIGNAL (diagnostic (uint, uint, uint, QString)), SLOT (on_diagnostic (uint, uint, uint, QString)));
  connect (&parser, SIGNAL (done (bool)), SLOT (on_parsed (bool)));
  connect (&parser, SIGNAL (highlighted (QString const &)), SLOT (setEditHtml (QString const &)));
  connect (&parseTimer, SIGNAL (timeout ()), SLOT (start_parsing ()));
}

ClangIDE::~ClangIDE ()
{
  delete ui;
}

void
ClangIDE::setEditPosition (int position)
{
  QTextCursor cursor = ui->codeEdit->textCursor ();
  cursor.setPosition (position);
  ui->codeEdit->setTextCursor (cursor);
}

void
ClangIDE::setEditHtml (QString const &html)
{
  modifying = true;
  int position = ui->codeEdit->textCursor ().position ();
  ui->codeEdit->setHtml (css + "<body class='code'>\n" + html + "\n</body>");
  setEditPosition (position);
  modifying = false;
}

void
ClangIDE::on_codeEdit_textChanged ()
{
  if (modifying)
    return;

  QString const &code = ui->codeEdit->toPlainText ();
  if (code.isEmpty ())
    {
      diagModels[0].clear ();
      diagModels[1].clear ();
      return;
    }

  QString html = parser.highlight (code);

  setEditHtml (html);

  parseTimer.start (500);
}

void
ClangIDE::on_codeEdit_cursorPositionChanged ()
{
  if (parseTimer.isActive ())
    parseTimer.start (500);
}

void
ClangIDE::start_parsing ()
{
  parser.load (ui->codeEdit->toPlainText ());
}

void
ClangIDE::on_diagnostic (uint pos, uint lineno, uint column, QString msg)
{
  QList<QStandardItem *> row;
  row.append (new QStandardItem (QString ("%1:%2").arg (lineno).arg (column)));
  row.append (new QStandardItem (msg));
  row[0]->setData (pos);
  diagModel->appendRow (row);
}

void
ClangIDE::on_diagnostics_doubleClicked (const QModelIndex &index)
{
  QStandardItemModel *model = qobject_cast<QStandardItemModel *> (ui->diagnostics->model ());
  QStandardItem *item = model->item (index.row ());

  setEditPosition (qvariant_cast<uint> (item->data ()));
  ui->codeEdit->setFocus (Qt::OtherFocusReason);
}

void
ClangIDE::on_parsed (bool errors)
{
  QStringList errorLabels;
  errorLabels.append ("Location");
  errorLabels.append ("Message");

  diagModel->setHorizontalHeaderLabels (errorLabels);

  ui->diagnostics->setModel (diagModel);

  if (diagModel == &diagModels[0])
    diagModel = &diagModels[1];
  else
    diagModel = &diagModels[0];

  diagModel->clear ();
}
