// Author: Dmitry Kukovinets (d1021976@gmail.com)

#include "typeeditdialog.h"

TypeEditDialog::TypeEditDialog(QWidget *parent):
	QDialog(parent),
	nameLineEdit_(new LineEditConfirm(this)),
	scrollArea_(new QScrollArea(this)),
	buttonBox_(new QDialogButtonBox(this)),
	mainLayout_(new QVBoxLayout(this)),
	scrollAreaLayout_(new QVBoxLayout(scrollArea_))
{
	auto palette = this->palette();
	palette.setColor(QPalette::Background, Qt::white);
	this->setPalette(palette);
	
	this->setModal(true);
	
	this->scrollAreaLayout_->setContentsMargins(0, 5, 0, 0);
	this->scrollArea_->setLayout(this->scrollAreaLayout_);
	this->buttonBox_->setOrientation(Qt::Horizontal);
	this->buttonBox_->setStandardButtons(QDialogButtonBox::Close);
	
	this->mainLayout_->setContentsMargins(5, 5, 5, 5);
	this->mainLayout_->addWidget(this->nameLineEdit_);
	this->mainLayout_->addWidget(this->scrollArea_);
	this->mainLayout_->addWidget(this->buttonBox_);
	
	// Connections
	this->connect(this->nameLineEdit_, &LineEditConfirm::accepted,
				  this, &TypeEditDialog::onNameChanged);
	this->connect(this->buttonBox_, &QDialogButtonBox::clicked,
				  [this](QAbstractButton *) { this->close(); });
}


QString TypeEditDialog::name() const
{ return this->nameLineEdit_->text(); }

void TypeEditDialog::setName(const QString &name)
{ this->nameLineEdit_->setText(name); }

void TypeEditDialog::confirmNameChanges()
{ this->nameLineEdit_->confirmText(); }


QString TypeEditDialog::field(int index) const
{ return this->fields_[index]->text(); }

QString TypeEditDialog::originalField(int index) const
{ return this->fields_[index]->originalText(); }

void TypeEditDialog::setFields(const QList<QString> &fields)
{
	// Adding type labels and fields data
	int i = 0;
	for (const auto &field: fields) {
		auto lineEdit = new LineEditConfirm(field, this->scrollArea_);	// LineEdit with text of the second
		
		this->connect(lineEdit, &LineEditConfirm::accepted,
					  [this, i](QString newText) { this->onFieldChanged(i, newText); });
		++i;
		
		this->scrollAreaLayout_->addWidget(lineEdit);
		this->fields_.append(lineEdit);
	}
}


void TypeEditDialog::removeField(int index)
{
	delete this->fields_.at(index);
	this->fields_.removeAt(index);
}

void TypeEditDialog::confirmFieldChanges(int index)
{
	this->fields_[index]->confirmText();
}


void TypeEditDialog::clear()
{
	this->close();
	
	this->nameLineEdit_->setText("");
	for (auto &lineEdit: this->fields_)
		delete lineEdit;
	this->fields_.clear();
}


// Slots
void TypeEditDialog::onNameChanged(QString newText)
{ emit nameChanged(newText); }


void TypeEditDialog::onFieldChanged(int index, QString newText)
{ emit fieldChanged(index, newText); }
