#pragma once
#include "PfasWidget.h"
#include <QPaintEvent>


//! Base class for all **PFAS** UI widgets
/*! _PfasUIItem_ provides the basic lifecycle management of UI widgets.
 */
class PfasUIItem : public PfasWidget {
  Q_OBJECT

public:
  PfasUIItem(QWidget *_parent) : PfasWidget(_parent){};
  virtual ~PfasUIItem() = default;

  virtual void OnCreate() = 0;
  virtual void OnDestroy() = 0;
  virtual void OnInitialize() = 0;
  virtual void OnUnInitialize() = 0;
  virtual void OnEnter() = 0;
  virtual void OnExit() = 0;

};
