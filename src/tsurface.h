#ifndef TSURFACE_H
#define TSURFACE_H

#include <QMainWindow>

#include <memory>

namespace Ui {
class tsurface;
}

class tsurface : public QMainWindow
{
    Q_OBJECT

public:
    explicit tsurface(QWidget *parent = nullptr);
    ~tsurface() override;

private:
    const std::unique_ptr<Ui::tsurface> m_ui;
};

#endif // TSURFACE_H
