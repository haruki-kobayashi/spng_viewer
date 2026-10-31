#include "viewer.h"
namespace spng {
void Viewer::bindControls(){
    auto openFile=[this]{auto p=QFileDialog::getOpenFileName(this,"spngファイルを開く",lastOpenDir,"SPNG画像 (*.spng *.SPNG);;すべてのファイル (*)");if(!p.isEmpty())openPath(p);};
    auto openFolder=[this]{auto p=QFileDialog::getExistingDirectory(this,"DATA / IMAGE / センサーフォルダを開く",lastOpenDir);if(!p.isEmpty())openPath(p);};
    auto fit=[this]{check("follow_check")->setChecked(false);view->fitAll();};
    for(auto object:objects){if(auto a=qobject_cast<QAction*>(object)){
        auto t=a->text(),shortcut=a->shortcut().toString();
        if(t=="ファイルを開く")connect(a,&QAction::triggered,this,openFile);
        else if(t=="フォルダを開く")connect(a,&QAction::triggered,this,openFolder);
        else if(t=="表示層をPNG保存")connect(a,&QAction::triggered,this,[this]{saveDialog(false);});
        else if(t=="選択範囲をPNG保存")connect(a,&QAction::triggered,this,[this]{saveDialog(true);});
        else if(t=="全体表示")connect(a,&QAction::triggered,this,fit);
        else if(t=="100%")connect(a,&QAction::triggered,view,&ImageView::actualSize);
        else if(t=="使い方")connect(a,&QAction::triggered,this,&Viewer::showHelp);
        else if(shortcut=="Left"||shortcut=="PgUp")connect(a,&QAction::triggered,this,[this]{stepLayer(-1);});
        else if(shortcut=="Right"||shortcut=="PgDown")connect(a,&QAction::triggered,this,[this]{stepLayer(1);});
        else if(shortcut=="Esc")connect(a,&QAction::triggered,this,[this]{view->setRoi({});});
    }else if(auto b=qobject_cast<QPushButton*>(object)){
        auto t=b->text();
        if(t=="座標から範囲を指定")connect(b,&QPushButton::clicked,this,&Viewer::applyRoi);
        else if(t=="解除")connect(b,&QPushButton::clicked,this,[this]{view->setRoi({});});
        else if(t=="選択範囲へ拡大")connect(b,&QPushButton::clicked,view,&ImageView::fitRoi);
        else if(t=="範囲をPNG保存")connect(b,&QPushButton::clicked,this,[this]{saveDialog(true);});
        else if(t=="表示層をPNG保存")connect(b,&QPushButton::clicked,this,[this]{saveDialog(false);});
        else if(t=="元の階調へ戻す")connect(b,&QPushButton::clicked,this,&Viewer::resetLevels);
        else if(t=="全層を先読み")connect(b,&QPushButton::clicked,this,[this]{startPreload();});
        else if(t=="再生範囲を2点の層に合わせる")connect(b,&QPushButton::clicked,this,&Viewer::rangeToPoints);
        else if(t=="最大値を表示")connect(b,&QPushButton::clicked,this,[this]{startProjection("max");});
        else if(t=="最小値を表示")connect(b,&QPushButton::clicked,this,[this]{startProjection("min");});
        else if(t=="通常の層表示へ戻る")connect(b,&QPushButton::clicked,this,&Viewer::returnToSlice);
        else if(t=="保存先を選んで一括出力")connect(b,&QPushButton::clicked,this,&Viewer::startExport);
        else if(t=="設定JSONを選択")connect(b,&QPushButton::clicked,this,&Viewer::chooseScanParameters);
        else if(t=="自動検出をやり直す")connect(b,&QPushButton::clicked,this,&Viewer::resetScanParameters);
        else if(t=="設定を現在のspngの全層へ適用")connect(b,&QPushButton::clicked,this,&Viewer::startProcessing);
        else if(t=="▶ 再生")connect(b,&QPushButton::clicked,this,&Viewer::togglePlay);
        else if(t=="◀ 前の層")connect(b,&QPushButton::clicked,this,[this]{stepLayer(-1);});
        else if(t=="次の層 ▶")connect(b,&QPushButton::clicked,this,[this]{stepLayer(1);});
        else if(t=="中止")connect(b,&QPushButton::clicked,this,&Viewer::cancelBulk);
        else if(t=="画像上をクリックして指定"||t=="入力した座標を登録"||t=="この点をリセット"){
            int tag=-1;for(auto p=b->parentWidget();p;p=p->parentWidget())if(auto g=qobject_cast<QGroupBox*>(p)){if(g->title()=="点1")tag=0;else if(g->title()=="点2")tag=1;if(tag>=0)break;}
            if(tag<0)throw Error("追従点の画面部品が見つかりません。");
            connect(b,&QPushButton::clicked,this,[this,t,tag]{if(t=="画像上をクリックして指定")armPoint(tag);else if(t=="入力した座標を登録")registerPoint(tag);else resetPoint(tag);});
        }else throw Error("未接続のボタン: "+t);
    }}
    connect(combo("container_combo"),&QComboBox::currentIndexChanged,this,[this]{containerChanged();});
    connect(combo("sensor_combo"),&QComboBox::currentIndexChanged,this,[this]{sensorChanged();});
    connect(combo("layer_combo"),&QComboBox::currentIndexChanged,this,[this]{layerChanged();});
    connect(combo("kind_combo"),&QComboBox::currentIndexChanged,this,[this]{kindChanged();});
    connect(combo("view_combo"),&QComboBox::currentIndexChanged,this,[this]{viewChanged();});
    connect(combo("view_combo")->lineEdit(),&QLineEdit::returnPressed,this,&Viewer::typedView);
    connect(combo("file_combo"),&QComboBox::currentIndexChanged,this,[this]{fileChanged();});
    connect(view,&ImageView::roiChanged,this,&Viewer::roiChanged);connect(view,&ImageView::cursorChanged,this,&Viewer::cursorPosition);connect(view,&ImageView::layerStep,this,&Viewer::stepLayer);
    connect(view,&ImageView::zoomChanged,this,[this](double z){label("zoom_label")->setText(QString("  %1% ").arg(z,0,'f',0));centerFollow();});
    connect(view,&ImageView::pointPicked,this,&Viewer::pickedPoint);connect(view,&ImageView::viewportResized,this,[this]{QTimer::singleShot(0,this,&Viewer::centerFollow);});
    connect(get<QSlider>("layer_slider"),&QSlider::valueChanged,this,[this](int i){{QSignalBlocker b(spin("layer_number"));spin("layer_number")->setValue(i);}requestLayer();});
    connect(spin("layer_number"),&QSpinBox::valueChanged,this,[this](int i){{QSignalBlocker b(get<QSlider>("layer_slider"));get<QSlider>("layer_slider")->setValue(i);}requestLayer();});
    connect(real("low"),&QDoubleSpinBox::valueChanged,this,[this]{refreshDisplay();});connect(real("high"),&QDoubleSpinBox::valueChanged,this,[this]{refreshDisplay();});connect(check("invert"),&QCheckBox::toggled,this,[this]{refreshDisplay();});
    connect(check("follow_check"),&QCheckBox::toggled,this,[this]{centerFollow();});connect(check("extend_check"),&QCheckBox::toggled,this,[this]{centerFollow();});
    connect(combo("stage_combo"),&QComboBox::currentTextChanged,this,[this]{stageChanged();});
    connect(check("depth_check"),&QCheckBox::toggled,this,[this]{renderProjection();});connect(check("depth_weight"),&QCheckBox::toggled,this,[this]{renderProjection();});connect(combo("palette_combo"),&QComboBox::currentIndexChanged,this,[this]{renderProjection();});
    connect(check("resize_check"),&QCheckBox::toggled,real("scale_box"),&QWidget::setEnabled);connect(check("resize_check"),&QCheckBox::toggled,combo("interp_combo"),&QWidget::setEnabled);connect(check("dilate_check"),&QCheckBox::toggled,spin("dilate_box"),&QWidget::setEnabled);
    connect(check("scan_auto"),&QCheckBox::toggled,this,[this](bool on){if(on)reloadScanParameters();else{invalidate({"params"});scanPending=false;scanError=false;label("scan_info")->setText("手動設定。現在の入力値を使用します。");updateControls();}});
    connect(spin("scan_layer"),&QSpinBox::valueChanged,this,[this]{reloadScanParameters();});
}
void Viewer::updateControls(){
    bool hasImage=!image.empty()&&!loadingImage;get<QAction>("full_action")->setEnabled(hasImage);button("apply_roi_button")->setEnabled(hasImage);get<QAction>("crop_action")->setEnabled(hasImage&&bool(view->roi));button("crop_button")->setEnabled(hasImage&&bool(view->roi));
    get<QSlider>("layer_slider")->setEnabled(bool(stack));spin("layer_number")->setEnabled(bool(stack));button("prev_button")->setEnabled(stack&&spin("layer_number")->value()>0);button("next_button")->setEnabled(stack&&spin("layer_number")->value()<stack->frames.size()-1);
    bool ready=bool(stack)&&!bulkRunning;for(auto n:{"play_button","preload_button","process_button","max_button","min_button","export_button"})button(n)->setEnabled(ready);combo("stage_combo")->setEnabled(ready);
    if(check("scan_auto")->isChecked())button("process_button")->setEnabled(ready&&!scanPending&&!scanError);
    get<QWidget>("processing_page")->setEnabled(!bulkRunning);bool fixed=projection&&check("depth_check")->isChecked();real("low")->setEnabled(!fixed);real("high")->setEnabled(!fixed);check("invert")->setEnabled(!fixed);
    if(bulkRunning){get<QAction>("full_action")->setEnabled(false);get<QAction>("crop_action")->setEnabled(false);button("crop_button")->setEnabled(false);button("apply_roi_button")->setEnabled(false);}
}
void Viewer::showHelp(){QMessageBox::information(this,"SPNG Viewer の使い方",
    "1. spngファイル、DATA / IMAGE、またはセンサーフォルダを開きます。\n"
    "2. フォルダを開いた場合はセンサー・Layer・画像種別・Viewを選びます。Viewは番号を入力してEnterでも切り替えられます。\n"
    "3. ホイール・矢印・スライダーで層を切り替えます。層番号はファイル中の0始まりの番号です。\n"
    "4. Ctrl＋左ドラッグ、またはX・Y・幅・高さの入力で範囲を指定します。X・Yは左下原点のpixel座標です。\n"
    "5. 表示層全体、または範囲内だけをPNGに保存できます。選択した画像データの画素値が保存され、枠や表示濃淡は入りません。\n\n"
    "左下pixelの座標は(0,0)、左上は(0,高さ−1)です。PNGの見た目の上下は反転しません。\n"
    "Ctrl＋ホイールで拡大縮小、左ドラッグで移動、Fで全体表示、1で100%表示、Escで選択解除。\n"
    "初期状態では0-255、16bit画像は0-65535、0/1画像は0-1を表示範囲にします。保存は元の階調です。\n\n"
    "各タブの機能：再生・追従タブで自動再生・ループ・2点による中心追従。重ね合わせ・出力タブで任意の層の最大/最小合成、層番号色分け、複数PNGの出力。画像処理タブでLayer別のスキャン設定を読み込み、全層へフィルタを適用して段階ごとに表示できます。");}
}
