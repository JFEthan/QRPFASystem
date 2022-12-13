#ifndef ABLATIONPAGE_H
#define ABLATIONPAGE_H

#include <QWidget>
#include "../public/pfaspage.h"

class PfAblationContent;
class RfAblationContent;
class RfPunctureContent;
class RPFASummaryContent;

namespace Ui {
class AblationPage;
}

class AblationPage : public PfasPage
{
    Q_OBJECT

public:
    explicit AblationPage(QWidget *parent = nullptr);
    ~AblationPage();

public:
    void OnCreate() override;
    void OnDestroy() override;
    void OnInitialize() override;
    void OnUnInitialize() override;
    void OnEnter() override;
    void OnExit() override;

public slots:
   void OnMainButtonClicked();
   void OnPFAblationButtonClicked();
   void OnRFAblationButtonClicked();
   void OnPFPunctureButtonClicked();
   void OnPostButtonClicked();
private:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::AblationPage *ui;
    PfAblationContent *pf_ablation_content = nullptr;
    RfAblationContent *rf_ablation_content = nullptr;
    RfPunctureContent *rf_puncturen_content = nullptr;
    RPFASummaryContent *rpfa_summary_comtent = nullptr;
};

#endif // ABLATIONPAGE_H
