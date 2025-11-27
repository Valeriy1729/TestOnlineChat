#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <iostream>

using std::cout;
using std::endl;


class Client : public QWidget {
	Q_OBJECT

	QTcpSocket* currSocket;
	QByteArray Data;

	QVBoxLayout* vlayout;
	QTextBrowser* textBrowser;
	QPushButton* connectBtn;
	QLineEdit* inputLine;
	QPushButton* sendBtn;

	void SendToServer(QString msg);

public:
	Client(QWidget* parent=nullptr);
	~Client();	

public slots:
	void slot_send_btn_clicked();
	void slot_connect_btn_clicked();
	void slotReadyRead();
};


#endif
