#include "viewer.h"
namespace spng {
static QJsonArray toArray(QVector<int> a){QJsonArray out;for(auto i:a)out.append(i);return out;}
static QJsonValue roiBox(std::optional<QRect> r){return r?QJsonValue(QJsonArray{r->x(),r->y(),r->x()+r->width(),r->y()+r->height()}):QJsonValue();}
static QJsonValue roiBottom(std::optional<QRect> r,QSize size){return r?QJsonValue(QJsonArray{r->x(),size.height()-r->y()-r->height(),r->width(),r->height()}):QJsonValue();}
static QString pngPath(QString path){QFileInfo p(path);return p.suffix().compare("png",Qt::CaseInsensitive)==0?path:p.dir().filePath(p.completeBaseName()+".png");}
QJsonObject Viewer::processingMetadata()const{return processed&&(projection?projection->source:stage())!="元画像"?processed->metadata:QJsonObject{};}
QString Viewer::saveDisplayedTo(QString path,bool crop){
    if(image.empty()||loadingImage||(!projection&&loadedIndex!=spin("layer_number")->value()))throw Error("表示画像の読み込み完了を待ってください。");
    auto roi=crop?view->roi:std::optional<QRect>{};if(crop&&!roi)throw Error("範囲を選択してください。");path=pngPath(path);bool needMetadata=bool(projection)||stage()!="元画像";
    QString sidecar=QFileInfo(path).dir().filePath(QFileInfo(path).completeBaseName()+".json");
    if(needMetadata&&QFileInfo::exists(sidecar)){
        QFile existing(sidecar);if(!existing.open(QIODevice::ReadOnly)||existing.size()>16*1024*1024)throw Error("同名のJSONがあります。別の出力名を指定してください。");
        auto old=QJsonDocument::fromJson(existing.readAll()).object();if(!old.contains("input_spng")||!old.contains("display_stage")||!old.contains("ROI_top_left_box")||!old.contains("processing"))throw Error("同名のJSONはビューアの処理記録ではありません。元ファイルを保護するため、別の出力名を指定してください。");
    }
    auto saved=savePng(image,path,roi);
    if(needMetadata){QJsonValue proj;if(projection)proj=QJsonObject{{"kind",projection->kind},{"requested",toArray(projection->requested)},{"used",toArray(projection->used)},{"depth_colored",check("depth_check")->isChecked()},{"colormap",combo("palette_combo")->currentData().toString()},{"brightness_weighted",check("depth_weight")->isChecked()},{"ties","lowest layer index"}};
        QJsonObject meta={{"input_spng",currentPath},{"display_stage",projection?projection->source:stage()},{"layer",projection?QJsonValue():QJsonValue(loadedIndex)},{"projection",proj},{"ROI_top_left_box",roiBox(roi)},{"ROI_bottom_left_xywh",roiBottom(roi,image.size())},{"processing",processingMetadata().isEmpty()?QJsonValue():QJsonValue(processingMetadata())}};
        QSaveFile f(sidecar);if(!f.open(QIODevice::WriteOnly))throw Error(f.errorString());auto bytes=QJsonDocument(meta).toJson();if(f.write(bytes)!=bytes.size()||!f.commit())throw Error(f.errorString());
    }
    lastSaveDir=QFileInfo(saved).absolutePath();label("state_label")->setText((needMetadata?"PNGと処理記録を保存しました：":"元の画素値でPNG保存しました：")+QDir::toNativeSeparators(saved));return saved;
}
void Viewer::saveDialog(bool crop){
    if(image.empty()||loadingImage||(!projection&&loadedIndex!=spin("layer_number")->value()))return;auto roi=crop?view->roi:std::optional<QRect>{};if(crop&&!roi)return;
    QString name=QFileInfo(currentPath).completeBaseName();bool derived=bool(projection)||stage()!="元画像";
    if(projection){name+=QString("_%1_z%2-%3").arg(projection->kind).arg(projection->requested.first()).arg(projection->requested.last());if(check("depth_check")->isChecked())name+="_color";}
    else{if(stage()!="元画像")name+="_"+QString(stage()).replace(' ','_');name+=QString("_z%1").arg(loadedIndex,3,10,QChar('0'));}
    if(roi)name+=QString("_x%1_y%2_w%3_h%4").arg(roi->x()).arg(image.size().height()-roi->y()-roi->height()).arg(roi->width()).arg(roi->height());
    auto path=QFileDialog::getSaveFileName(this,derived?"表示データをPNG保存":crop?"選択範囲をPNG保存":"表示している層をPNG保存",QDir(lastSaveDir).filePath(name+".png"),"PNG画像 (*.png)");if(path.isEmpty())return;auto target=pngPath(path);if(target!=path&&QFileInfo::exists(target)&&QMessageBox::question(this,"上書き確認",QFileInfo(target).fileName()+" は存在します。上書きしますか？")!=QMessageBox::Yes)return;
    try{saveDisplayedTo(target,crop);}catch(const std::exception& e){QMessageBox::warning(this,derived?"保存エラー":"保存できませんでした",QString::fromUtf8(e.what()));}
}
void Viewer::startExport(){if(!stack)return;QVector<int> indices;try{indices=parseLayers(get<QLineEdit>("layer_spec")->text(),int(stack->frames.size()));}catch(const std::exception&e){QMessageBox::warning(this,"層の指定",QString::fromUtf8(e.what()));return;}auto roi=check("batch_crop")->isChecked()?view->roi:std::optional<QRect>{};if(check("batch_crop")->isChecked()&&!roi){QMessageBox::information(this,"範囲未選択","画像上で範囲を選択してください。");return;}auto parent=QFileDialog::getExistingDirectory(this,"PNG出力先（中に専用フォルダを作ります）",lastSaveDir);if(parent.isEmpty())return;auto read=reader();auto source=stage();auto stem=QFileInfo(currentPath).completeBaseName();auto folder=QDir(parent).filePath(stem+"_png_"+QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz")+"_"+QUuid::createUuid().toString(QUuid::Id128).left(6));
    QJsonObject meta={{"input_spng",currentPath},{"stage",source},{"processed",source!="元画像"&&processed?QJsonValue(processed->metadata):QJsonValue()},{"coordinate_origin","bottom-left"},{"crop_bottom_left_xywh",image.empty()?QJsonValue():roiBottom(roi,image.size())}};
    beginBulk("PNGを書き出し中…",[read,indices,folder,stem,source,roi,meta](Progress p,Cancel c){return exportLayers(read,indices,folder,stem,source,roi,meta,p,c);},[this,parent](QJsonObject result){lastSaveDir=parent;label("state_label")->setText(QString("PNG出力完了：%1層、欠損%2層　%3").arg(result["saved"].toInt()).arg(result["missing"].toArray().size()).arg(QDir::toNativeSeparators(result["folder"].toString())));});
}
}
