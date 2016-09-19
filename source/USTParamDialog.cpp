#include "USTParamDialog.h"
#include "USTPConfig.h"
#include "USTPStrategyWidget.h"

USTParamDialog::USTParamDialog(USTPStrategyWidget* pStrategyWidget, QWidget* parent)
:QDialog(parent)
{
	mCancelBtn = createButton(tr("ȡ ��"), SLOT(close()));;
	mConfirmBtn = createButton(tr("ȷ ��"), SLOT(createOrder()));
	mBtnLayout = new QHBoxLayout;
	mBtnLayout->addWidget(mConfirmBtn);
	mBtnLayout->addWidget(mCancelBtn);

	mOrderGroup = createOrderGroup();

	mOrderLayout = new QVBoxLayout;
	mOrderLayout->addWidget(mOrderGroup);

	mViewLayout = new QVBoxLayout;
	mViewLayout->addLayout(mOrderLayout);
	mViewLayout->addStretch(1);
	mViewLayout->addSpacing(12);
	mViewLayout->addLayout(mBtnLayout);	
	setLayout(mViewLayout);
	initConnect(pStrategyWidget);
	setWindowTitle(STRATERGY_WINDOW_TITLE);
}

USTParamDialog::~USTParamDialog()
{

}

QGroupBox* USTParamDialog::createOrderGroup()
{	
	QGroupBox *groupBox = new QGroupBox(tr("��������"));

	mFirstInsLabel = new QLabel(tr("CTP��Լ:"));
	mFirstInsEdit = new QLineEdit;
	mFirstInsLabel->setBuddy(mFirstInsEdit);

	mSecondInsLabel = new QLabel(tr("SGIT��Լ:"));
	mSecondInsEdit = new QLineEdit;
	mSecondInsLabel->setBuddy(mSecondInsEdit);

	mStyleLabel = new QLabel(tr("��������:"));
	QStringList list;
	list <<tr("0-��������");
	mStyleComboBox = createComboBox(list);
	mStyleComboBox->setCurrentIndex(0);
	mStyleLabel->setBuddy(mStyleComboBox);

	mDirectionLabel = new QLabel(tr("��/��:"));
	QStringList bsList;
	bsList << tr("0-����") << tr("1-����");
	mBSComboBox = createComboBox(bsList);
	mDirectionLabel->setBuddy(mBSComboBox);

	mOffsetLabel = new QLabel(tr("��/ƽ:"));
	QStringList offsetList;
	offsetList << tr("0-����") << tr("1-ƽ��") << tr("2-ƽ��") << tr("3-ƽ��");
	mOffsetComboBox = createComboBox(offsetList);
	mOffsetLabel->setBuddy(mOffsetComboBox);

	mHedgeLabel = new QLabel(tr("Ͷ��/�ױ�:"));
	QStringList hedgeList;
	hedgeList << tr("0-Ͷ��") << tr("1-����");
	mHedgeComboBox = createComboBox(hedgeList);
	mHedgeComboBox->setCurrentIndex(1);
	mHedgeLabel->setBuddy(mHedgeComboBox);

	mPriceLabel = new QLabel(tr("ί�м۲�:"));
	mPriceSpinBox = new QDoubleSpinBox;
	mPriceSpinBox->setRange(-10000.0, 10000.0);
	mPriceSpinBox->setDecimals(3);
	mPriceSpinBox->setSingleStep(1.0);
	mPriceSpinBox->setValue(20.0);
	mPriceLabel->setBuddy(mPriceSpinBox);

	mQtyLabel = new QLabel(tr("ί����(CTP/SGIT):"));
	mDivisionLabel = new QLabel(tr("   /"));
	mCtpQtySpinBox = new QSpinBox;
	mCtpQtySpinBox->setRange(1, 1000);
	mCtpQtySpinBox->setSingleStep(1);
	mCtpQtySpinBox->setValue(1);

	mSgitQtySpinBox = new QSpinBox;
	mSgitQtySpinBox->setRange(1, 1000);
	mSgitQtySpinBox->setSingleStep(1);
	mSgitQtySpinBox->setValue(1);

	mCycleLabel = new QLabel(tr("ѭ����:"));
	mCycleSpinBox = new QSpinBox;
	mCycleSpinBox->setRange(1, 1000);
	mCycleSpinBox->setSingleStep(1);
	mCycleSpinBox->setValue(10);
	mCycleLabel->setBuddy(mCycleSpinBox);


	QGridLayout* vBox = new QGridLayout;
	vBox->addWidget(mFirstInsLabel, 0, 0, 1, 1);
	vBox->addWidget(mFirstInsEdit, 0, 1, 1, 2);
	vBox->addWidget(mSecondInsLabel, 0, 3, 1, 1);
	vBox->addWidget(mSecondInsEdit, 0, 4, 1, 2);

	vBox->addWidget(mStyleLabel, 1, 0, 1, 1);
	vBox->addWidget(mStyleComboBox, 1, 1, 1, 2);
	vBox->addWidget(mDirectionLabel, 1, 3, 1, 1);
	vBox->addWidget(mBSComboBox, 1, 4, 1, 2);

	vBox->addWidget(mOffsetLabel, 2, 0, 1, 1);
	vBox->addWidget(mOffsetComboBox, 2, 1, 1, 2);
	vBox->addWidget(mHedgeLabel, 2, 3, 1, 1);
	vBox->addWidget(mHedgeComboBox, 2, 4, 1, 2);

	vBox->addWidget(mQtyLabel, 3, 0, 1, 1);
	vBox->addWidget(mCtpQtySpinBox, 3, 1, 1, 2);
	vBox->addWidget(mDivisionLabel, 3, 3, 1, 1);
	vBox->addWidget(mSgitQtySpinBox, 3, 4, 1, 2);

	vBox->addWidget(mPriceLabel, 4, 0, 1, 1);
	vBox->addWidget(mPriceSpinBox, 4, 1, 1, 2);
	vBox->addWidget(mCycleLabel, 4, 3, 1, 1);
	vBox->addWidget(mCycleSpinBox, 4, 4, 1, 2);
	groupBox->setLayout(vBox);
	return groupBox;
}


