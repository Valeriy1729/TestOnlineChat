#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QColor>
#include <QTextBrowser>
#include <QGridLayout>
#include <QDateTime>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <iostream>
#include <vector>
#include <functional>
#include "msg.h"

#define OK_CODE 		quint8(111)
#define WRONG_AUTH_CODE		quint8(31)
#define OK_AUTH_CODE		quint8(17)
#define OK_LOGIN_CODE		quint8(16)
#define WRONG_LOGIN_CODE	quint8(15)

#define LOGIN_CODE 		quint16(65535)
#define AUTH_CODE 		quint16(65533)
#define MESSAGE_CODE 		quint16(65534)

using std::cout;
using std::endl;
using std::vector;

enum class ToolsState { CONNECTED, LOGINED, UNCONNECTED};


class Client : public QWidget {
	Q_OBJECT

	bool is_connected;
	bool is_logined;
	QTcpSocket* currSocket;
	QByteArray Data;
	quint16 dataSize;

	QGridLayout* glayout;
	vector<QPushButton*> ConnectLine;
	QPushButton* connectBtn;
	QPushButton* authBtn;
	QPushButton* loginBtn;
	QLabel* labelUserName;
	QTextBrowser* textBrowser;
	QLineEdit* inputLine;
	QPushButton* sendBtn;

	QString userName;
	QString _possibleUserName;

	void SendToServer(Message msg);
	void setState(ToolsState State);
	void sendOKcode();
	bool checkName(QString name);
	bool checkPassw(QString passw);

public:
	Client(QWidget* parent=nullptr);
	~Client();	

public slots:
	void slot_send_btn_clicked();
	void slot_connect_btn_clicked();
	void slot_auth_btn_clicked();
	void slot_login_btn_clicked();
	void slotReadyRead();
};


#endif
