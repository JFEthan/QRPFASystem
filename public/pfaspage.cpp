#include "pfaspage.h"
#include <QDesktopWidget>
#include <QApplication>
PfasPage::PfasPage(QWidget *_parent)
    :PfasUIItem(_parent)
{

}
PfasPage::~PfasPage() = default;


void PfasPage::OnCreate() {
  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

  QDesktopWidget* desktopWidget = QApplication::desktop();
  QRect _screen_rect = desktopWidget->availableGeometry(); //用户可用窗口大小
   setGeometry(_screen_rect);
//  if (ui_mgr) {
//    auto _screen_rect = ui_mgr->GetUIRect();
//    setGeometry(_screen_rect);
//  }
}

void PfasPage::OnEnter() {
  active = true;
  show();
  raise();
//  PFAS_LOG(trace) << "[LOG_TEST][CurrentPage=" << this->page_name << "]";
}

void PfasPage::OnInitialize()
{

}
void PfasPage::OnUnInitialize()
{

}

void PfasPage::OnExit() {
  active = false;
  hide();
}

void PfasPage::OnDestroy() {
  auto _vbox = layout();
  setLayout(nullptr);
  if (_vbox) {
    delete _vbox;
  }
}

bool PfasPage::IsActive() { return active; }

//PfasWidgetContent *PfasPage::GetContentWidget() {
//  auto _vbox = new QVBoxLayout;
//  content = new PfasWidgetContent;
//  _vbox->addWidget(content);
//  _vbox->setSpacing(0);
//  _vbox->setContentsMargins(0, 0, 0, 0);
//  setLayout(_vbox);
//  return content;
//}

