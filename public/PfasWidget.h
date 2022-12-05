#pragma once
#include <QWidget>

//! Widget subclass support CSS styling function
/*! Every widget that want to customize their style by using global CSS file shall
    inherit from this class.
*/
class PfasWidget : public QWidget {
  Q_OBJECT

public:
  PfasWidget(QWidget *_parent = nullptr);
  ~PfasWidget() override = default;

protected:
  //! Re-implement to support CSS style sheet
  /*! According to *Qt Style Sheets Reference*, customWidgets widgets derived from QWidget has to
     re-implement *paintEvent* to support CSS style sheet. The implemention code is universal and
     could be found on the same reference page.
   */
  void paintEvent(QPaintEvent *) override;

};
