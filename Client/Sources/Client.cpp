#include "Client.h"

Client::Client(QWidget* parent) : QWidget(parent), dataSize(0), userName("")
{
	glayout = new QGridLayout(this);
	connectBtn = new QPushButton("Connect", this);
	authBtn = new QPushButton("Sign in", this);
	loginBtn = new QPushButton("Log in", this);
	labelUserName = new QLabel("Username:\tneed to authorizate", this);
	textBrowser = new QTextBrowser(this);
	inputLine = new QLineEdit(this);
	sendBtn = new QPushButton("Send", this);

	labelUserName->setStyleSheet("background-color: #dddddd; font-weight: bold;");

	ConnectLine = vector<QPushButton*>();
	ConnectLine.push_back(new QPushButton(this));
	ConnectLine.push_back(connectBtn);
	ConnectLine.push_back(new QPushButton(this));

	setState(ToolsState::UNCONNECTED);
	is_logined = false;

	int _line_ind {0};

	glayout->addWidget(labelUserName, _line_ind, 0, 1, 6);
	glayout->addWidget(authBtn, ++_line_ind, 0, 1, 3);
	glayout->addWidget(loginBtn, _line_ind, 3, 1, 3);
	glayout->addWidget(ConnectLine[0], ++_line_ind, 0, 1, 1);
	glayout->addWidget(ConnectLine[1], _line_ind, 1, 1, 4);
	glayout->addWidget(ConnectLine[2], _line_ind, 5, 1, 1);
	glayout->addWidget(textBrowser, ++_line_ind, 0, 1, 6);
	glayout->addWidget(inputLine, ++_line_ind, 0, 1, 6);
	glayout->addWidget(sendBtn, ++_line_ind, 0, 1, 6);

	currSocket = new QTcpSocket(this);

	connect(connectBtn, SIGNAL(clicked()), this, SLOT(slot_connect_btn_clicked()));
	connect(sendBtn, SIGNAL(clicked()), this, SLOT(slot_send_btn_clicked()));
	connect(currSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(currSocket, SIGNAL(disconnected()), currSocket, SLOT(deleteLater()));
	connect(authBtn, SIGNAL(clicked()), this, SLOT(slot_auth_btn_clicked()));
	connect(loginBtn, SIGNAL(clicked()), this, SLOT(slot_login_btn_clicked()));
}

Client::~Client()
{ }

void Client::sendOKcode()
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);
	out << OK_CODE;
	currSocket->write(Data);
}

void Client::setState(ToolsState State)
{
	QString sheet;
	switch(State) {
		case ToolsState::UNCONNECTED:
			sheet = QString("background-color: #ff0000");
			authBtn->setEnabled(false);
			loginBtn->setEnabled(false);
			sendBtn->setEnabled(false);
			break;
		case ToolsState::CONNECTED:
			sheet = QString("background-color: #00ff00");
			authBtn->setEnabled(true);
			loginBtn->setEnabled(true);
			sendBtn->setEnabled(false);
			break;
		case ToolsState::LOGINED:
			sheet = QString("background-color: #00ff00");
			authBtn->setEnabled(true);
			loginBtn->setEnabled(true);
			sendBtn->setEnabled(true);
			break;
	};
	ConnectLine[0]->setStyleSheet(sheet);
	ConnectLine[2]->setStyleSheet(sheet);
	is_connected = (State == ToolsState::UNCONNECTED) ? false : true;
}

void Client::slot_connect_btn_clicked()
{
	if(is_connected) return;
	currSocket->connectToHost("127.0.0.1", 55555);
}

void Client::slot_send_btn_clicked()
{
	if(!is_connected) return;

	Message msg;
	QDateTime currDt = QDateTime::currentDateTime();

	msg.time = currDt.time().toString();
	msg.userName = userName;
	msg.text = inputLine->text();

	SendToServer(msg);		
	inputLine->setText("");
}

void Client::slotReadyRead()
{
	QDataStream in(currSocket);

	if(currSocket->bytesAvailable() == 1) {
		// server sent a code
		quint8 code; in >> code;
		if(code == OK_CODE) {
			setState(ToolsState::CONNECTED);
		} else if(code == OK_AUTH_CODE) {
			userName = _possibleUserName;
			labelUserName->setText("User:\t" + userName);
			setState(ToolsState::LOGINED);
			is_logined = true;
			QMessageBox::information(this, "Notification",
				QString("signed in successfully"));
		} else if(code == WRONG_AUTH_CODE) {
			QMessageBox::warning(this, "Error",
				QString("Invalid username or password. Try again"));
		} else if(code == OK_LOGIN_CODE) {
			QMessageBox::information(this, "Notification",
				QString("logined in successfully"));
		} else if(code == WRONG_LOGIN_CODE) {
			QMessageBox::warning(this, "Error",
				QString("The same user also exists"));
		}
		return;
	}

	if(in.status() == QDataStream::Ok) {
		cout << "reading data..." << endl;		

		for(;;) {
			if(dataSize == 0) {
				if(currSocket->bytesAvailable() < 2) break;
				in >> dataSize;	
			}
			if(currSocket->bytesAvailable() < dataSize) break;
			Message msg;
			in >> msg.time >> msg.userName >> msg.text;
			dataSize = 0;
			textBrowser->append(msg.formated());
		}
		
		cout << "data reading finished" << endl;
	}
	else cout << "data reading error..." << endl;
}


void Client::SendToServer(Message msg)
{
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);
	out << quint16(0) << MESSAGE_CODE << msg.time << msg.userName << msg.text;

	out.device()->seek(0);
	out << quint16(Data.size() - sizeof(quint16));

	currSocket->write(Data);
}

void Client::slot_login_btn_clicked()
{
	if(is_logined) return;

	QString name = QInputDialog::getText(this, "Input", "Name: ");

	if(!checkName(name)) {
		QMessageBox::warning(this, "Error", QString("Invalid username. Try again"));
		return;
	}

	QString passw = QInputDialog::getText(this, "Input", "New password: ");
	std::string _passw = passw.toUtf8().constData();
	size_t hsh = std::hash<std::string>{}(_passw);
	char chsh[128]; sprintf(chsh, "%lld", (long long)hsh);
	QString shsh(chsh);

	if(!checkPassw(passw)) {
		QMessageBox::warning(this, "Error", QString("Invalid password. Try again"));
		return;
	}

	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);
	out << quint16(0) << LOGIN_CODE << name << QString(shsh);
	out.device()->seek(0);
	out << quint16(Data.size() - sizeof(quint16));

	currSocket->write(Data);
}

void Client::slot_auth_btn_clicked()
{
	if(is_logined) return;

	QString name = QInputDialog::getText(this, "Input", "Name: ");

	if(!checkName(name)) {
		QMessageBox::warning(this, "Error", QString("Invalid username. Try again"));
		return;
	}

	QString passw = QInputDialog::getText(this, "Input", "Password: ");
	std::string _passw = passw.toUtf8().constData();
	size_t hsh = std::hash<std::string>{}(_passw);
	char chsh[128]; sprintf(chsh, "%lld", (long long)hsh);
	QString shsh(chsh);

	if(!checkPassw(passw)) {
		QMessageBox::warning(this, "Error", QString("Invalid password. Try again"));
		return;
	}

	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);
	out << quint16(0) << AUTH_CODE << name << QString(shsh);
	out.device()->seek(0);
	out << quint16(Data.size() - sizeof(quint16));

	currSocket->write(Data);

	_possibleUserName = name;
}

bool Client::checkName(QString name)
{ return name != QString(""); }

bool Client::checkPassw(QString passw)
{ return true; }

