#pragma once
#include "core.h"
#include "ops.h"
#include "scanparams.h"
#include "imageview.h"
#include <QtWidgets>
#include <QtConcurrent>
#include <variant>

namespace spng {
class Viewer:public QMainWindow {
    Q_OBJECT
public:
    explicit Viewer(QWidget* parent=nullptr);
    ~Viewer()override;
    QHash<QString,QObject*> objects;
    template<class T>T* get(const char* name)const{return qobject_cast<T*>(objects.value(QString::fromUtf8(name)));}
    QLabel* label(const char* n)const{return get<QLabel>(n);}
    QComboBox* combo(const char* n)const{return get<QComboBox>(n);}
    QSpinBox* spin(const char* n)const{return get<QSpinBox>(n);}
    QDoubleSpinBox* real(const char* n)const{return get<QDoubleSpinBox>(n);}
    QCheckBox* check(const char* n)const{return get<QCheckBox>(n);}
    QPushButton* button(const char* n)const{return get<QPushButton>(n);}
    ImageView* view=nullptr;
    std::shared_ptr<Stack> stack;
    Image image;int loadedIndex=-1;QString currentPath,lastOpenDir,lastSaveDir;
    bool loadingImage=false,levelsInitialized=false,playing=false,bulkRunning=false,closing=false;
    BankPtr rawBank,processed;ProjectionPtr projection;
    std::optional<Point> points[2];
    QThreadPool pool;QTimer debounce,playTimer;
    QHash<QString,quint64> tokens;quint64 serial=0,bulkSerial=0;
    Cancel cancellation;
    QVector<FileRecord> records;
    QHash<int,Image> frameCache;QList<int> cacheOrder;quint64 cacheBytes=0;
    QGraphicsLineItem* centerCross[2]={nullptr,nullptr};
    bool scanPending=false,scanError=false;
    std::optional<ScanSettings> scanSettings;QString scanOverride,scanOverrideScope;
    QHash<QString,QPair<double,double>> scanExact;
    void openPath(const QString& path);
    void openStack(const QString& path);
    void clearStack();
    void stackReady(std::shared_ptr<Stack> value);
    void setCombo(QComboBox*,QStringList,QString preferred={},QStringList data={});
    void containerChanged();void sensorChanged();void layerChanged();void kindChanged();void viewChanged();void fileChanged();void typedView();
    void requestLayer();void loadRequestedLayer();void frameReady(int,Image);void stepLayer(int);
    void setLevels(double,double);void resetLevels();void refreshDisplay();void applyRoi();void roiChanged();void cursorPosition(QPointF={},bool valid=false);
    void updateControls();void saveDialog(bool crop);void showHelp();
    QString saveDisplayedTo(QString path,bool crop);
    void bindControls();void stopPlay();void togglePlay();void beginPlay();void playTick();void schedulePlay();
    void startPreload(bool playAfter=false);void startProcessing();void startProjection(QString kind);void startExport();void cancelBulk();
    void stageChanged();void renderProjection();void returnToSlice();
    void armPoint(int tag);void registerPoint(int tag);void resetPoint(int tag);void pickedPoint(int tag,int x,int y);void centerFollow();void rangeToPoints();
    FilterConfig filterConfig()const;void setParameterWidgets(const FilterConfig&);
    void reloadScanParameters();void chooseScanParameters();void resetScanParameters();void invalidateScanParameters();
    Reader reader()const;QString stage()const;QJsonObject processingMetadata()const;
    void invalidate(std::initializer_list<QString> channels){for(auto c:channels)tokens[c]=0;}
    void jobError(QString channel,QString error);
    template<class F,class C>void beginBulk(QString title,F operation,C callback){
        if(bulkRunning)return;
        stopPlay();debounce.stop();invalidate({"frame"});loadingImage=false;bulkRunning=true;
        const quint64 number=++bulkSerial;cancellation=std::make_shared<std::atomic_bool>(false);auto cancel=cancellation;
        auto bar=get<QProgressBar>("progress_bar");bar->setRange(0,100);bar->setValue(0);bar->show();button("cancel_button")->show();label("state_label")->setText(title);
        Progress progress=[this,number](int done,int total,QString message){QMetaObject::invokeMethod(this,[this,number,done,total,message]{if(!closing&&number==bulkSerial&&bulkRunning){get<QProgressBar>("progress_bar")->setValue(qRound(double(done)/std::max(1,total)*100));label("state_label")->setText(QString("%1　%2/%3").arg(message).arg(done).arg(total));}},Qt::QueuedConnection);};
        updateControls();run("bulk",[operation,progress,cancel]{return operation(progress,cancel);},[this,callback](auto result){bulkRunning=false;get<QProgressBar>("progress_bar")->hide();button("cancel_button")->hide();updateControls();callback(result);});
    }
    template<class F,class C>void run(QString channel,F fn,C callback){
        using T=std::invoke_result_t<F>;using Result=std::variant<T,std::exception_ptr>;
        const auto token=++serial;tokens[channel]=token;
        auto watcher=new QFutureWatcher<Result>(this);
        connect(watcher,&QFutureWatcher<Result>::finished,this,[this,watcher,channel,token,callback](){
            Result r=watcher->result();watcher->deleteLater();if(closing||tokens.value(channel)!=token)return;
            if(auto err=std::get_if<std::exception_ptr>(&r)){try{std::rethrow_exception(*err);}catch(const std::exception& e){jobError(channel,QString::fromUtf8(e.what()));}return;}
            callback(std::get<T>(std::move(r)));
        });
        watcher->setFuture(QtConcurrent::run(&pool,[fn]()->Result{try{return Result(std::in_place_index<0>,fn());}catch(...){return Result(std::in_place_index<1>,std::current_exception());}}));
    }
protected:
    void closeEvent(QCloseEvent*)override;
    void dragEnterEvent(QDragEnterEvent*)override;
    void dropEvent(QDropEvent*)override;
};
}
