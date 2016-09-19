#include "USTPLoginDialog.h"
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <Windows.h>
#include "CTPMdApi.h"
#include "SgitMdApi.h"
#include "CTPTradeApi.h"
#include "SgitTradeApi.h"
#include "USTPConfig.h"
#include "USTPMutexId.h"
#include "USTPCtpLoader.h"
#include "USTPLogger.h"

USTPLoginDialog::USTPLoginDialog(QWidget* parent)
:QDialog(parent)
{	
	mUserABrokerId = USTPCtpLoader::getUserABrokerId();
	mUserBBrokerId = USTPCtpLoader::getUserBBrokerId();
	mAUserLabel = new QLabel(tr("CTP�û��˺�:"));
	mAPasswordLabel = new QLabel(tr("CTP��¼����:"));
	mAUserEdit = new QLineEdit(tr("055349"));
	mAPasswordEdit = new QLineEdit(tr("123456"));
	mAPasswordEdit->setEchoMode(QLineEdit::Password);

	mBUserLabel = new QLabel(tr("SGIT�û��˺�:"));
	mBPasswordLabel = new QLabel(tr("SGIT��¼����:"));
	mBUserEdit = new QLineEdit(tr("06000217"));
	mBPasswordEdit = new QLineEdit(tr("888888"));
	mBPasswordEdit->setEchoMode(QLineEdit::Password);
	
	mLoginBtn =  new QPushButton(tr("��  ¼"));
    mCancelBtn = new QPushButton(tr("ȡ  ��"));

    mBtnLayout = new QHBoxLayout;	
	mBtnLayout->addWidget(mLoginBtn);
	mBtnLayout->addSpacing(20);
	mBtnLayout->addWidget(mCancelBtn);

	mGridLayout = new QGridLayout;
	mGridLayout->addWidget(mAUserLabel, 0, 0, 1, 1);
	mGridLayout->addWidget(mAUserEdit, 0, 1, 1, 2);
	mGridLayout->addWidget(mAPasswordLabel, 1, 0, 1, 1);
	mGridLayout->addWidget(mAPasswordEdit, 1, 1, 1, 2);

	mGridLayout->addWidget(mBUserLabel, 2, 0, 1, 1);
	mGridLayout->addWidget(mBUserEdit, 2, 1, 1, 2);
	mGridLayout->addWidget(mBPasswordLabel, 3, 0, 1, 1);
	mGridLayout->addWidget(mBPasswordEdit, 3, 1, 1, 2);

    mViewLayout = new QVBoxLayout;
	mViewLayout->setMargin(160);
    mViewLayout->addLayout(mGridLayout);
    mViewLayout->addStretch(5);
    mViewLayout->addSpacing(30);
    mViewLayout->addLayout(mBtnLayout);
    setLayout(mViewLayout);
	setWindowIcon(QIcon("../image/title.png"));
    setWindowTitle(LOGIN_WINDOW_TITLE);
	QDesktopWidget *pDesk = QApplication::desktop();
	resize(LOGIN_WINDOW_WIDTH, LOGIN_WINDOW_HEIGHT);
	move((pDesk->width() - width()) / 2, (pDesk->height() - height()) / 2);
	initConnect();
}

USTPLoginDialog::~USTPLoginDialog()
{
}

