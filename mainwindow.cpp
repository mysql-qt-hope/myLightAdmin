#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Status->setText("");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName(ui->Database->text());
    db.setHostName(ui->HostName->text());
    db.setUserName(ui->User->text());
    db.setPassword(ui->Password->text());
    if (db.isOpen()){
        db.close();
    }
     bool ok = db.open();
     if (ok){
         QPalette ok_palette;
         QSqlQuery query;
         QString outHtml;
         ok_palette.setColor(QPalette::WindowText, Qt::darkGreen);

         ui->Status->setAutoFillBackground(true);
         ui->Status->setPalette(ok_palette);
         ui->Status->setText("Connected");

         query.prepare("SHOW TABLES");
         query.exec();
         outHtml.append("<center><h3>Tables</h3>\r\n");
         outHtml.append("<table border=1>");
         while (query.next()){
             QSqlQuery tquery;
             QString tableName = query.value(0).toString();
             QString query_str = QString("SHOW COLUMNS FROM ");

             outHtml.append("<tr><th colspan=6>");
             outHtml.append(tableName);
             outHtml.append("</th></tr>\r\n");

//             if (!QString::compare(tableName,QString("answertypes"))){
//                 tableName = "answertipes";
//             }
             query_str.append(tableName);
             tquery.prepare(query_str);
             tquery.exec();
             while (tquery.next()){
                 outHtml.append("<tr><td>");
                 outHtml.append(tquery.value(0).toString());
                 outHtml.append("</td><td>");
                 outHtml.append(tquery.value(1).toString());
                 outHtml.append("</td><td>");
                 outHtml.append(tquery.value(2).toString());
                 outHtml.append("</td><td>");
                 outHtml.append(tquery.value(3).toString());
                 outHtml.append("</td><td>");
                 outHtml.append(tquery.value(4).toString());
                 outHtml.append("</td><td>");
                 outHtml.append(tquery.value(5).toString());
                 outHtml.append("</td></tr>\r\n");
             }
             outHtml.append("<tr><td colspan=6><p style=\"color:red\">");
             outHtml.append(tquery.lastError().text());
             outHtml.append("</p></td></tr>\r\n");
         }
         outHtml.append("</table></center>");
         ui->textEdit->setHtml(outHtml);
     } else {
         QSqlError err = db.lastError();
         QString error = err.text();
         QMessageBox message;

         QPalette err_palette;
         err_palette.setColor(QPalette::WindowText, Qt::darkRed);

         ui->Status->setAutoFillBackground(true);
         ui->Status->setPalette(err_palette);
         ui->Status->setText("ERROR");

         message.setText(error);
         message.exec();

         db.close();
     }
}
void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    QString outHtml;
    int ColumnCount = 10;
    QString ColumnCountStr = ui->ColumnCount->text();

    ColumnCount = ColumnCountStr.toInt();
    query.prepare(ui->SQLText->toPlainText());
    query.exec();
    outHtml.append("<center><table border=1>");
    while (query.next()){
        outHtml.append("<tr>");
        for (int i = 0; i < ColumnCount; i++){
            QString val = query.value(i).toString();
            outHtml.append("<td>");
            outHtml.append(val);
            outHtml.append("</td>");
        }
        outHtml.append("</tr>");
    }
    outHtml.append("</table></center>");
    ui->textEdit->setHtml(outHtml);
    if (query.lastError().text().size() > 5){
        ui->textEdit->setHtml(query.lastError().text());
    }
}