QComboBox* USTParamDialog::createComboBox(const QStringList &itemList)
{
	QComboBox *comboBox = new QComboBox;
	foreach(QString item, itemList){
		comboBox->addItem(item);
	}
	comboBox->setEditable(true);
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}

QPushButton *USTParamDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void USTParamDialog::initConnect(USTPStrategyWidget* pStrategyWidget)
{	
	connect(mStyleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(doEnableBox(int)));

	connect(this, SIGNAL(onCreateNewOrder(const QString&, const QString&, const QString&, const QString&, const QString&, const QString&, const double&, 
		const int&, const int&, const int&)), pStrategyWidget, SLOT(doCreateNewOrder(const QString&, const QString&, const QString&, const QString&, 
		const QString&, const QString&, const double&, const int&, const int&, const int&)));
}


void USTParamDialog::createOrder()
{
	QString firstName, secondName;
	firstName = mFirstInsEdit->text();
	secondName = mSecondInsEdit->text();
	if(firstName.isEmpty() || secondName.isEmpty()){
		ShowWarning(tr("��Լ���ò���Ϊ��."));
	}else{
		QString style = mStyleComboBox->currentText();
		QString bs = mBSComboBox->currentText();
		QString offset = mOffsetComboBox->currentText();
		QString hedge = mHedgeComboBox->currentText();
		double price = mPriceSpinBox->value();
		int ctpQty = mCtpQtySpinBox->value();
		int sgitQty = mSgitQtySpinBox->value();
		int cycleNum = mCycleSpinBox->value();
		emit onCreateNewOrder(firstName, secondName, style, bs, offset, hedge, price, ctpQty, sgitQty, cycleNum);
		accept();
	}
}

#include "moc_USTParamDialog.cpp"