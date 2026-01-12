#ifndef TRESOURCEDIALOG_H
#define TRESOURCEDIALOG_H

#include <QDialog>

namespace Ui {
class TResourceDialog;
}

class QModelIndex;
class QItemSelection;

class TResourceDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit TResourceDialog(QWidget *parent = nullptr);
        ~TResourceDialog();

        typedef enum
        {
            SHOW_LIST,
            SHOW_DETAIL,
            SHOW_ICON
        }SHOW_t;

        typedef enum
        {
            LABEL_LISTVIEW,
            LABEL_RESOURCES
        }LABEL_t;

        void setPathTemporary(const QString& path) { mPathTemporary = path; }
        void setLastOpenPath(const QString& path) { mLastOpenPath = path; }
        QString& getLastOpenPath() { return mLastOpenPath; }
        bool haveChanged() { return mChanged; }
        QStringList& getImageList() { return mImages; }

    protected:
        QPixmap sizeImage(const QSize& size, const QString& file);
        void removeItemFromListView(const QString& file, int row);
        void setLabel(LABEL_t lb, int number, const QString& text);

    private slots:
        void on_pushButtonCut_clicked();
        void on_pushButtonCopy_clicked();
        void on_pushButtonPaste_clicked();
        void on_pushButtonDelete_clicked();
        void on_pushButtonRename_clicked();
        void on_pushButtonUndo_clicked();
        void on_pushButtonRedo_clicked();
        void on_pushButtonImport_clicked();
        void on_pushButtonExport_clicked();
        void on_pushButtonDataMap_clicked();
        void on_comboBoxStyle_currentIndexChanged(int index);
        void on_listViewImages_activated(const QModelIndex &index);
        void on_listViewImages_entered(const QModelIndex &index);
        void on_tableViewDynamicImages_activated(const QModelIndex &index);
        void on_tableViewDynamicImages_doubleClicked(const QModelIndex &index);
        void on_tableViewSounds_activated(const QModelIndex &index);
        void on_tableViewDynamicDataSources_activated(const QModelIndex &index);

    private:
        Ui::TResourceDialog *ui;

        QString mPathTemporary;
        SHOW_t mShowType{SHOW_ICON};
        QString mPathTemplate;
        QString mMapsFile;
        QString mLastOpenPath;
        // Images
        QStringList mImages;
        bool mChanged{false};
};

#endif // TRESOURCEDIALOG_H