void USTPLoginDialog::initConnect()
{	
	connect(mLoginBtn, SIGNAL(clicked()), this, SLOT(doUserLogin()));
	connect(mCancelBtn, SIGNAL(clicked()), this, SLOT(close()));
	
	connect(USTPCtpLoader::getUserATradeSpi(), SIGNAL(onCTPTradeRspUserLogin(const QString&, const QString&, const QString&, const int&, const int&, const int&, const int&, const QString&, bool)),
		this, SLOT(doCTPTradeRspUserLogin(const QString&, const QString&, const QString&, const int&, const int&, const int&, const int&, const QString&, bool)));

	connect(USTPCtpLoader::getUserATradeSpi(), SIGNAL(onCTPRspQryInstrument(const QString&, const QString&, const QString&, const double&, const int&, const int&, bool)),
		this, SLOT(doCTPRspQryInstrument(const QString&, const QString&, const QString&, const double&, const int&, const int&, bool)));

	connect(USTPCtpLoader::getUserATradeSpi(), SIGNAL(onCTPTradeSettlementInfoConfirm(const QString&, const QString&, const int&, const QString&)),
		this, SLOT(doCTPTradeSettlementInfoConfirm(const QString&, const QString&, const int&, const QString&)));

	connect(USTPCtpLoader::getUserATradeSpi(), SIGNAL(onCTPRspQryInvestorPosition(const QString&, const char&, const int&, const int&, const char&, const QString&, const QString&, const QString&, bool)), 
		this, SLOT(doCTPRspQryInvestorPosition(const QString&, const char&, const int&, const int&, const char&, const QString&, const QString&, const QString&, bool)));


	connect(USTPCtpLoader::getUserBTradeSpi(), SIGNAL(onSgitTradeRspUserLogin(const QString&, const QString&, const QString&, const int&, const int&, const int&, const int&, const QString&, bool)),
		this, SLOT(doSgitTradeRspUserLogin(const QString&, const QString&, const QString&, const int&, const int&, const int&, const int&, const QString&, bool)));

	connect(USTPCtpLoader::getUserBTradeSpi(), SIGNAL(onSgitRspQryInstrument(const QString&, const QString&, const QString&, const double&, const int&, const int&, bool)),
		this, SLOT(doSgitRspQryInstrument(const QString&, const QString&, const QString&, const double&, const int&, const int&, bool)));

	connect(USTPCtpLoader::getUserBTradeSpi(), SIGNAL(onSgitTradeSettlementInfoConfirm(const QString&, const QString&, const int&, const QString&)),
		this, SLOT(doSgitTradeSettlementInfoConfirm(const QString&, const QString&, const int&, const QString&)));

	connect(USTPCtpLoader::getUserBTradeSpi(), SIGNAL(onSgitRspQryInvestorPosition(const QString&, const char&, const int&, const int&, const char&, const QString&, const QString&, const QString&, bool)), 
		this, SLOT(doSgitRspQryInvestorPosition(const QString&, const char&, const int&, const int&, const char&, const QString&, const QString&, const QString&, bool)));
	
	connect(USTPCtpLoader::getAMdSpi(), SIGNAL(onCTPMdRspUserLogin(const QString&, const QString&, const int&, const QString&, bool)),
		this, SLOT(doCTPMdRspUserLogin(const QString&, const QString&, const int&, const QString&, bool)));

	connect(USTPCtpLoader::getBMdSpi(), SIGNAL(onSgitMdRspUserLogin(const QString&, const QString&, const int&, const QString&, bool)),
		this, SLOT(doSgitMdRspUserLogin(const QString&, const QString&, const int&, const QString&, bool)));
}

void USTPLoginDialog::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), QPixmap("../image/background.png"));
}

void USTPLoginDialog::doUserLogin()
{	
	mUserAUserId = mAUserEdit->text();
	mUserAPassword = mAPasswordEdit->text();

	mUserBUserId = mBUserEdit->text();
	mUserBPassword = mBPasswordEdit->text();
	if ((!mUserAUserId.isEmpty()) && (!mUserAPassword.isEmpty()) && (!mUserBUserId.isEmpty()) && (!mUserBPassword.isEmpty())){
		USTPMutexId::initialize();
		int nMdResult = CTPMdApi::reqUserLogin(mUserAUserId, mUserAUserId, mUserAPassword);
		if(nMdResult != 0){
			ShowWarning(tr("CTP�����¼������ʧ��."));
		}
	}else{
		ShowWarning(tr("�û��˺Ż�����Ϊ��."));
	}
}

void USTPLoginDialog::doCTPMdRspUserLogin(const QString& brokerId, const QString& userId, const int& errorId, const QString& errorMsg, bool bIsLast)
{
	if (errorId >= 0){
		int nTradeResult = CTPTradeApi::reqUserLogin(USTPMutexId::getMutexId(), mUserABrokerId, mUserAUserId, mUserAPassword);
		if(nTradeResult != 0)
			ShowWarning(tr("CTP���׵�¼����������ʧ��."));
	}
}

void USTPLoginDialog::doSgitMdRspUserLogin(const QString& brokerId, const QString& userId, const int& errorId, const QString& errorMsg, bool bIsLast)
{
	if (errorId >= 0){
		int nTradeResult = SgitTradeApi::reqUserLogin(USTPMutexId::getMutexId(), mUserBBrokerId, mUserBUserId, mUserBPassword);
		if(nTradeResult != 0)
			ShowWarning(tr("�����׵�¼����������ʧ��."));
	}
}

void USTPLoginDialog::doCTPTradeRspUserLogin(const QString& tradingDate, const QString& brokerId, const QString& userId, const int& maxLocalId, const int& frontId,
											  const int& sessionId, const int& errorId, const QString& errorMsg, bool bIsLast)
{
	if (errorId == 0){
		QString nextSecDay = USTPCtpLoader::getDateTime();
		QString validDay = USTPCtpLoader::getValidDateTime();
		if(tradingDate.toInt() > nextSecDay.toInt() || nextSecDay.toInt() > validDay.toInt()){
			ShowWarning(tr("ϵͳʱ�䲻�Ϸ�."));
			return;
		}
		USTPMutexId::setUserAInfo(userId, mUserAPassword, maxLocalId, frontId, sessionId);
		if(mUserAUserId == mUserBUserId)
			USTPMutexId::setUserBInfo(userId, mUserBPassword, maxLocalId, frontId, sessionId);

		int nResult = CTPTradeApi::reqSettlementInfoConfirm(USTPMutexId::getMutexId(), brokerId, userId);
		if(nResult != 0)
			ShowWarning(tr("CTP�˺��������ʧ��."));
	}else
		ShowWarning(tr("[CTP]") + errorMsg);
}

