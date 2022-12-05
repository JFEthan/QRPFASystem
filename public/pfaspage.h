#ifndef PFASPAGE_H
#define PFASPAGE_H

#include "PfasUIItem.h"

class PfasPage : public PfasUIItem
{
public:
    PfasPage(QWidget *_parent);
    ~PfasPage() override;

    void OnCreate() override;
    void OnDestroy() override;
    void OnInitialize() override;
    void OnUnInitialize() override;
    void OnEnter() override;
    void OnExit() override;

    bool IsActive();
//protected:
//  PfasWidgetContent *GetContentWidget();

protected:
    bool active = false;
};

#endif // PFASPAGE_H
