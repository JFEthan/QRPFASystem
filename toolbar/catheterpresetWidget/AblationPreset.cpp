#include "AblationPreset.h"
#include "ui_AblationPreset.h"
#include <QFileDialog>


AblationPreset::AblationPreset(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AblationPreset)
{
    ui->setupUi(this);
    Init();

    connect(ui->btnPresetEdit,SIGNAL(clicked()),this,SLOT(OnbtnPresetEditClicked()));
    connect(ui->btnPresetSetting,SIGNAL(clicked()),this,SLOT(OnbtnPresetSettingClicked()));
    connect(ui->btnPresetConfirm,SIGNAL(clicked()),this,SLOT(OnbtnPresetConfirmClicked()));

    connect(ui->btnAddCatheter,SIGNAL(clicked()),this,SLOT(OnbtnAddCatheterClicked()));
    connect(ui->btnDeleteCatheter,SIGNAL(clicked()),this,SLOT(OnbtnDeleteCatheterClicked()));
    connect(ui->btnAddList,SIGNAL(clicked()),this,SLOT(OnbtnAddListClicked()));
    connect(ui->btnDeleteList,SIGNAL(clicked()),this,SLOT(OnbtnDeleteListClicked()));


}

AblationPreset::~AblationPreset()
{
    delete ui;
}

void AblationPreset::Init()
{ 
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_2->setFocusPolicy(Qt::NoFocus);

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget_2->verticalHeader()->setVisible(false);

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{border:none;background-color:transparent;height:32px;font-size:14px;}");
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border:none;background-color:transparent;height:32px;font-size:14px;}");

    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget_2->setFrameShape(QFrame::NoFrame);

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget_2->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget_2->resizeRowsToContents();

    ui->tableWidget->setStyleSheet("QTableWidget::item{border:2px solid rgb(12, 75, 153);background-color:white;}QTableWidget::item:selected{background:lightblue;}");
    ui->tableWidget_2->setStyleSheet("QTableWidget::item{border:2px solid rgb(12, 75, 153);background-color:white;}QTableWidget::item:selected{background:lightblue;}");




    ui->btnPresetEdit->setStyleSheet("background-color:transparent;");
    ui->btnPresetSetting->setStyleSheet("background-color:transparent;");
    ui->btnPresetConfirm->setStyleSheet("background-color:transparent;");
    ui->btnUSB1->setStyleSheet("background-color:transparent;");
    ui->btnUSB2->setStyleSheet("background-color:transparent;");
    ui->btnAddCatheter->setStyleSheet("background-color:transparent;");
    ui->btnDeleteCatheter->setStyleSheet("background-color:transparent;");
    ui->btnAddList->setStyleSheet("background-color:transparent;");
    ui->btnDeleteList->setStyleSheet("background-color:transparent;");
}

void AblationPreset::OnbtnPresetEditClicked()
{

    //QList<QTableWidgetSelectionRange> ranges1 = ui->tableWidget->selectedRanges();
    //QList<QTableWidgetSelectionRange> ranges2 = ui->tableWidget_2->selectedRanges();
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::DoubleClicked);

    ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsEditable);
    ui->tableWidget->item(1,0)->setFlags(Qt::ItemIsEditable);
    for(int i = 0;i<4;i++)
    {
        ui->tableWidget_2->item(0,i)->setFlags(Qt::ItemIsEditable);
        ui->tableWidget_2->item(1,i)->setFlags(Qt::ItemIsEditable);
    }
}

void AblationPreset::OnbtnPresetSettingClicked()
{

}

void AblationPreset::OnbtnPresetConfirmClicked()
{
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsDragEnabled);
    //ui->tableWidget->setFocus();

    ////////save in config.json
}
void AblationPreset::OnbtnAddCatheterClicked()
{
    int rowIndex = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowIndex);
    //ui->tableWidget->item(rowIndex,0)->setFlags(Qt::Item);
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsEditable);
    ui->tableWidget->item(1,0)->setFlags(Qt::ItemIsEditable);

    ui->tableWidget->setStyleSheet("QTableWidget::item{border:2px solid rgb(12, 75, 153);background-color:white;}QTableWidget::item:selected{background:lightblue;}");

    //ui->tableWidget->setShowGrid(true);
}
void AblationPreset::OnbtnDeleteCatheterClicked()
{
    int rowIndex = ui->tableWidget->currentRow();
    if(rowIndex>1)
        ui->tableWidget->removeRow(rowIndex);
}
void AblationPreset::OnbtnAddListClicked()
{
    int rowIndex = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->insertRow(rowIndex);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::DoubleClicked);
    for(int i = 0;i<4;i++)
    {
        ui->tableWidget_2->item(0,i)->setFlags(Qt::ItemIsEditable);
        ui->tableWidget_2->item(1,i)->setFlags(Qt::ItemIsEditable);
    }

    ui->tableWidget_2->setStyleSheet("QTableWidget::item{border:2px solid rgb(12, 75, 153);background-color:white;}QTableWidget::item:selected{background:lightblue;}");

    //ui->tableWidget_2->setShowGrid(true);
}
void AblationPreset::OnbtnDeleteListClicked()
{
    int rowIndex = ui->tableWidget_2->currentRow();
    if(rowIndex!=-1)
        ui->tableWidget_2->removeRow(rowIndex);
}