void USTPLoginDialog::doSgitTradeRspUserLogin(const QString& tradingDate, const QString& brokerId, const QString& userId, const int& maxLocalId, const int& frontId,
											 const int& sessionId, const int& errorId, const QString& errorMsg, bool bIsLast)
{
	if (errorId == 0){
		USTPMutexId::setUserBInfo(userId, mUserBPassword, maxLocalId, frontId, sessionId);
		int nResult = SgitTradeApi::reqQryInstrument(USTPMutexId::getMutexId(), "", "", "");
		if(nResult != 0)
			ShowWarning(tr("�����˺������ѯ��Լʧ��."));
	}else
		ShowWarning(tr("[SGIT]") + errorMsg);
}


void USTPLoginDialog::doCTPTradeSettlementInfoConfirm(const QString& brokerId, const QString& investorId, const int& errorId, const QString& errorMsg)
{
	if (errorId == 0){
		if(investorId == mUserAUserId){
			int nResult = CTPTradeApi::reqQryInstrument(USTPMutexId::getMutexId(), "", "", "");
			if(nResult != 0)
				ShowWarning(tr("CTP�����ѯ��Լʧ��."));
		}else{
			int nResult = CTPTradeApi::reqQryInvestorPosition(USTPMutexId::getMutexId(), mUserABrokerId, USTPMutexId::getAUserId(), "");
		}
	}else
		ShowWarning(tr("[CTP]") + errorMsg);
}

void USTPLoginDialog::doCTPRspQryInstrument(const QString& exchangeId, const QString& productId, const QString& instrumentId, const double& priceTick, 
											 const int& volumeMultiple, const int& maxMarketVolume, bool bIsLast)
{	
	USTPMutexId::setInsPriceTick(instrumentId, exchangeId, priceTick, volumeMultiple);
	USTPMutexId::setInsMarketMaxVolume(instrumentId, maxMarketVolume);

#ifdef _DEBUG
	QString data =  QString(tr(" [ [SGIT]MarketVolume ]  Instrument: ")) + instrumentId +  QString(tr("   MaxMarketVolume: ")) + QString::number(maxMarketVolume) +
		tr("  Exh: ") + exchangeId;
	USTPLogger::saveData(data);
#endif
	if(bIsLast){
		::Sleep(1000);
		int nResult = CTPTradeApi::reqQryInvestorPosition(USTPMutexId::getMutexId(), mUserABrokerId, USTPMutexId::getAUserId(), "");
	}
}

void USTPLoginDialog::doSgitRspQryInstrument(const QString& exchangeId, const QString& productId, const QString& instrumentId, const double& priceTick, 
											 const int& volumeMultiple, const int& maxMarketVolume, bool bIsLast)
{	
	USTPMutexId::setInsPriceTick(instrumentId, exchangeId, priceTick, volumeMultiple);
	USTPMutexId::setInsMarketMaxVolume(instrumentId, maxMarketVolume);

#ifdef _DEBUG
	QString data =  QString(tr(" [ [SGIT]MarketVolume]  Instrument: ")) + instrumentId +  QString(tr("   MaxMarketVolume: ")) + QString::number(maxMarketVolume) +
		tr("  Exh: ") + exchangeId;
	USTPLogger::saveData(data);
#endif
	if(bIsLast){
		::Sleep(1000);
		int nResult = SgitTradeApi::reqQryInvestorPosition(USTPMutexId::getMutexId(), mUserBBrokerId, USTPMutexId::getBUserId(), "");
	}
}

void  USTPLoginDialog::doCTPRspQryInvestorPosition(const QString& instrumentId, const char& direction, const int& position, const int& yPosition, const char& hedgeFlag,
													const QString& brokerId, const QString& tradingDay, const QString& investorId, bool bIsLast)
{	
	if(direction == '2' && instrumentId != ""){
		if (position > 0)
			USTPMutexId::addBidPosition(investorId, instrumentId, position);
	}else if(direction == '3' && instrumentId != ""){
		if (position > 0) 
			USTPMutexId::addAskPosition(investorId, instrumentId, position);
	}
	if(bIsLast){
		int nMdResult = SgitMdApi::reqUserLogin(mUserBUserId, mUserBUserId, mUserBPassword);
		if(nMdResult != 0)
			ShowWarning(tr("���������¼����������ʧ��."));
	}
}

void  USTPLoginDialog::doSgitRspQryInvestorPosition(const QString& instrumentId, const char& direction, const int& position, const int& yPosition, const char& hedgeFlag,
													const QString& brokerId, const QString& tradingDay, const QString& investorId, bool bIsLast)
{	
	if(direction == '2' && instrumentId != ""){
		if (position > 0)
			USTPMutexId::addBidPosition(investorId, instrumentId, position);
	}else if(direction == '3' && instrumentId != ""){
		if (position > 0) 
			USTPMutexId::addAskPosition(investorId, instrumentId, position);
	}
	if(bIsLast){
		accept();
	}
}

#include "moc_USTPLoginDialog.cpp"
