#include "viewer.h"
#include <QTest>
#include <QFontDatabase>
#include <QBuffer>
#include <thread>

using namespace spng;
class ViewerTests:public QObject {
    Q_OBJECT
    const QString root=QStringLiteral(SPNG_TEST_ROOT);
    QString testDir,path;std::unique_ptr<Viewer> w;
    QString writeStack(QString relative,QVector<Image> images){
        QString file=QDir(testDir).filePath(relative);QDir().mkpath(QFileInfo(file).absolutePath());QFile f(file);if(!f.open(QIODevice::WriteOnly))qFatal("Cannot create synthetic fixture");
        for(auto im:images){QByteArray bytes;if(!im.empty()){auto png=savePng(im,QDir(testDir).filePath("fixture-image.png"));QFile p(png);p.open(QIODevice::ReadOnly);bytes=p.readAll();}char length[8];qToLittleEndian<quint64>(im.empty()?std::numeric_limits<quint64>::max():quint64(bytes.size()),length);f.write(length,8);if(!im.empty())f.write(bytes);}f.close();return file;
    }
    Image constant(int value,int width=32,int height=24){return {cv::Mat(height,width,CV_8U,cv::Scalar(value)),"L"};}
    void selectLayer(int index){w->spin("layer_number")->setValue(index);}
    void setPoint(int tag,int z,double x,double y){auto key=[tag](QString axis){return QString("point%1_%2").arg(tag).arg(axis).toUtf8();};w->spin(key("z"))->setValue(z);w->real(key("x"))->setValue(x);w->real(key("y"))->setValue(y);w->registerPoint(tag);}
    void chooseDialogPath(QString target){QTimer::singleShot(0,qApp,[target]{for(auto widget:QApplication::topLevelWidgets())if(auto d=qobject_cast<QFileDialog*>(widget)){d->setDirectory(QFileInfo(target).isDir()?target:QFileInfo(target).absolutePath());if(!QFileInfo(target).isDir())d->selectFile(target);QMetaObject::invokeMethod(d,"accept",Qt::QueuedConnection);return;}});}
    QString prepareScan(){
        auto acquisition=testDir+"/scan/0000";QDir().mkpath(acquisition+"/PARAMS");QJsonArray params;
        for(int layer=0;layer<2;layer++)params.append(QJsonObject{{"ResizeFactor",1.41421356},{"NormCutOffFreq",.12345678},{"WeightZFilter",-.23456789},{"ThrBinary",14-layer},{"ThrBinaryPlus",-5-layer},{"Expansion",2}});
        QFile f(acquisition+"/PARAMS/CombinedParam.json");if(!f.open(QIODevice::WriteOnly))qFatal("Cannot create synthetic parameter fixture");f.write(QJsonDocument(QJsonObject{{"ImageFilterParam",QJsonObject{{"Algorithm","ImageFilterWithInterpolation_GPU"},{"CommonParamArray",params}}}}).toJson());f.close();
        for(int layer=0;layer<3;layer++)writeStack(QString("scan/0000/IMAGE/00_00/Raw_0_00000000_%1_001.spng").arg(layer),{constant(50)});
        return acquisition;
    }
    void normalizeScreenshot(){
        w->label("source_label")->setText("tests/screen_fixture/Raw_0_00000000_0_005.spng");
        w->label("source_label")->setToolTip("");
    }
    void compareScreenshot(QImage actual,const QString& filename){
        const QString expectedPath=root+"/evidence/expected_"+filename;
        if(qEnvironmentVariable("SPNG_UPDATE_EXPECTED")=="1")QVERIFY(actual.save(expectedPath));
        QImage expected(expectedPath);QVERIFY2(!expected.isNull(),qPrintable(expectedPath));
        actual.setDevicePixelRatio(expected.devicePixelRatio());
        QCOMPARE(actual.convertToFormat(QImage::Format_RGBA8888),expected.convertToFormat(QImage::Format_RGBA8888));
    }
private slots:
    void initTestCase(){testDir=root+"/tests/gui_"+QUuid::createUuid().toString(QUuid::WithoutBraces);QDir().mkpath(testDir);QVector<Image> frames;for(int i=0;i<5;i++)frames.append(constant(50+20*i));path=writeStack("Raw_0_00000000_0_005.spng",frames);}
    void init(){w=std::make_unique<Viewer>();w->setAttribute(Qt::WA_DontShowOnScreen,true);w->show();w->openPath(path);QTRY_VERIFY_WITH_TIMEOUT(!w->image.empty()&&!w->loadingImage&&!w->scanPending,10000);}
    void cleanup(){if(w){w->close();w.reset();QCoreApplication::sendPostedEvents(nullptr,QEvent::DeferredDelete);QCoreApplication::processEvents();}}
    void initialValues(){QCOMPARE(w->image.size(),QSize(32,24));QCOMPARE(w->image.pixels.at<quint8>(0,0),quint8(50));QCOMPARE(w->combo("sensor_combo")->maxVisibleItems(),12);QCOMPARE(w->real("scale_box")->value(),1.4142);QCOMPARE(w->real("z_weight")->value(),-.2);QCOMPARE(w->real("scale_box")->singleStep(),.0001);QCOMPARE(w->real("cutoff_box")->singleStep(),.01);QCOMPARE(w->real("z_weight")->singleStep(),.01);QVERIFY(w->combo("interp_combo")->currentText().startsWith("GPU"));QVERIFY(w->check("scan_auto")->isChecked());QVERIFY(w->label("filename")->text().endsWith("　／　元画像"));}
    void layersCursorAndCrop(){
        Image a=constant(0,7,5);for(int y=0;y<5;y++)for(int x=0;x<7;x++)a.pixels.at<quint8>(y,x)=quint8(y*7+x);auto file=writeStack("arange.spng",{a,{},constant(120,7,5)});w->openPath(file);QTRY_VERIFY(!w->image.empty()&&!w->loadingImage);
        w->cursorPosition({.1,4.1},true);QVERIFY(w->label("cursor_label")->text().endsWith("28"));QVERIFY(w->label("cursor_label")->text().contains("X: 0　Y: 0"));
        auto start=w->view->mapFromScene({1.2,1.2}),end=w->view->mapFromScene({4.2,3.2});QTest::mousePress(w->view->viewport(),Qt::LeftButton,Qt::ControlModifier,start);QTest::mouseMove(w->view->viewport(),end);QTest::mouseRelease(w->view->viewport(),Qt::LeftButton,Qt::ControlModifier,end);QCOMPARE(w->view->roi,std::optional<QRect>(QRect(1,1,4,3)));QCOMPARE(w->spin("roi_y")->value(),1);
        w->real("low")->setValue(10);w->real("high")->setValue(20);w->check("invert")->setChecked(true);auto out=w->saveDisplayedTo(QDir(testDir).filePath("cropped.png"),true);QImage png(out);QCOMPARE(png.size(),QSize(4,3));QCOMPARE(qGray(png.pixel(0,0)),8);QCOMPARE(qGray(png.pixel(3,2)),25);
        selectLayer(1);QTRY_COMPARE(w->loadedIndex,1);QVERIFY(w->image.empty());QVERIFY(!w->get<QAction>("full_action")->isEnabled());selectLayer(2);QTRY_VERIFY(w->loadedIndex==2&&!w->image.empty());QCOMPARE(w->image.pixels.at<quint8>(0,0),quint8(120));
    }
    void leftDragPans(){w->view->setImage(QImage(1600,1200,QImage::Format_Grayscale8),true);w->view->setZoom(200);w->view->setRoi(QRect(500,500,50,50));auto h=w->view->horizontalScrollBar(),v=w->view->verticalScrollBar();h->setValue((h->minimum()+h->maximum())/2);v->setValue((v->minimum()+v->maximum())/2);int x=h->value(),y=v->value();auto p=w->view->viewport()->rect().center();QTest::mousePress(w->view->viewport(),Qt::LeftButton,Qt::NoModifier,p);QTest::mouseMove(w->view->viewport(),p+QPoint(50,30));QTest::mouseRelease(w->view->viewport(),Qt::LeftButton,Qt::NoModifier,p+QPoint(50,30));QCOMPARE(h->value(),x-50);QCOMPARE(v->value(),y-30);QCOMPARE(w->view->roi,std::optional<QRect>(QRect(500,500,50,50)));}
    void pendingLayerDoesNotDarken(){
        w->view->setRoi(QRect(1,1,5,5));QCoreApplication::processEvents();auto before=w->view->viewport()->grab().toImage();auto release=std::make_shared<std::atomic_bool>(false);auto started=std::make_shared<std::atomic_int>(0);
        for(QString channel:{"test_block1","test_block2"})w->run(channel,[release,started]{started->fetch_add(1);while(!release->load())std::this_thread::sleep_for(std::chrono::milliseconds(2));return 0;},[](int){});
        // Release unconditionally before an assertion can end the test and invoke cleanup.
        QElapsedTimer timer;timer.start();while(started->load()<2&&timer.elapsed()<3000)QTest::qWait(5);
        selectLayer(2);w->debounce.stop();w->loadRequestedLayer();QCoreApplication::processEvents();bool pending=w->loadingImage,disabled=!w->get<QAction>("full_action")->isEnabled(),same=before==w->view->viewport()->grab().toImage();release->store(true);
        QVERIFY(pending);QVERIFY(disabled);QVERIFY(same);QTRY_VERIFY(w->loadedIndex==2&&!w->loadingImage);QVERIFY(w->view->roiItem->isVisible());
    }
    void playbackStopAndLoop(){w->check("loop_check")->setChecked(false);w->spin("play_end")->setValue(2);w->real("play_rate")->setValue(60);w->togglePlay();QTRY_VERIFY_WITH_TIMEOUT(w->rawBank&&!w->playing&&w->loadedIndex==2,10000);w->check("loop_check")->setChecked(true);w->togglePlay();int wraps=0,previous=-1;QElapsedTimer timer;timer.start();while(wraps<2&&timer.elapsed()<3000){QTest::qWait(3);int current=w->loadedIndex;if(current==0&&previous>0)wraps++;if(current>=0)previous=current;}w->stopPlay();QVERIFY(wraps>=2);}
    void followingAndReset(){setPoint(0,0,4,6);setPoint(1,4,24,16);QVERIFY(w->check("follow_check")->isChecked());w->view->setZoom(200);selectLayer(2);QTRY_COMPARE(w->loadedIndex,2);auto p=w->view->mapFromScene({14.5,24-.5-11});auto c=w->view->viewport()->rect().center();QVERIFY(std::abs(p.x()-c.x())<=2);QVERIFY(std::abs(p.y()-c.y())<=2);w->resetPoint(0);QVERIFY(!w->points[0]);QVERIFY(w->points[1]);QVERIFY(!w->check("follow_check")->isChecked());QCOMPARE(w->spin("point0_z")->text(),QString());QCOMPARE(w->real("point0_x")->text(),QString());w->resetPoint(1);QVERIFY(!w->points[1]);}
    void processingStagesAndScaledPoints(){w->check("resize_check")->setChecked(true);w->real("scale_box")->setValue(2);w->check("highpass_check")->setChecked(false);w->check("z_check")->setChecked(false);w->check("binary_check")->setChecked(false);w->check("dilate_check")->setChecked(false);w->startProcessing();QTRY_VERIFY_WITH_TIMEOUT(w->processed&&!w->bulkRunning&&!w->loadingImage&&!w->image.empty(),10000);QCOMPARE(w->stage(),QString("補間後"));QCOMPARE(w->image.size(),QSize(64,48));setPoint(0,0,4,6);setPoint(1,4,24,16);w->view->setZoom(200);selectLayer(2);QTRY_COMPARE(w->loadedIndex,2);auto p=w->view->mapFromScene({29.,48-23.});auto c=w->view->viewport()->rect().center();QVERIFY(std::abs(p.x()-c.x())<=2);QVERIFY(std::abs(p.y()-c.y())<=2);auto out=w->saveDisplayedTo(QDir(testDir).filePath("processed.png"),false);QImage saved(out);QCOMPARE(saved.size(),QSize(64,48));QCOMPARE(qGray(saved.pixel(0,0)),90);QFile record(QDir(testDir).filePath("processed.json"));QVERIFY(record.open(QIODevice::ReadOnly));auto m=QJsonDocument::fromJson(record.readAll()).object();QCOMPARE(m["processing"].toObject()["config"].toObject()["scale"].toDouble(),2.);}
    void binaryAndStageSwitch(){w->check("resize_check")->setChecked(false);w->check("highpass_check")->setChecked(false);w->check("z_check")->setChecked(false);w->real("threshold_box")->setValue(100);w->spin("dilate_box")->setValue(1);w->startProcessing();QTRY_VERIFY(w->processed&&!w->bulkRunning&&!w->loadingImage);QCOMPARE(w->stage(),QString("二値化後"));QCOMPARE(w->real("high")->value(),1.);selectLayer(4);QTRY_COMPARE(w->loadedIndex,4);QCOMPARE(w->image.pixels.at<quint8>(0,0),quint8(1));w->combo("stage_combo")->setCurrentText("元画像");QTRY_VERIFY(w->loadedIndex==4&&!w->loadingImage);QCOMPARE(w->image.pixels.at<quint8>(0,0),quint8(130));QCOMPARE(w->real("high")->value(),255.);}
    void projectionAndAllPalettes(){w->get<QLineEdit>("layer_spec")->setText("0,2-4");w->startProjection("max");QTRY_VERIFY(w->projection&&!w->bulkRunning);QCOMPARE(w->projection->values.pixels.at<quint8>(0,0),quint8(130));QCOMPARE(w->projection->layers.at<int>(0,0),4);w->check("invert")->setChecked(true);w->check("depth_check")->setChecked(true);QVERIFY(!w->check("invert")->isChecked());QVERIFY(!w->check("invert")->isEnabled());for(int i=0;i<5;i++){w->combo("palette_combo")->setCurrentIndex(i);QVERIFY(!w->combo("palette_combo")->itemIcon(i).isNull());auto expected=projectionImage(*w->projection,true,false,w->combo("palette_combo")->currentData().toString());QCOMPARE(cv::norm(w->image.pixels,expected.pixels,cv::NORM_INF),0.);}w->cursorPosition({1,2},true);QVERIFY(w->label("cursor_label")->text().contains("選択層: 4"));w->returnToSlice();QTRY_VERIFY(!w->projection&&!w->loadingImage&&!w->image.empty());QCOMPARE(w->image.mode,QString("L"));}
    void cancelAndStaleBulk(){auto called=std::make_shared<std::atomic_bool>(false);w->beginBulk("test",[](Progress,Cancel c){for(int i=0;i<100;i++){checkCancel(c);std::this_thread::sleep_for(std::chrono::milliseconds(5));}return 1;},[called](int){called->store(true);});w->cancelBulk();QTRY_VERIFY(!w->bulkRunning);QVERIFY(!called->load());QCOMPARE(w->label("state_label")->text(),QString("処理を中止しました。"));w->beginBulk("test",[](Progress,Cancel){std::this_thread::sleep_for(std::chrono::milliseconds(50));return 1;},[called](int){called->store(true);});w->openPath(path);QTRY_VERIFY(!w->image.empty()&&!w->loadingImage);QTest::qWait(150);QVERIFY(!called->load());}
    void scanLayerSettings(){
        auto acquisition=QDir(testDir).filePath("scan/0000");QDir().mkpath(acquisition+"/PARAMS");QJsonArray params;
        for(int layer=0;layer<2;layer++)params.append(QJsonObject{{"ResizeFactor",1.41421356},{"NormCutOffFreq",.12345678},{"WeightZFilter",-.23456789},{"ThrBinary",14-layer},{"ThrBinaryPlus",-5-layer},{"Expansion",2}});
        QFile file(acquisition+"/PARAMS/CombinedParam.json");QVERIFY(file.open(QIODevice::WriteOnly));file.write(QJsonDocument(QJsonObject{{"ImageFilterParam",QJsonObject{{"Algorithm","ImageFilterWithInterpolation_GPU"},{"CommonParamArray",params}}}}).toJson());file.close();
        auto p0=writeStack("scan/0000/IMAGE/00_00/Raw_0_00000000_0_001.spng",{constant(50)});auto p1=writeStack("scan/0000/IMAGE/00_00/Raw_0_00000000_1_001.spng",{constant(50)});auto p2=writeStack("scan/0000/IMAGE/00_00/Raw_0_00000000_2_001.spng",{constant(50)});
        w->openPath(p0);QTRY_VERIFY(w->scanSettings&&!w->scanPending);QCOMPARE(w->real("threshold_box")->value(),9.);QCOMPARE(w->filterConfig().scale,1.41421356);QCOMPARE(w->filterConfig().cutoff,.12345678);QCOMPARE(w->filterConfig().zWeight,-.23456789);w->real("cutoff_box")->stepUp();QCOMPARE(w->filterConfig().cutoff,w->real("cutoff_box")->value());
        w->openPath(p1);QTRY_VERIFY(w->scanSettings&&w->scanSettings->layer==1&&!w->scanPending);QCOMPARE(w->real("threshold_box")->value(),7.);w->openPath(p2);QTRY_VERIFY(w->scanError&&!w->scanPending);QVERIFY(!w->button("process_button")->isEnabled());w->check("scan_auto")->setChecked(false);QVERIFY(w->button("process_button")->isEnabled());
    }
    void folderNavigation(){writeStack("dataset/IMAGE/00_00/Raw_0_00000000_0_001.spng",{constant(10)});writeStack("dataset/IMAGE/00_00/Raw_0_00000002_0_001.spng",{constant(20)});writeStack("dataset/IMAGE/00_00/Raw_0_00000003_1_001.spng",{constant(30)});writeStack("dataset/IMAGE/00_00/IF1_Filtered_2_00000000_0_001.spng",{constant(40)});writeStack("dataset/IMAGE/00_01/Raw_0_00000010_0_001.spng",{constant(50)});QDir().mkpath(testDir+"/dataset/DATA/00_00");w->openPath(testDir+"/dataset");QTRY_VERIFY(!w->image.empty()&&!w->loadingImage);QCOMPARE(w->combo("kind_combo")->currentText(),QString("Raw_0"));w->combo("view_combo")->setEditText("00000002");w->typedView();QTRY_VERIFY(!w->image.empty()&&w->image.pixels.at<quint8>(0,0)==20);w->combo("layer_combo")->setCurrentText("Layer1");QTRY_VERIFY(!w->image.empty()&&w->image.pixels.at<quint8>(0,0)==30);w->combo("layer_combo")->setCurrentText("Layer0");w->combo("kind_combo")->setCurrentText("IF1_Filtered_2");QTRY_VERIFY(!w->image.empty()&&w->image.pixels.at<quint8>(0,0)==40);w->combo("sensor_combo")->setCurrentText("00_01");QTRY_VERIFY(!w->image.empty()&&w->image.pixels.at<quint8>(0,0)==50);w->combo("container_combo")->setCurrentText("DATA");QTRY_VERIFY(w->label("state_label")->text().contains("spngがありません"));QVERIFY(w->image.empty());}
    void multiExportThroughDialog(){w->get<QLineEdit>("layer_spec")->setText("0,2,4");w->view->setRoi(QRect(1,20,4,4));w->check("batch_crop")->setChecked(true);chooseDialogPath(testDir);w->startExport();QTRY_VERIFY_WITH_TIMEOUT(!w->bulkRunning&&w->label("state_label")->text().contains("PNG出力完了"),10000);auto folders=QDir(testDir).entryList({"*_png_*"},QDir::Dirs);QCOMPARE(folders.size(),1);auto dir=QDir(testDir+"/"+folders.first());QCOMPARE(dir.entryList({"*.png"},QDir::Files).size(),3);QImage png(dir.filePath("Raw_0_00000000_0_005_z004.png"));QCOMPARE(png.size(),QSize(4,4));QCOMPARE(qGray(png.pixel(0,0)),130);}
    void sourceSafeOutputSuffix(){auto before=QFileInfo(path).size();auto saved=w->saveDisplayedTo(testDir+"/do-not-write.spng",false);QVERIFY(saved.endsWith(".png"));QVERIFY(!QFileInfo::exists(testDir+"/do-not-write.spng"));QCOMPARE(QFileInfo(path).size(),before);}
    void protectSidecarConfiguration(){
        w->check("resize_check")->setChecked(false);w->startProcessing();QTRY_VERIFY(w->processed&&!w->bulkRunning&&!w->loadingImage);
        const QString stem=testDir+"/protected_config";const QByteArray original="{\"instrument_setting\":42}";QFile f(stem+".json");QVERIFY(f.open(QIODevice::WriteOnly));f.write(original);f.close();
        QVERIFY_EXCEPTION_THROWN(w->saveDisplayedTo(stem+".png",false),Error);QVERIFY(!QFileInfo::exists(stem+".png"));QVERIFY(f.open(QIODevice::ReadOnly));QCOMPARE(f.readAll(),original);
    }
    void malformedReadOnlyInputs(){
        QFile original(path);QVERIFY(original.open(QIODevice::ReadOnly));auto bytes=original.readAll();original.close();
        auto testBytes=[&](QString name,QByteArray data,bool valid){QString file=testDir+"/"+name;QFile f(file);QVERIFY(f.open(QIODevice::WriteOnly));f.write(data);f.close();bool threw=false;try{Stack stack(file);stack.read(0);}catch(const Error&){threw=true;}QCOMPARE(threw,!valid);QVERIFY(f.open(QIODevice::ReadOnly));QCOMPARE(f.readAll(),data);};
        testBytes("truncated.spng",bytes.left(bytes.size()-1),false);testBytes("short.spng","abc",false);testBytes("invalid_header.spng",QByteArray::fromHex("2100000000000000")+QByteArray(33,'x'),false);testBytes("zero_missing.spng",QByteArray(8,0)+bytes,true);
        QVERIFY_EXCEPTION_THROWN(bottomLeftRoi(-1,0,2,2,{7,5}),Error);QVERIFY_EXCEPTION_THROWN(bottomLeftRoi(6,0,2,1,{7,5}),Error);
    }
    void mousePickAndParameterControls(){
        w->armPoint(0);auto p=w->view->mapFromScene({4.5,17.5});QTest::mouseClick(w->view->viewport(),Qt::LeftButton,Qt::NoModifier,p);QVERIFY(w->points[0]);QCOMPARE(w->points[0]->x,4.);QCOMPARE(w->points[0]->y,6.);
        w->check("resize_check")->setChecked(false);QCOMPARE(w->filterConfig().scale,1.);w->check("dilate_check")->setChecked(false);QCOMPARE(w->filterConfig().dilation,1);w->combo("z_step")->setCurrentIndex(1);QCOMPARE(w->filterConfig().zStep,2);
    }
    void wheelZoomKeysAndClamp(){
        QApplication::setActiveWindow(w.get());w->view->setFocus();QCoreApplication::processEvents();
        QTest::keyClick(w->view,Qt::Key_Right);QTRY_COMPARE(w->loadedIndex,1);QTest::keyClick(w->view,Qt::Key_PageDown);QTRY_COMPARE(w->loadedIndex,2);
        auto vp=w->view->viewport();auto pos=vp->rect().center();QWheelEvent layer(pos,vp->mapToGlobal(pos),QPoint(),QPoint(0,120),Qt::NoButton,Qt::NoModifier,Qt::NoScrollPhase,false);QApplication::sendEvent(vp,&layer);QTRY_COMPARE(w->loadedIndex,1);
        auto before=w->view->transform().m11();QWheelEvent zoom(pos,vp->mapToGlobal(pos),QPoint(),QPoint(0,120),Qt::NoButton,Qt::ControlModifier,Qt::NoScrollPhase,false);QApplication::sendEvent(vp,&zoom);QVERIFY(std::abs(w->view->transform().m11()-std::min(32.,before*1.2))<1e-8);
        QTest::keyClick(w->view,Qt::Key_1);QCOMPARE(w->view->transform().m11(),1.);w->view->setRoi(QRect(1,2,5,6));QTest::keyClick(w->view,Qt::Key_Escape);QVERIFY(!w->view->roi);
        setPoint(0,1,5,6);setPoint(1,3,15,16);w->check("extend_check")->setChecked(false);selectLayer(0);QTRY_VERIFY(w->loadedIndex==0&&!w->loadingImage);auto expected=linePosition(*w->points[0],*w->points[1],0,false);QCOMPARE(expected,QPointF(5,6));
        QTest::keyClick(w->view,Qt::Key_F);QVERIFY(w->view->fitMode);QVERIFY(!w->check("follow_check")->isChecked());
    }
    void scanExplicitIdsValidationAndDiscovery(){
        auto acquisition=prepareScan();auto settings=acquisition+"/PARAMS/CombinedParam.json";QFile file(settings);QVERIFY(file.open(QIODevice::ReadOnly));auto base=QJsonDocument::fromJson(file.readAll()).object();file.close();
        auto write=[&](QJsonObject object){if(!file.open(QIODevice::WriteOnly))qFatal("Cannot write synthetic settings");file.write(QJsonDocument(object).toJson());file.close();};
        auto section=base["ImageFilterParam"].toObject();auto array=section["CommonParamArray"].toArray();auto a=array[0].toObject(),b=array[1].toObject();a["LayerID"]=1;b["LayerID"]=0;array[0]=a;array[1]=b;section["CommonParamArray"]=array;auto explicitIds=base;explicitIds["ImageFilterParam"]=section;write(explicitIds);QCOMPARE(readSettings(settings,0).config.threshold,7.);QCOMPARE(readSettings(settings,1).config.threshold,9.);QVERIFY_EXCEPTION_THROWN(readSettings(settings,2),Error);
        for(auto pair:{QPair<QString,QJsonValue>{"Expansion",2.5},{"NormCutOffFreq",.9},{"ThrBinary",500},{"WeightZFilter",QJsonValue()}}){auto invalid=base;auto s=base["ImageFilterParam"].toObject();auto values=s["CommonParamArray"].toArray();auto item=values[0].toObject();item[pair.first]=pair.second;values[0]=item;s["CommonParamArray"]=values;invalid["ImageFilterParam"]=s;write(invalid);QVERIFY_EXCEPTION_THROWN(readSettings(settings,0),Error);}
        auto invalid=base;section=base["ImageFilterParam"].toObject();section["Algorithm"]="unknown";invalid["ImageFilterParam"]=section;write(invalid);QVERIFY_EXCEPTION_THROWN(readSettings(settings,0),Error);write(base);
        for(QString container:{"IMAGE","DATA"})QCOMPARE(findParameterFile(acquisition+"/"+container+"/00_00/Raw_0_00000000_0_001.spng"),settings);
        auto other=acquisition+"/another/IMAGE/00_00/Raw_0_00000000_0_001.spng";QDir().mkpath(QFileInfo(other).absolutePath());QVERIFY(findParameterFile(other).isEmpty());QDir().mkpath(acquisition+"/another/PARAMS");QFile misspelled(acquisition+"/another/PARAMS/ConbinedParam.json");QVERIFY(misspelled.open(QIODevice::WriteOnly));misspelled.write(QJsonDocument(base).toJson());misspelled.close();QVERIFY(findParameterFile(other).isEmpty());
    }
    void scanManualAndOverrideScope(){
        auto acquisition=prepareScan();auto p0=acquisition+"/IMAGE/00_00/Raw_0_00000000_0_001.spng";auto p1=acquisition+"/IMAGE/00_00/Raw_0_00000000_1_001.spng";
        QFile base(acquisition+"/PARAMS/CombinedParam.json");QVERIFY(base.open(QIODevice::ReadOnly));auto bytes=base.readAll();base.close();
        w->openPath(p0);QTRY_VERIFY(!w->image.empty()&&!w->loadingImage&&!w->scanPending);w->check("scan_auto")->setChecked(false);w->real("threshold_box")->setValue(20);w->openPath(p1);QTRY_VERIFY(!w->image.empty()&&!w->loadingImage&&!w->scanPending);QCOMPARE(w->real("threshold_box")->value(),20.);
        w->check("scan_auto")->setChecked(true);QTRY_VERIFY(!w->scanPending);QCOMPARE(w->real("threshold_box")->value(),7.);
        auto unknown=writeStack("scan/0000/IMAGE/00_00/unnamed.spng",{constant(8)});w->openPath(unknown);QTRY_VERIFY(!w->image.empty()&&!w->loadingImage&&!w->scanPending);QVERIFY(w->scanError);QCOMPARE(w->spin("scan_layer")->value(),-1);w->spin("scan_layer")->setValue(1);QTRY_VERIFY(!w->scanPending);QVERIFY(!w->scanError);QCOMPARE(w->real("threshold_box")->value(),7.);
        w->openPath(p0);w->openPath(p1);QTRY_VERIFY(!w->image.empty()&&!w->loadingImage&&!w->scanPending);QVERIFY(w->scanSettings);QCOMPARE(w->scanSettings->layer,1);
        w->scanOverride=base.fileName();w->scanOverrideScope=acquisition;auto other=writeStack("another/DATA/00_00/Raw_0_00000000_0_001.spng",{constant(9)});QDir().mkpath(testDir+"/another/PARAMS");QFile copy(testDir+"/another/PARAMS/CombinedParam.json");QVERIFY(copy.open(QIODevice::WriteOnly));copy.write(bytes);copy.close();w->openPath(other);QTRY_VERIFY(!w->image.empty()&&!w->loadingImage&&!w->scanPending);QVERIFY(w->scanOverride.isEmpty());QCOMPARE(w->real("threshold_box")->value(),9.);
    }
    void geometryScreens(){
        auto scale=qEnvironmentVariable("QT_SCALE_FACTOR","1").replace('.','_');
        w->openPath(root+"/tests/screen_fixture/Raw_0_00000000_0_005.spng");QTRY_VERIFY(!w->image.empty()&&!w->loadingImage&&!w->scanPending);normalizeScreenshot();
        for(auto size:{QSize(1100,700),QSize(1280,800),QSize(1920,1080)}){
            w->resize(size);for(int tab=0;tab<4;tab++){w->get<QTabWidget>("feature_tabs")->setCurrentIndex(tab);w->view->setFocus();for(int i=0;i<5;i++)QCoreApplication::processEvents();auto name=QString("%1_%2_%3_%4.png").arg(scale).arg(size.width()).arg(size.height()).arg(tab);auto actual=w->grab().toImage();actual.save(root+"/evidence/cpp_geometry_"+name);compareScreenshot(actual,"geometry_"+name);}
        }
    }
    void initialScreens(){w->close();w.reset();w=std::make_unique<Viewer>();w->setAttribute(Qt::WA_DontShowOnScreen,true);w->show();for(int i=0;i<4;i++){w->get<QTabWidget>("feature_tabs")->setCurrentIndex(i);QCoreApplication::processEvents();auto actual=w->grab().toImage();auto output=root+QString("/evidence/cpp_integrated_initial_%1.png").arg(i);actual.save(output);compareScreenshot(actual,QString("tab_%1.png").arg(i));}}
    void loadedScreens(){
        w->close();w.reset();w=std::make_unique<Viewer>();w->setAttribute(Qt::WA_DontShowOnScreen,true);w->show();
        w->openPath(root+"/tests/screen_fixture/Raw_0_00000000_0_005.spng");QTRY_VERIFY(!w->image.empty()&&!w->loadingImage&&!w->scanPending);normalizeScreenshot();
        auto snap=[&](QString name,int tab){w->get<QTabWidget>("feature_tabs")->setCurrentIndex(tab);w->view->setFocus();for(int i=0;i<5;i++)QCoreApplication::processEvents();w->grab().save(root+"/evidence/cpp_loaded_"+name+".png");compareScreenshot(w->grab().toImage(),"loaded_"+name+".png");QJsonObject labels;for(auto it=w->objects.begin();it!=w->objects.end();++it)if(auto label=qobject_cast<QLabel*>(it.value()))labels[it.key()]=label->text();QFile f(root+"/evidence/cpp_loaded_"+name+".json");QVERIFY(f.open(QIODevice::WriteOnly));f.write(QJsonDocument(labels).toJson());};
        snap("raw",0);w->spin("roi_x")->setValue(32);w->spin("roi_y")->setValue(42);w->spin("roi_w")->setValue(80);w->spin("roi_h")->setValue(54);w->applyRoi();w->cursorPosition({40.1,80.1},true);snap("roi",0);
        selectLayer(1);QTRY_VERIFY(w->loadedIndex==1&&!w->loadingImage);snap("missing",0);selectLayer(2);QTRY_VERIFY(w->loadedIndex==2&&!w->loadingImage);
        w->check("resize_check")->setChecked(true);w->real("scale_box")->setValue(1.4142);w->combo("z_step")->setCurrentIndex(1);w->startProcessing();QTRY_VERIFY(w->processed&&!w->bulkRunning&&!w->loadingImage);
        auto info=w->label("process_info")->text();info.replace(QRegularExpression("完了：[0-9.]+秒"),"完了：0.00秒");w->label("process_info")->setText(info);snap("processed",3);
        w->combo("stage_combo")->setCurrentText("2D filter後");QTRY_VERIFY(!w->loadingImage&&w->loadedIndex==2);snap("highpass",3);
        w->get<QLineEdit>("layer_spec")->setText("0-4");w->startProjection("min");QTRY_VERIFY(w->projection&&!w->bulkRunning);w->check("depth_check")->setChecked(true);w->check("depth_weight")->setChecked(true);w->combo("palette_combo")->setCurrentText("kingfisher");snap("depth",2);
        w->returnToSlice();QTRY_VERIFY(!w->projection&&!w->loadingImage);setPoint(0,0,35,60);setPoint(1,4,150,130);w->view->setZoom(200);snap("follow",1);
    }
};
int main(int argc,char** argv){qputenv("QT_QPA_PLATFORM","windows");QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);QApplication app(argc,argv);app.setFont(QFontDatabase::systemFont(QFontDatabase::GeneralFont));app.setStyle("Fusion");cv::setNumThreads(2);ViewerTests tests;return QTest::qExec(&tests,argc,argv);}
#include "viewer_tests.moc"
