// SPDX-License-Identifier: MIT
// Generated from the reference Qt widget tree. Behavior is in Viewer.
#include "ui_generated.h"
#include "imageview.h"
#include "ops.h"
#include <QtWidgets>
namespace spng {
void buildReferenceUi(QMainWindow* win,QHash<QString,QObject*>& objects){
  auto* w_001=new QSplitter();
  objects[QString::fromUtf8("w_001")]=w_001; w_001->setObjectName(QString::fromUtf8("w_001"));
  w_001->setEnabled(true);
  w_001->setMinimumSize(0,0); w_001->setMaximumSize(16777215,16777215);
  w_001->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(5)));
  w_001->setOrientation(Qt::Orientation(1));
  auto* feature_tabs=new QTabWidget();
  objects[QString::fromUtf8("feature_tabs")]=feature_tabs; feature_tabs->setObjectName(QString::fromUtf8("feature_tabs"));
  feature_tabs->setEnabled(true);
  feature_tabs->setMinimumSize(420,0); feature_tabs->setMaximumSize(16777215,16777215);
  feature_tabs->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)));
  auto* w_003=new QScrollArea();
  objects[QString::fromUtf8("w_003")]=w_003; w_003->setObjectName(QString::fromUtf8("w_003"));
  w_003->setEnabled(true);
  w_003->setMinimumSize(400,0); w_003->setMaximumSize(16777215,16777215);
  w_003->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)));
  auto* w_004=new QWidget();
  objects[QString::fromUtf8("w_004")]=w_004; w_004->setObjectName(QString::fromUtf8("w_004"));
  w_004->setEnabled(true);
  w_004->setMinimumSize(0,0); w_004->setMaximumSize(16777215,16777215);
  w_004->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  auto* layout_005=new QVBoxLayout();
  layout_005->setContentsMargins(12,12,12,12); layout_005->setSpacing(12);
  auto* w_006=new QLabel();
  objects[QString::fromUtf8("w_006")]=w_006; w_006->setObjectName(QString::fromUtf8("w_006"));
  w_006->setEnabled(true);
  w_006->setStyleSheet(QString::fromUtf8("font-size:21px;font-weight:600;color:#174b69"));
  w_006->setMinimumSize(0,0); w_006->setMaximumSize(16777215,16777215);
  w_006->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_006->setText(QString::fromUtf8("SPNG Viewer")); w_006->setWordWrap(false); w_006->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_006->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_006->setSizePolicy(policy); }
  layout_005->addWidget(w_006);
  layout_005->setStretch(0,0);
  auto* source_label=new QLabel();
  objects[QString::fromUtf8("source_label")]=source_label; source_label->setObjectName(QString::fromUtf8("source_label"));
  source_label->setEnabled(true);
  source_label->setMinimumSize(0,0); source_label->setMaximumSize(16777215,16777215);
  source_label->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  source_label->setText(QString::fromUtf8("spngファイル、またはDATA / IMAGEフォルダを開いてください。")); source_label->setWordWrap(true); source_label->setTextInteractionFlags(Qt::TextInteractionFlags(1)); source_label->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); source_label->setSizePolicy(policy); }
  layout_005->addWidget(source_label);
  layout_005->setStretch(1,0);
  auto* w_008=new QGroupBox();
  objects[QString::fromUtf8("w_008")]=w_008; w_008->setObjectName(QString::fromUtf8("w_008"));
  w_008->setEnabled(true);
  w_008->setMinimumSize(0,0); w_008->setMaximumSize(16777215,16777215);
  w_008->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_008->setTitle(QString::fromUtf8("画像の選択"));
  auto* layout_009=new QFormLayout();
  layout_009->setContentsMargins(9,9,9,9); layout_009->setSpacing(6);
  layout_009->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_009->setFormAlignment(Qt::Alignment(33)); layout_009->setLabelAlignment(Qt::Alignment(1));
  auto* w_010=new QLabel();
  objects[QString::fromUtf8("w_010")]=w_010; w_010->setObjectName(QString::fromUtf8("w_010"));
  w_010->setEnabled(true);
  w_010->setMinimumSize(0,0); w_010->setMaximumSize(16777215,16777215);
  w_010->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_010->setText(QString::fromUtf8("画像フォルダ")); w_010->setWordWrap(false); w_010->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_010->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_010->setSizePolicy(policy); }
  layout_009->setWidget(0,QFormLayout::ItemRole(0),w_010);
  auto* container_combo=new QComboBox();
  objects[QString::fromUtf8("container_combo")]=container_combo; container_combo->setObjectName(QString::fromUtf8("container_combo"));
  container_combo->setEnabled(true);
  container_combo->setStyleSheet(QString::fromUtf8("QComboBox { combobox-popup: 0; }"));
  container_combo->setMinimumSize(0,0); container_combo->setMaximumSize(16777215,16777215);
  container_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  container_combo->setEditable(false); container_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); container_combo->setMaxVisibleItems(12); container_combo->setMinimumContentsLength(10); container_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(2)); container_combo->setIconSize(QSize(16,16));
  container_combo->setCurrentIndex(-1);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); container_combo->setSizePolicy(policy); }
  layout_009->setWidget(0,QFormLayout::ItemRole(1),container_combo);
  auto* w_012=new QLabel();
  objects[QString::fromUtf8("w_012")]=w_012; w_012->setObjectName(QString::fromUtf8("w_012"));
  w_012->setEnabled(true);
  w_012->setMinimumSize(0,0); w_012->setMaximumSize(16777215,16777215);
  w_012->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_012->setText(QString::fromUtf8("センサー")); w_012->setWordWrap(false); w_012->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_012->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_012->setSizePolicy(policy); }
  layout_009->setWidget(1,QFormLayout::ItemRole(0),w_012);
  auto* sensor_combo=new QComboBox();
  objects[QString::fromUtf8("sensor_combo")]=sensor_combo; sensor_combo->setObjectName(QString::fromUtf8("sensor_combo"));
  sensor_combo->setEnabled(true);
  sensor_combo->setStyleSheet(QString::fromUtf8("QComboBox { combobox-popup: 0; }"));
  sensor_combo->setMinimumSize(0,0); sensor_combo->setMaximumSize(16777215,16777215);
  sensor_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  sensor_combo->setEditable(false); sensor_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); sensor_combo->setMaxVisibleItems(12); sensor_combo->setMinimumContentsLength(10); sensor_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(2)); sensor_combo->setIconSize(QSize(16,16));
  sensor_combo->setCurrentIndex(-1);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); sensor_combo->setSizePolicy(policy); }
  layout_009->setWidget(1,QFormLayout::ItemRole(1),sensor_combo);
  auto* w_014=new QLabel();
  objects[QString::fromUtf8("w_014")]=w_014; w_014->setObjectName(QString::fromUtf8("w_014"));
  w_014->setEnabled(true);
  w_014->setMinimumSize(0,0); w_014->setMaximumSize(16777215,16777215);
  w_014->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_014->setText(QString::fromUtf8("Layer")); w_014->setWordWrap(false); w_014->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_014->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_014->setSizePolicy(policy); }
  layout_009->setWidget(2,QFormLayout::ItemRole(0),w_014);
  auto* layer_combo=new QComboBox();
  objects[QString::fromUtf8("layer_combo")]=layer_combo; layer_combo->setObjectName(QString::fromUtf8("layer_combo"));
  layer_combo->setEnabled(true);
  layer_combo->setStyleSheet(QString::fromUtf8("QComboBox { combobox-popup: 0; }"));
  layer_combo->setMinimumSize(0,0); layer_combo->setMaximumSize(16777215,16777215);
  layer_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  layer_combo->setEditable(false); layer_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); layer_combo->setMaxVisibleItems(12); layer_combo->setMinimumContentsLength(10); layer_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(2)); layer_combo->setIconSize(QSize(16,16));
  layer_combo->setCurrentIndex(-1);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); layer_combo->setSizePolicy(policy); }
  layout_009->setWidget(2,QFormLayout::ItemRole(1),layer_combo);
  auto* w_016=new QLabel();
  objects[QString::fromUtf8("w_016")]=w_016; w_016->setObjectName(QString::fromUtf8("w_016"));
  w_016->setEnabled(true);
  w_016->setMinimumSize(0,0); w_016->setMaximumSize(16777215,16777215);
  w_016->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_016->setText(QString::fromUtf8("画像種別")); w_016->setWordWrap(false); w_016->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_016->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_016->setSizePolicy(policy); }
  layout_009->setWidget(3,QFormLayout::ItemRole(0),w_016);
  auto* kind_combo=new QComboBox();
  objects[QString::fromUtf8("kind_combo")]=kind_combo; kind_combo->setObjectName(QString::fromUtf8("kind_combo"));
  kind_combo->setEnabled(true);
  kind_combo->setStyleSheet(QString::fromUtf8("QComboBox { combobox-popup: 0; }"));
  kind_combo->setMinimumSize(0,0); kind_combo->setMaximumSize(16777215,16777215);
  kind_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  kind_combo->setEditable(false); kind_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); kind_combo->setMaxVisibleItems(12); kind_combo->setMinimumContentsLength(10); kind_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(2)); kind_combo->setIconSize(QSize(16,16));
  kind_combo->setCurrentIndex(-1);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); kind_combo->setSizePolicy(policy); }
  layout_009->setWidget(3,QFormLayout::ItemRole(1),kind_combo);
  auto* w_018=new QLabel();
  objects[QString::fromUtf8("w_018")]=w_018; w_018->setObjectName(QString::fromUtf8("w_018"));
  w_018->setEnabled(true);
  w_018->setMinimumSize(0,0); w_018->setMaximumSize(16777215,16777215);
  w_018->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_018->setText(QString::fromUtf8("View（入力可）")); w_018->setWordWrap(false); w_018->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_018->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_018->setSizePolicy(policy); }
  layout_009->setWidget(4,QFormLayout::ItemRole(0),w_018);
  auto* view_combo=new QComboBox();
  objects[QString::fromUtf8("view_combo")]=view_combo; view_combo->setObjectName(QString::fromUtf8("view_combo"));
  view_combo->setEnabled(true);
  view_combo->setStyleSheet(QString::fromUtf8("QComboBox { combobox-popup: 0; }"));
  view_combo->setMinimumSize(0,0); view_combo->setMaximumSize(16777215,16777215);
  view_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  view_combo->setEditable(true); view_combo->setInsertPolicy(QComboBox::InsertPolicy(0)); view_combo->setMaxVisibleItems(12); view_combo->setMinimumContentsLength(10); view_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(2)); view_combo->setIconSize(QSize(16,16));
  view_combo->setCurrentIndex(-1);
  view_combo->completer()->setFilterMode(Qt::MatchFlags(1));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); view_combo->setSizePolicy(policy); }
  layout_009->setWidget(4,QFormLayout::ItemRole(1),view_combo);
  auto* w_020=new QLabel();
  objects[QString::fromUtf8("w_020")]=w_020; w_020->setObjectName(QString::fromUtf8("w_020"));
  w_020->setEnabled(true);
  w_020->setMinimumSize(0,0); w_020->setMaximumSize(16777215,16777215);
  w_020->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_020->setText(QString::fromUtf8("ファイル")); w_020->setWordWrap(false); w_020->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_020->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_020->setSizePolicy(policy); }
  layout_009->setWidget(5,QFormLayout::ItemRole(0),w_020);
  auto* file_combo=new QComboBox();
  objects[QString::fromUtf8("file_combo")]=file_combo; file_combo->setObjectName(QString::fromUtf8("file_combo"));
  file_combo->setEnabled(true);
  file_combo->setStyleSheet(QString::fromUtf8("QComboBox { combobox-popup: 0; }"));
  file_combo->setMinimumSize(0,0); file_combo->setMaximumSize(16777215,16777215);
  file_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  file_combo->setEditable(false); file_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); file_combo->setMaxVisibleItems(12); file_combo->setMinimumContentsLength(10); file_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(2)); file_combo->setIconSize(QSize(16,16));
  file_combo->setCurrentIndex(-1);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); file_combo->setSizePolicy(policy); }
  layout_009->setWidget(5,QFormLayout::ItemRole(1),file_combo);
  w_008->setLayout(layout_009);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_008->setSizePolicy(policy); }
  layout_005->addWidget(w_008);
  layout_005->setStretch(2,0);
  auto* meta=new QLabel();
  objects[QString::fromUtf8("meta")]=meta; meta->setObjectName(QString::fromUtf8("meta"));
  meta->setEnabled(true);
  meta->setMinimumSize(0,0); meta->setMaximumSize(16777215,16777215);
  meta->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  meta->setText(QString::fromUtf8("画像なし")); meta->setWordWrap(true); meta->setTextInteractionFlags(Qt::TextInteractionFlags(1)); meta->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); meta->setSizePolicy(policy); }
  layout_005->addWidget(meta);
  layout_005->setStretch(3,0);
  auto* w_023=new QGroupBox();
  objects[QString::fromUtf8("w_023")]=w_023; w_023->setObjectName(QString::fromUtf8("w_023"));
  w_023->setEnabled(true);
  w_023->setMinimumSize(0,0); w_023->setMaximumSize(16777215,16777215);
  w_023->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_023->setTitle(QString::fromUtf8("切り出し範囲［pixel］"));
  auto* layout_024=new QFormLayout();
  layout_024->setContentsMargins(9,9,9,9); layout_024->setSpacing(6);
  layout_024->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_024->setFormAlignment(Qt::Alignment(33)); layout_024->setLabelAlignment(Qt::Alignment(1));
  auto* w_025=new QLabel();
  objects[QString::fromUtf8("w_025")]=w_025; w_025->setObjectName(QString::fromUtf8("w_025"));
  w_025->setEnabled(true);
  w_025->setMinimumSize(0,0); w_025->setMaximumSize(16777215,16777215);
  w_025->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_025->setText(QString::fromUtf8("原点：左下　／　X → 右、Y → 上\nX・Yは範囲の左下。幅・高さは画素数。")); w_025->setWordWrap(true); w_025->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_025->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_025->setSizePolicy(policy); }
  layout_024->setWidget(0,QFormLayout::ItemRole(2),w_025);
  auto* w_026=new QLabel();
  objects[QString::fromUtf8("w_026")]=w_026; w_026->setObjectName(QString::fromUtf8("w_026"));
  w_026->setEnabled(true);
  w_026->setMinimumSize(0,0); w_026->setMaximumSize(16777215,16777215);
  w_026->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_026->setText(QString::fromUtf8("X")); w_026->setWordWrap(false); w_026->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_026->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_026->setSizePolicy(policy); }
  layout_024->setWidget(1,QFormLayout::ItemRole(0),w_026);
  auto* roi_x=new QSpinBox();
  objects[QString::fromUtf8("roi_x")]=roi_x; roi_x->setObjectName(QString::fromUtf8("roi_x"));
  roi_x->setEnabled(true);
  roi_x->setMinimumSize(0,0); roi_x->setMaximumSize(16777215,16777215);
  roi_x->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  roi_x->setRange(0,1000000); roi_x->setSingleStep(1); roi_x->setValue(0); roi_x->setKeyboardTracking(false);
  roi_x->setPrefix(QString::fromUtf8("")); roi_x->setSuffix(QString::fromUtf8("")); roi_x->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); roi_x->setSizePolicy(policy); }
  layout_024->setWidget(1,QFormLayout::ItemRole(1),roi_x);
  auto* w_028=new QLabel();
  objects[QString::fromUtf8("w_028")]=w_028; w_028->setObjectName(QString::fromUtf8("w_028"));
  w_028->setEnabled(true);
  w_028->setMinimumSize(0,0); w_028->setMaximumSize(16777215,16777215);
  w_028->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_028->setText(QString::fromUtf8("Y")); w_028->setWordWrap(false); w_028->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_028->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_028->setSizePolicy(policy); }
  layout_024->setWidget(2,QFormLayout::ItemRole(0),w_028);
  auto* roi_y=new QSpinBox();
  objects[QString::fromUtf8("roi_y")]=roi_y; roi_y->setObjectName(QString::fromUtf8("roi_y"));
  roi_y->setEnabled(true);
  roi_y->setMinimumSize(0,0); roi_y->setMaximumSize(16777215,16777215);
  roi_y->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  roi_y->setRange(0,1000000); roi_y->setSingleStep(1); roi_y->setValue(0); roi_y->setKeyboardTracking(false);
  roi_y->setPrefix(QString::fromUtf8("")); roi_y->setSuffix(QString::fromUtf8("")); roi_y->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); roi_y->setSizePolicy(policy); }
  layout_024->setWidget(2,QFormLayout::ItemRole(1),roi_y);
  auto* w_030=new QLabel();
  objects[QString::fromUtf8("w_030")]=w_030; w_030->setObjectName(QString::fromUtf8("w_030"));
  w_030->setEnabled(true);
  w_030->setMinimumSize(0,0); w_030->setMaximumSize(16777215,16777215);
  w_030->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_030->setText(QString::fromUtf8("幅")); w_030->setWordWrap(false); w_030->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_030->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_030->setSizePolicy(policy); }
  layout_024->setWidget(3,QFormLayout::ItemRole(0),w_030);
  auto* roi_w=new QSpinBox();
  objects[QString::fromUtf8("roi_w")]=roi_w; roi_w->setObjectName(QString::fromUtf8("roi_w"));
  roi_w->setEnabled(true);
  roi_w->setMinimumSize(0,0); roi_w->setMaximumSize(16777215,16777215);
  roi_w->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  roi_w->setRange(1,1000000); roi_w->setSingleStep(1); roi_w->setValue(1); roi_w->setKeyboardTracking(false);
  roi_w->setPrefix(QString::fromUtf8("")); roi_w->setSuffix(QString::fromUtf8("")); roi_w->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); roi_w->setSizePolicy(policy); }
  layout_024->setWidget(3,QFormLayout::ItemRole(1),roi_w);
  auto* w_032=new QLabel();
  objects[QString::fromUtf8("w_032")]=w_032; w_032->setObjectName(QString::fromUtf8("w_032"));
  w_032->setEnabled(true);
  w_032->setMinimumSize(0,0); w_032->setMaximumSize(16777215,16777215);
  w_032->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_032->setText(QString::fromUtf8("高さ")); w_032->setWordWrap(false); w_032->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_032->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_032->setSizePolicy(policy); }
  layout_024->setWidget(4,QFormLayout::ItemRole(0),w_032);
  auto* roi_h=new QSpinBox();
  objects[QString::fromUtf8("roi_h")]=roi_h; roi_h->setObjectName(QString::fromUtf8("roi_h"));
  roi_h->setEnabled(true);
  roi_h->setMinimumSize(0,0); roi_h->setMaximumSize(16777215,16777215);
  roi_h->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  roi_h->setRange(1,1000000); roi_h->setSingleStep(1); roi_h->setValue(1); roi_h->setKeyboardTracking(false);
  roi_h->setPrefix(QString::fromUtf8("")); roi_h->setSuffix(QString::fromUtf8("")); roi_h->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); roi_h->setSizePolicy(policy); }
  layout_024->setWidget(4,QFormLayout::ItemRole(1),roi_h);
  auto* layout_034=new QHBoxLayout();
  layout_034->setContentsMargins(0,0,0,0); layout_034->setSpacing(6);
  auto* apply_roi_button=new QPushButton();
  objects[QString::fromUtf8("apply_roi_button")]=apply_roi_button; apply_roi_button->setObjectName(QString::fromUtf8("apply_roi_button"));
  apply_roi_button->setEnabled(false);
  apply_roi_button->setMinimumSize(0,0); apply_roi_button->setMaximumSize(16777215,16777215);
  apply_roi_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  apply_roi_button->setText(QString::fromUtf8("座標から範囲を指定")); apply_roi_button->setCheckable(false); apply_roi_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); apply_roi_button->setSizePolicy(policy); }
  layout_034->addWidget(apply_roi_button);
  layout_034->setStretch(0,0);
  auto* w_036=new QPushButton();
  objects[QString::fromUtf8("w_036")]=w_036; w_036->setObjectName(QString::fromUtf8("w_036"));
  w_036->setEnabled(true);
  w_036->setMinimumSize(0,0); w_036->setMaximumSize(16777215,16777215);
  w_036->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_036->setText(QString::fromUtf8("解除")); w_036->setCheckable(false); w_036->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_036->setSizePolicy(policy); }
  layout_034->addWidget(w_036);
  layout_034->setStretch(1,0);
  layout_024->setLayout(5,QFormLayout::ItemRole(2),layout_034);
  auto* layout_037=new QHBoxLayout();
  layout_037->setContentsMargins(0,0,0,0); layout_037->setSpacing(6);
  auto* w_038=new QPushButton();
  objects[QString::fromUtf8("w_038")]=w_038; w_038->setObjectName(QString::fromUtf8("w_038"));
  w_038->setEnabled(true);
  w_038->setMinimumSize(0,0); w_038->setMaximumSize(16777215,16777215);
  w_038->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_038->setText(QString::fromUtf8("選択範囲へ拡大")); w_038->setCheckable(false); w_038->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_038->setSizePolicy(policy); }
  layout_037->addWidget(w_038);
  layout_037->setStretch(0,0);
  auto* crop_button=new QPushButton();
  objects[QString::fromUtf8("crop_button")]=crop_button; crop_button->setObjectName(QString::fromUtf8("crop_button"));
  crop_button->setEnabled(false);
  crop_button->setMinimumSize(0,0); crop_button->setMaximumSize(16777215,16777215);
  crop_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  crop_button->setText(QString::fromUtf8("範囲をPNG保存")); crop_button->setCheckable(false); crop_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); crop_button->setSizePolicy(policy); }
  layout_037->addWidget(crop_button);
  layout_037->setStretch(1,0);
  layout_024->setLayout(6,QFormLayout::ItemRole(2),layout_037);
  auto* roi_label=new QLabel();
  objects[QString::fromUtf8("roi_label")]=roi_label; roi_label->setObjectName(QString::fromUtf8("roi_label"));
  roi_label->setEnabled(true);
  roi_label->setMinimumSize(0,0); roi_label->setMaximumSize(16777215,16777215);
  roi_label->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  roi_label->setText(QString::fromUtf8("範囲未選択")); roi_label->setWordWrap(true); roi_label->setTextInteractionFlags(Qt::TextInteractionFlags(4)); roi_label->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); roi_label->setSizePolicy(policy); }
  layout_024->setWidget(7,QFormLayout::ItemRole(2),roi_label);
  w_023->setLayout(layout_024);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_023->setSizePolicy(policy); }
  layout_005->addWidget(w_023);
  layout_005->setStretch(4,0);
  auto* w_041=new QGroupBox();
  objects[QString::fromUtf8("w_041")]=w_041; w_041->setObjectName(QString::fromUtf8("w_041"));
  w_041->setEnabled(true);
  w_041->setMinimumSize(0,0); w_041->setMaximumSize(16777215,16777215);
  w_041->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_041->setTitle(QString::fromUtf8("表示の濃淡（保存画素値には影響しません）"));
  auto* layout_042=new QFormLayout();
  layout_042->setContentsMargins(9,9,9,9); layout_042->setSpacing(6);
  layout_042->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_042->setFormAlignment(Qt::Alignment(33)); layout_042->setLabelAlignment(Qt::Alignment(1));
  auto* w_043=new QLabel();
  objects[QString::fromUtf8("w_043")]=w_043; w_043->setObjectName(QString::fromUtf8("w_043"));
  w_043->setEnabled(true);
  w_043->setMinimumSize(0,0); w_043->setMaximumSize(16777215,16777215);
  w_043->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_043->setText(QString::fromUtf8("黒に対応する値")); w_043->setWordWrap(false); w_043->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_043->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_043->setSizePolicy(policy); }
  layout_042->setWidget(0,QFormLayout::ItemRole(0),w_043);
  auto* low=new QDoubleSpinBox();
  objects[QString::fromUtf8("low")]=low; low->setObjectName(QString::fromUtf8("low"));
  low->setEnabled(true);
  low->setMinimumSize(0,0); low->setMaximumSize(16777215,16777215);
  low->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  low->setDecimals(1);
  low->setRange(-2147483648.0,2147483647.0); low->setSingleStep(1.0); low->setValue(0.0); low->setKeyboardTracking(false);
  low->setPrefix(QString::fromUtf8("")); low->setSuffix(QString::fromUtf8("")); low->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); low->setSizePolicy(policy); }
  layout_042->setWidget(0,QFormLayout::ItemRole(1),low);
  auto* w_045=new QLabel();
  objects[QString::fromUtf8("w_045")]=w_045; w_045->setObjectName(QString::fromUtf8("w_045"));
  w_045->setEnabled(true);
  w_045->setMinimumSize(0,0); w_045->setMaximumSize(16777215,16777215);
  w_045->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_045->setText(QString::fromUtf8("白に対応する値")); w_045->setWordWrap(false); w_045->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_045->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_045->setSizePolicy(policy); }
  layout_042->setWidget(1,QFormLayout::ItemRole(0),w_045);
  auto* high=new QDoubleSpinBox();
  objects[QString::fromUtf8("high")]=high; high->setObjectName(QString::fromUtf8("high"));
  high->setEnabled(true);
  high->setMinimumSize(0,0); high->setMaximumSize(16777215,16777215);
  high->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  high->setDecimals(1);
  high->setRange(-2147483648.0,2147483647.0); high->setSingleStep(1.0); high->setValue(255.0); high->setKeyboardTracking(false);
  high->setPrefix(QString::fromUtf8("")); high->setSuffix(QString::fromUtf8("")); high->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); high->setSizePolicy(policy); }
  layout_042->setWidget(1,QFormLayout::ItemRole(1),high);
  auto* invert=new QCheckBox();
  objects[QString::fromUtf8("invert")]=invert; invert->setObjectName(QString::fromUtf8("invert"));
  invert->setEnabled(true);
  invert->setMinimumSize(0,0); invert->setMaximumSize(16777215,16777215);
  invert->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  invert->setText(QString::fromUtf8("表示だけ白黒反転")); invert->setCheckable(true); invert->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); invert->setSizePolicy(policy); }
  layout_042->setWidget(2,QFormLayout::ItemRole(2),invert);
  auto* w_048=new QPushButton();
  objects[QString::fromUtf8("w_048")]=w_048; w_048->setObjectName(QString::fromUtf8("w_048"));
  w_048->setEnabled(true);
  w_048->setMinimumSize(0,0); w_048->setMaximumSize(16777215,16777215);
  w_048->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_048->setText(QString::fromUtf8("元の階調へ戻す")); w_048->setCheckable(false); w_048->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_048->setSizePolicy(policy); }
  layout_042->setWidget(3,QFormLayout::ItemRole(2),w_048);
  auto* w_049=new QLabel();
  objects[QString::fromUtf8("w_049")]=w_049; w_049->setObjectName(QString::fromUtf8("w_049"));
  w_049->setEnabled(true);
  w_049->setMinimumSize(0,0); w_049->setMaximumSize(16777215,16777215);
  w_049->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_049->setText(QString::fromUtf8("0/1画像は白＝1、黒＝0で表示します。\n保存するPNGは元の値（0/1を含む）です。")); w_049->setWordWrap(true); w_049->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_049->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_049->setSizePolicy(policy); }
  layout_042->setWidget(4,QFormLayout::ItemRole(2),w_049);
  w_041->setLayout(layout_042);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_041->setSizePolicy(policy); }
  layout_005->addWidget(w_041);
  layout_005->setStretch(5,0);
  auto* w_050=new QLabel();
  objects[QString::fromUtf8("w_050")]=w_050; w_050->setObjectName(QString::fromUtf8("w_050"));
  w_050->setEnabled(true);
  w_050->setMinimumSize(0,0); w_050->setMaximumSize(16777215,16777215);
  w_050->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_050->setText(QString::fromUtf8("左ドラッグ：画像を移動\nCtrl＋左ドラッグ：範囲選択\nホイール：層の切り替え\nCtrl＋ホイール：拡大・縮小\n← → ／ PageUp・PageDown：層の切り替え")); w_050->setWordWrap(true); w_050->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_050->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_050->setSizePolicy(policy); }
  layout_005->addWidget(w_050);
  layout_005->setStretch(6,0);
  layout_005->addItem(new QSpacerItem(0,0,QSizePolicy::Policy(1),QSizePolicy::Policy(7)));
  layout_005->setStretch(7,1);
  w_004->setLayout(layout_005);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_004->setSizePolicy(policy); }
  w_003->setWidgetResizable(true); w_003->setWidget(w_004);
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_003->setSizePolicy(policy); }
  feature_tabs->addTab(w_003,QString::fromUtf8("画像"));
  auto* w_051=new QScrollArea();
  objects[QString::fromUtf8("w_051")]=w_051; w_051->setObjectName(QString::fromUtf8("w_051"));
  w_051->setEnabled(true);
  w_051->setMinimumSize(0,0); w_051->setMaximumSize(16777215,16777215);
  w_051->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)));
  auto* w_052=new QWidget();
  objects[QString::fromUtf8("w_052")]=w_052; w_052->setObjectName(QString::fromUtf8("w_052"));
  w_052->setEnabled(true);
  w_052->setMinimumSize(0,0); w_052->setMaximumSize(16777215,16777215);
  w_052->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  auto* layout_053=new QVBoxLayout();
  layout_053->setContentsMargins(10,10,10,10); layout_053->setSpacing(6);
  auto* w_054=new QGroupBox();
  objects[QString::fromUtf8("w_054")]=w_054; w_054->setObjectName(QString::fromUtf8("w_054"));
  w_054->setEnabled(true);
  w_054->setMinimumSize(0,0); w_054->setMaximumSize(16777215,16777215);
  w_054->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_054->setTitle(QString::fromUtf8("自動層送り"));
  auto* layout_055=new QFormLayout();
  layout_055->setContentsMargins(9,9,9,9); layout_055->setSpacing(6);
  layout_055->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_055->setFormAlignment(Qt::Alignment(33)); layout_055->setLabelAlignment(Qt::Alignment(1));
  auto* w_056=new QLabel();
  objects[QString::fromUtf8("w_056")]=w_056; w_056->setObjectName(QString::fromUtf8("w_056"));
  w_056->setEnabled(true);
  w_056->setMinimumSize(0,0); w_056->setMaximumSize(16777215,16777215);
  w_056->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_056->setText(QString::fromUtf8("開始層（含む）")); w_056->setWordWrap(false); w_056->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_056->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_056->setSizePolicy(policy); }
  layout_055->setWidget(0,QFormLayout::ItemRole(0),w_056);
  auto* play_start=new QSpinBox();
  objects[QString::fromUtf8("play_start")]=play_start; play_start->setObjectName(QString::fromUtf8("play_start"));
  play_start->setEnabled(true);
  play_start->setMinimumSize(0,0); play_start->setMaximumSize(16777215,16777215);
  play_start->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  play_start->setRange(0,100000); play_start->setSingleStep(1); play_start->setValue(0); play_start->setKeyboardTracking(false);
  play_start->setPrefix(QString::fromUtf8("")); play_start->setSuffix(QString::fromUtf8("")); play_start->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); play_start->setSizePolicy(policy); }
  layout_055->setWidget(0,QFormLayout::ItemRole(1),play_start);
  auto* w_058=new QLabel();
  objects[QString::fromUtf8("w_058")]=w_058; w_058->setObjectName(QString::fromUtf8("w_058"));
  w_058->setEnabled(true);
  w_058->setMinimumSize(0,0); w_058->setMaximumSize(16777215,16777215);
  w_058->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_058->setText(QString::fromUtf8("終了層（含む）")); w_058->setWordWrap(false); w_058->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_058->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_058->setSizePolicy(policy); }
  layout_055->setWidget(1,QFormLayout::ItemRole(0),w_058);
  auto* play_end=new QSpinBox();
  objects[QString::fromUtf8("play_end")]=play_end; play_end->setObjectName(QString::fromUtf8("play_end"));
  play_end->setEnabled(true);
  play_end->setMinimumSize(0,0); play_end->setMaximumSize(16777215,16777215);
  play_end->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  play_end->setRange(0,100000); play_end->setSingleStep(1); play_end->setValue(0); play_end->setKeyboardTracking(false);
  play_end->setPrefix(QString::fromUtf8("")); play_end->setSuffix(QString::fromUtf8("")); play_end->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); play_end->setSizePolicy(policy); }
  layout_055->setWidget(1,QFormLayout::ItemRole(1),play_end);
  auto* w_060=new QLabel();
  objects[QString::fromUtf8("w_060")]=w_060; w_060->setObjectName(QString::fromUtf8("w_060"));
  w_060->setEnabled(true);
  w_060->setMinimumSize(0,0); w_060->setMaximumSize(16777215,16777215);
  w_060->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_060->setText(QString::fromUtf8("層の間隔")); w_060->setWordWrap(false); w_060->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_060->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_060->setSizePolicy(policy); }
  layout_055->setWidget(2,QFormLayout::ItemRole(0),w_060);
  auto* play_step=new QSpinBox();
  objects[QString::fromUtf8("play_step")]=play_step; play_step->setObjectName(QString::fromUtf8("play_step"));
  play_step->setEnabled(true);
  play_step->setMinimumSize(0,0); play_step->setMaximumSize(16777215,16777215);
  play_step->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  play_step->setRange(1,10000); play_step->setSingleStep(1); play_step->setValue(1); play_step->setKeyboardTracking(false);
  play_step->setPrefix(QString::fromUtf8("")); play_step->setSuffix(QString::fromUtf8("")); play_step->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); play_step->setSizePolicy(policy); }
  layout_055->setWidget(2,QFormLayout::ItemRole(1),play_step);
  auto* preload_check=new QCheckBox();
  objects[QString::fromUtf8("preload_check")]=preload_check; preload_check->setObjectName(QString::fromUtf8("preload_check"));
  preload_check->setEnabled(true);
  preload_check->setMinimumSize(0,0); preload_check->setMaximumSize(16777215,16777215);
  preload_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  preload_check->setText(QString::fromUtf8("再生前に全層を先読みする")); preload_check->setCheckable(true); preload_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); preload_check->setSizePolicy(policy); }
  layout_055->setWidget(3,QFormLayout::ItemRole(2),preload_check);
  auto* preload_button=new QPushButton();
  objects[QString::fromUtf8("preload_button")]=preload_button; preload_button->setObjectName(QString::fromUtf8("preload_button"));
  preload_button->setEnabled(false);
  preload_button->setMinimumSize(0,0); preload_button->setMaximumSize(16777215,16777215);
  preload_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  preload_button->setText(QString::fromUtf8("全層を先読み")); preload_button->setCheckable(false); preload_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); preload_button->setSizePolicy(policy); }
  layout_055->setWidget(4,QFormLayout::ItemRole(2),preload_button);
  w_054->setLayout(layout_055);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_054->setSizePolicy(policy); }
  layout_053->addWidget(w_054);
  layout_053->setStretch(0,0);
  auto* w_064=new QLabel();
  objects[QString::fromUtf8("w_064")]=w_064; w_064->setObjectName(QString::fromUtf8("w_064"));
  w_064->setEnabled(true);
  w_064->setMinimumSize(0,0); w_064->setMaximumSize(16777215,16777215);
  w_064->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_064->setText(QString::fromUtf8("再生ボタン・速度・ループは画像下にも表示します。読み込みが遅い場合は指定速度より遅く再生し、層を飛ばしません。")); w_064->setWordWrap(true); w_064->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_064->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_064->setSizePolicy(policy); }
  layout_053->addWidget(w_064);
  layout_053->setStretch(1,0);
  auto* w_065=new QGroupBox();
  objects[QString::fromUtf8("w_065")]=w_065; w_065->setObjectName(QString::fromUtf8("w_065"));
  w_065->setEnabled(true);
  w_065->setMinimumSize(0,0); w_065->setMaximumSize(16777215,16777215);
  w_065->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_065->setTitle(QString::fromUtf8("2点を結ぶ経路を画面中央へ"));
  auto* layout_066=new QVBoxLayout();
  layout_066->setContentsMargins(9,9,9,9); layout_066->setSpacing(6);
  auto* w_067=new QLabel();
  objects[QString::fromUtf8("w_067")]=w_067; w_067->setObjectName(QString::fromUtf8("w_067"));
  w_067->setEnabled(true);
  w_067->setMinimumSize(0,0); w_067->setMaximumSize(16777215,16777215);
  w_067->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_067->setText(QString::fromUtf8("X・Yは元のspngのpixel座標（左下原点）。異なる層で1点ずつ指定してください。")); w_067->setWordWrap(true); w_067->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_067->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_067->setSizePolicy(policy); }
  layout_066->addWidget(w_067);
  layout_066->setStretch(0,0);
  auto* w_068=new QGroupBox();
  objects[QString::fromUtf8("w_068")]=w_068; w_068->setObjectName(QString::fromUtf8("w_068"));
  w_068->setEnabled(true);
  w_068->setMinimumSize(0,0); w_068->setMaximumSize(16777215,16777215);
  w_068->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_068->setTitle(QString::fromUtf8("点1"));
  auto* layout_069=new QFormLayout();
  layout_069->setContentsMargins(9,9,9,9); layout_069->setSpacing(6);
  layout_069->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_069->setFormAlignment(Qt::Alignment(33)); layout_069->setLabelAlignment(Qt::Alignment(1));
  auto* w_070=new QLabel();
  objects[QString::fromUtf8("w_070")]=w_070; w_070->setObjectName(QString::fromUtf8("w_070"));
  w_070->setEnabled(true);
  w_070->setMinimumSize(0,0); w_070->setMaximumSize(16777215,16777215);
  w_070->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_070->setText(QString::fromUtf8("層番号")); w_070->setWordWrap(false); w_070->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_070->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_070->setSizePolicy(policy); }
  layout_069->setWidget(0,QFormLayout::ItemRole(0),w_070);
  auto* point0_z=new QSpinBox();
  objects[QString::fromUtf8("point0_z")]=point0_z; point0_z->setObjectName(QString::fromUtf8("point0_z"));
  point0_z->setEnabled(true);
  point0_z->setMinimumSize(0,0); point0_z->setMaximumSize(16777215,16777215);
  point0_z->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  point0_z->setRange(0,100000); point0_z->setSingleStep(1); point0_z->setValue(0); point0_z->setKeyboardTracking(false);
  point0_z->setPrefix(QString::fromUtf8("")); point0_z->setSuffix(QString::fromUtf8("")); point0_z->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); point0_z->setSizePolicy(policy); }
  layout_069->setWidget(0,QFormLayout::ItemRole(1),point0_z);
  auto* w_072=new QLabel();
  objects[QString::fromUtf8("w_072")]=w_072; w_072->setObjectName(QString::fromUtf8("w_072"));
  w_072->setEnabled(true);
  w_072->setMinimumSize(0,0); w_072->setMaximumSize(16777215,16777215);
  w_072->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_072->setText(QString::fromUtf8("X")); w_072->setWordWrap(false); w_072->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_072->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_072->setSizePolicy(policy); }
  layout_069->setWidget(1,QFormLayout::ItemRole(0),w_072);
  auto* point0_x=new QDoubleSpinBox();
  objects[QString::fromUtf8("point0_x")]=point0_x; point0_x->setObjectName(QString::fromUtf8("point0_x"));
  point0_x->setEnabled(true);
  point0_x->setMinimumSize(0,0); point0_x->setMaximumSize(16777215,16777215);
  point0_x->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  point0_x->setDecimals(2);
  point0_x->setRange(-100000.0,100000.0); point0_x->setSingleStep(1.0); point0_x->setValue(0.0); point0_x->setKeyboardTracking(false);
  point0_x->setPrefix(QString::fromUtf8("")); point0_x->setSuffix(QString::fromUtf8("")); point0_x->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); point0_x->setSizePolicy(policy); }
  layout_069->setWidget(1,QFormLayout::ItemRole(1),point0_x);
  auto* w_074=new QLabel();
  objects[QString::fromUtf8("w_074")]=w_074; w_074->setObjectName(QString::fromUtf8("w_074"));
  w_074->setEnabled(true);
  w_074->setMinimumSize(0,0); w_074->setMaximumSize(16777215,16777215);
  w_074->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_074->setText(QString::fromUtf8("Y")); w_074->setWordWrap(false); w_074->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_074->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_074->setSizePolicy(policy); }
  layout_069->setWidget(2,QFormLayout::ItemRole(0),w_074);
  auto* point0_y=new QDoubleSpinBox();
  objects[QString::fromUtf8("point0_y")]=point0_y; point0_y->setObjectName(QString::fromUtf8("point0_y"));
  point0_y->setEnabled(true);
  point0_y->setMinimumSize(0,0); point0_y->setMaximumSize(16777215,16777215);
  point0_y->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  point0_y->setDecimals(2);
  point0_y->setRange(-100000.0,100000.0); point0_y->setSingleStep(1.0); point0_y->setValue(0.0); point0_y->setKeyboardTracking(false);
  point0_y->setPrefix(QString::fromUtf8("")); point0_y->setSuffix(QString::fromUtf8("")); point0_y->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); point0_y->setSizePolicy(policy); }
  layout_069->setWidget(2,QFormLayout::ItemRole(1),point0_y);
  auto* w_076=new QPushButton();
  objects[QString::fromUtf8("w_076")]=w_076; w_076->setObjectName(QString::fromUtf8("w_076"));
  w_076->setEnabled(true);
  w_076->setMinimumSize(0,0); w_076->setMaximumSize(16777215,16777215);
  w_076->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_076->setText(QString::fromUtf8("画像上をクリックして指定")); w_076->setCheckable(false); w_076->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_076->setSizePolicy(policy); }
  layout_069->setWidget(3,QFormLayout::ItemRole(2),w_076);
  auto* w_077=new QPushButton();
  objects[QString::fromUtf8("w_077")]=w_077; w_077->setObjectName(QString::fromUtf8("w_077"));
  w_077->setEnabled(true);
  w_077->setMinimumSize(0,0); w_077->setMaximumSize(16777215,16777215);
  w_077->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_077->setText(QString::fromUtf8("入力した座標を登録")); w_077->setCheckable(false); w_077->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_077->setSizePolicy(policy); }
  layout_069->setWidget(4,QFormLayout::ItemRole(2),w_077);
  auto* w_078=new QPushButton();
  objects[QString::fromUtf8("w_078")]=w_078; w_078->setObjectName(QString::fromUtf8("w_078"));
  w_078->setEnabled(true);
  w_078->setMinimumSize(0,0); w_078->setMaximumSize(16777215,16777215);
  w_078->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_078->setText(QString::fromUtf8("この点をリセット")); w_078->setCheckable(false); w_078->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_078->setSizePolicy(policy); }
  layout_069->setWidget(5,QFormLayout::ItemRole(2),w_078);
  w_068->setLayout(layout_069);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_068->setSizePolicy(policy); }
  layout_066->addWidget(w_068);
  layout_066->setStretch(1,0);
  auto* w_079=new QGroupBox();
  objects[QString::fromUtf8("w_079")]=w_079; w_079->setObjectName(QString::fromUtf8("w_079"));
  w_079->setEnabled(true);
  w_079->setMinimumSize(0,0); w_079->setMaximumSize(16777215,16777215);
  w_079->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_079->setTitle(QString::fromUtf8("点2"));
  auto* layout_080=new QFormLayout();
  layout_080->setContentsMargins(9,9,9,9); layout_080->setSpacing(6);
  layout_080->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_080->setFormAlignment(Qt::Alignment(33)); layout_080->setLabelAlignment(Qt::Alignment(1));
  auto* w_081=new QLabel();
  objects[QString::fromUtf8("w_081")]=w_081; w_081->setObjectName(QString::fromUtf8("w_081"));
  w_081->setEnabled(true);
  w_081->setMinimumSize(0,0); w_081->setMaximumSize(16777215,16777215);
  w_081->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_081->setText(QString::fromUtf8("層番号")); w_081->setWordWrap(false); w_081->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_081->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_081->setSizePolicy(policy); }
  layout_080->setWidget(0,QFormLayout::ItemRole(0),w_081);
  auto* point1_z=new QSpinBox();
  objects[QString::fromUtf8("point1_z")]=point1_z; point1_z->setObjectName(QString::fromUtf8("point1_z"));
  point1_z->setEnabled(true);
  point1_z->setMinimumSize(0,0); point1_z->setMaximumSize(16777215,16777215);
  point1_z->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  point1_z->setRange(0,100000); point1_z->setSingleStep(1); point1_z->setValue(0); point1_z->setKeyboardTracking(false);
  point1_z->setPrefix(QString::fromUtf8("")); point1_z->setSuffix(QString::fromUtf8("")); point1_z->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); point1_z->setSizePolicy(policy); }
  layout_080->setWidget(0,QFormLayout::ItemRole(1),point1_z);
  auto* w_083=new QLabel();
  objects[QString::fromUtf8("w_083")]=w_083; w_083->setObjectName(QString::fromUtf8("w_083"));
  w_083->setEnabled(true);
  w_083->setMinimumSize(0,0); w_083->setMaximumSize(16777215,16777215);
  w_083->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_083->setText(QString::fromUtf8("X")); w_083->setWordWrap(false); w_083->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_083->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_083->setSizePolicy(policy); }
  layout_080->setWidget(1,QFormLayout::ItemRole(0),w_083);
  auto* point1_x=new QDoubleSpinBox();
  objects[QString::fromUtf8("point1_x")]=point1_x; point1_x->setObjectName(QString::fromUtf8("point1_x"));
  point1_x->setEnabled(true);
  point1_x->setMinimumSize(0,0); point1_x->setMaximumSize(16777215,16777215);
  point1_x->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  point1_x->setDecimals(2);
  point1_x->setRange(-100000.0,100000.0); point1_x->setSingleStep(1.0); point1_x->setValue(0.0); point1_x->setKeyboardTracking(false);
  point1_x->setPrefix(QString::fromUtf8("")); point1_x->setSuffix(QString::fromUtf8("")); point1_x->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); point1_x->setSizePolicy(policy); }
  layout_080->setWidget(1,QFormLayout::ItemRole(1),point1_x);
  auto* w_085=new QLabel();
  objects[QString::fromUtf8("w_085")]=w_085; w_085->setObjectName(QString::fromUtf8("w_085"));
  w_085->setEnabled(true);
  w_085->setMinimumSize(0,0); w_085->setMaximumSize(16777215,16777215);
  w_085->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_085->setText(QString::fromUtf8("Y")); w_085->setWordWrap(false); w_085->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_085->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_085->setSizePolicy(policy); }
  layout_080->setWidget(2,QFormLayout::ItemRole(0),w_085);
  auto* point1_y=new QDoubleSpinBox();
  objects[QString::fromUtf8("point1_y")]=point1_y; point1_y->setObjectName(QString::fromUtf8("point1_y"));
  point1_y->setEnabled(true);
  point1_y->setMinimumSize(0,0); point1_y->setMaximumSize(16777215,16777215);
  point1_y->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  point1_y->setDecimals(2);
  point1_y->setRange(-100000.0,100000.0); point1_y->setSingleStep(1.0); point1_y->setValue(0.0); point1_y->setKeyboardTracking(false);
  point1_y->setPrefix(QString::fromUtf8("")); point1_y->setSuffix(QString::fromUtf8("")); point1_y->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); point1_y->setSizePolicy(policy); }
  layout_080->setWidget(2,QFormLayout::ItemRole(1),point1_y);
  auto* w_087=new QPushButton();
  objects[QString::fromUtf8("w_087")]=w_087; w_087->setObjectName(QString::fromUtf8("w_087"));
  w_087->setEnabled(true);
  w_087->setMinimumSize(0,0); w_087->setMaximumSize(16777215,16777215);
  w_087->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_087->setText(QString::fromUtf8("画像上をクリックして指定")); w_087->setCheckable(false); w_087->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_087->setSizePolicy(policy); }
  layout_080->setWidget(3,QFormLayout::ItemRole(2),w_087);
  auto* w_088=new QPushButton();
  objects[QString::fromUtf8("w_088")]=w_088; w_088->setObjectName(QString::fromUtf8("w_088"));
  w_088->setEnabled(true);
  w_088->setMinimumSize(0,0); w_088->setMaximumSize(16777215,16777215);
  w_088->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_088->setText(QString::fromUtf8("入力した座標を登録")); w_088->setCheckable(false); w_088->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_088->setSizePolicy(policy); }
  layout_080->setWidget(4,QFormLayout::ItemRole(2),w_088);
  auto* w_089=new QPushButton();
  objects[QString::fromUtf8("w_089")]=w_089; w_089->setObjectName(QString::fromUtf8("w_089"));
  w_089->setEnabled(true);
  w_089->setMinimumSize(0,0); w_089->setMaximumSize(16777215,16777215);
  w_089->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_089->setText(QString::fromUtf8("この点をリセット")); w_089->setCheckable(false); w_089->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_089->setSizePolicy(policy); }
  layout_080->setWidget(5,QFormLayout::ItemRole(2),w_089);
  w_079->setLayout(layout_080);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_079->setSizePolicy(policy); }
  layout_066->addWidget(w_079);
  layout_066->setStretch(2,0);
  auto* follow_check=new QCheckBox();
  objects[QString::fromUtf8("follow_check")]=follow_check; follow_check->setObjectName(QString::fromUtf8("follow_check"));
  follow_check->setEnabled(true);
  follow_check->setMinimumSize(0,0); follow_check->setMaximumSize(16777215,16777215);
  follow_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  follow_check->setText(QString::fromUtf8("経路に沿って中心を追従")); follow_check->setCheckable(true); follow_check->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); follow_check->setSizePolicy(policy); }
  layout_066->addWidget(follow_check);
  layout_066->setStretch(3,0);
  auto* extend_check=new QCheckBox();
  objects[QString::fromUtf8("extend_check")]=extend_check; extend_check->setObjectName(QString::fromUtf8("extend_check"));
  extend_check->setEnabled(true);
  extend_check->setMinimumSize(0,0); extend_check->setMaximumSize(16777215,16777215);
  extend_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  extend_check->setText(QString::fromUtf8("2点の外側にも直線を延長")); extend_check->setCheckable(true); extend_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); extend_check->setSizePolicy(policy); }
  layout_066->addWidget(extend_check);
  layout_066->setStretch(4,0);
  auto* track_label=new QLabel();
  objects[QString::fromUtf8("track_label")]=track_label; track_label->setObjectName(QString::fromUtf8("track_label"));
  track_label->setEnabled(true);
  track_label->setMinimumSize(0,0); track_label->setMaximumSize(16777215,16777215);
  track_label->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  track_label->setText(QString::fromUtf8("2点未登録")); track_label->setWordWrap(true); track_label->setTextInteractionFlags(Qt::TextInteractionFlags(4)); track_label->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); track_label->setSizePolicy(policy); }
  layout_066->addWidget(track_label);
  layout_066->setStretch(5,0);
  auto* w_093=new QPushButton();
  objects[QString::fromUtf8("w_093")]=w_093; w_093->setObjectName(QString::fromUtf8("w_093"));
  w_093->setEnabled(true);
  w_093->setMinimumSize(0,0); w_093->setMaximumSize(16777215,16777215);
  w_093->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_093->setText(QString::fromUtf8("再生範囲を2点の層に合わせる")); w_093->setCheckable(false); w_093->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_093->setSizePolicy(policy); }
  layout_066->addWidget(w_093);
  layout_066->setStretch(6,0);
  w_065->setLayout(layout_066);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_065->setSizePolicy(policy); }
  layout_053->addWidget(w_065);
  layout_053->setStretch(2,0);
  layout_053->addItem(new QSpacerItem(0,0,QSizePolicy::Policy(1),QSizePolicy::Policy(7)));
  layout_053->setStretch(3,0);
  w_052->setLayout(layout_053);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_052->setSizePolicy(policy); }
  w_051->setWidgetResizable(true); w_051->setWidget(w_052);
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_051->setSizePolicy(policy); }
  feature_tabs->addTab(w_051,QString::fromUtf8("再生・追従"));
  auto* w_094=new QScrollArea();
  objects[QString::fromUtf8("w_094")]=w_094; w_094->setObjectName(QString::fromUtf8("w_094"));
  w_094->setEnabled(true);
  w_094->setMinimumSize(0,0); w_094->setMaximumSize(16777215,16777215);
  w_094->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)));
  auto* w_095=new QWidget();
  objects[QString::fromUtf8("w_095")]=w_095; w_095->setObjectName(QString::fromUtf8("w_095"));
  w_095->setEnabled(true);
  w_095->setMinimumSize(0,0); w_095->setMaximumSize(16777215,16777215);
  w_095->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  auto* layout_096=new QVBoxLayout();
  layout_096->setContentsMargins(10,10,10,10); layout_096->setSpacing(6);
  auto* w_097=new QLabel();
  objects[QString::fromUtf8("w_097")]=w_097; w_097->setObjectName(QString::fromUtf8("w_097"));
  w_097->setEnabled(true);
  w_097->setMinimumSize(0,0); w_097->setMaximumSize(16777215,16777215);
  w_097->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_097->setText(QString::fromUtf8("対象は、現在のファイルと「表示データ」で選択した処理段階です。層番号は0始まり・両端を含みます。")); w_097->setWordWrap(true); w_097->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_097->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_097->setSizePolicy(policy); }
  layout_096->addWidget(w_097);
  layout_096->setStretch(0,0);
  auto* layout_098=new QFormLayout();
  layout_098->setContentsMargins(0,0,0,0); layout_098->setSpacing(6);
  layout_098->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_098->setFormAlignment(Qt::Alignment(33)); layout_098->setLabelAlignment(Qt::Alignment(1));
  auto* w_099=new QLabel();
  objects[QString::fromUtf8("w_099")]=w_099; w_099->setObjectName(QString::fromUtf8("w_099"));
  w_099->setEnabled(true);
  w_099->setMinimumSize(0,0); w_099->setMaximumSize(16777215,16777215);
  w_099->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_099->setText(QString::fromUtf8("層の指定")); w_099->setWordWrap(false); w_099->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_099->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_099->setSizePolicy(policy); }
  layout_098->setWidget(0,QFormLayout::ItemRole(0),w_099);
  auto* layer_spec=new QLineEdit();
  objects[QString::fromUtf8("layer_spec")]=layer_spec; layer_spec->setObjectName(QString::fromUtf8("layer_spec"));
  layer_spec->setEnabled(true);
  layer_spec->setMinimumSize(0,0); layer_spec->setMaximumSize(16777215,16777215);
  layer_spec->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)));
  layer_spec->setText(QString::fromUtf8("0")); layer_spec->setReadOnly(false); layer_spec->setPlaceholderText(QString::fromUtf8("例: 0-15,20,25-31:2"));
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); layer_spec->setSizePolicy(policy); }
  layout_098->setWidget(0,QFormLayout::ItemRole(1),layer_spec);
  layout_096->addLayout(layout_098);
  layout_096->setStretch(1,0);
  auto* w_101=new QLabel();
  objects[QString::fromUtf8("w_101")]=w_101; w_101->setObjectName(QString::fromUtf8("w_101"));
  w_101->setEnabled(true);
  w_101->setMinimumSize(0,0); w_101->setMaximumSize(16777215,16777215);
  w_101->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_101->setText(QString::fromUtf8("カンマで追加、a-bで範囲、a-b:2で2層おき。欠損層は除外します。")); w_101->setWordWrap(true); w_101->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_101->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_101->setSizePolicy(policy); }
  layout_096->addWidget(w_101);
  layout_096->setStretch(2,0);
  auto* w_102=new QGroupBox();
  objects[QString::fromUtf8("w_102")]=w_102; w_102->setObjectName(QString::fromUtf8("w_102"));
  w_102->setEnabled(true);
  w_102->setMinimumSize(0,0); w_102->setMaximumSize(16777215,16777215);
  w_102->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_102->setTitle(QString::fromUtf8("最大値・最小値を重ね合わせ"));
  auto* layout_103=new QVBoxLayout();
  layout_103->setContentsMargins(9,9,9,9); layout_103->setSpacing(6);
  auto* layout_104=new QHBoxLayout();
  layout_104->setContentsMargins(0,0,0,0); layout_104->setSpacing(6);
  auto* max_button=new QPushButton();
  objects[QString::fromUtf8("max_button")]=max_button; max_button->setObjectName(QString::fromUtf8("max_button"));
  max_button->setEnabled(false);
  max_button->setMinimumSize(0,0); max_button->setMaximumSize(16777215,16777215);
  max_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  max_button->setText(QString::fromUtf8("最大値を表示")); max_button->setCheckable(false); max_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); max_button->setSizePolicy(policy); }
  layout_104->addWidget(max_button);
  layout_104->setStretch(0,0);
  auto* min_button=new QPushButton();
  objects[QString::fromUtf8("min_button")]=min_button; min_button->setObjectName(QString::fromUtf8("min_button"));
  min_button->setEnabled(false);
  min_button->setMinimumSize(0,0); min_button->setMaximumSize(16777215,16777215);
  min_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  min_button->setText(QString::fromUtf8("最小値を表示")); min_button->setCheckable(false); min_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); min_button->setSizePolicy(policy); }
  layout_104->addWidget(min_button);
  layout_104->setStretch(1,0);
  layout_103->addLayout(layout_104);
  layout_103->setStretch(0,0);
  auto* depth_check=new QCheckBox();
  objects[QString::fromUtf8("depth_check")]=depth_check; depth_check->setObjectName(QString::fromUtf8("depth_check"));
  depth_check->setEnabled(true);
  depth_check->setMinimumSize(0,0); depth_check->setMaximumSize(16777215,16777215);
  depth_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  depth_check->setText(QString::fromUtf8("最大・最小になった層の番号で色分け")); depth_check->setCheckable(true); depth_check->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); depth_check->setSizePolicy(policy); }
  layout_103->addWidget(depth_check);
  layout_103->setStretch(1,0);
  auto* palette_combo=new QComboBox();
  objects[QString::fromUtf8("palette_combo")]=palette_combo; palette_combo->setObjectName(QString::fromUtf8("palette_combo"));
  palette_combo->setEnabled(true);
  palette_combo->setMinimumSize(0,0); palette_combo->setMaximumSize(16777215,16777215);
  palette_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  palette_combo->setEditable(false); palette_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); palette_combo->setMaxVisibleItems(10); palette_combo->setMinimumContentsLength(0); palette_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(1)); palette_combo->setIconSize(QSize(260,22));
  palette_combo->addItem(QIcon(QPixmap::fromImage(paletteBar(QString::fromUtf8("kingfisher")))),QString::fromUtf8("kingfisher"),QString::fromUtf8("kingfisher"));
  palette_combo->setItemData(0,QString::fromUtf8("kingfisher"),3);
  palette_combo->setItemData(0,QString::fromUtf8("kingfisher"),11);
  palette_combo->addItem(QIcon(QPixmap::fromImage(paletteBar(QString::fromUtf8("meadow")))),QString::fromUtf8("meadow"),QString::fromUtf8("meadow"));
  palette_combo->setItemData(1,QString::fromUtf8("meadow"),3);
  palette_combo->setItemData(1,QString::fromUtf8("meadow"),11);
  palette_combo->addItem(QIcon(QPixmap::fromImage(paletteBar(QString::fromUtf8("ember")))),QString::fromUtf8("ember"),QString::fromUtf8("ember"));
  palette_combo->setItemData(2,QString::fromUtf8("ember"),3);
  palette_combo->setItemData(2,QString::fromUtf8("ember"),11);
  palette_combo->addItem(QIcon(QPixmap::fromImage(paletteBar(QString::fromUtf8("heat")))),QString::fromUtf8("heat"),QString::fromUtf8("heat"));
  palette_combo->setItemData(3,QString::fromUtf8("heat"),3);
  palette_combo->setItemData(3,QString::fromUtf8("heat"),11);
  palette_combo->addItem(QIcon(QPixmap::fromImage(paletteBar(QString::fromUtf8("rainbow")))),QString::fromUtf8("rainbow"),QString::fromUtf8("rainbow"));
  palette_combo->setItemData(4,QString::fromUtf8("rainbow"),3);
  palette_combo->setItemData(4,QString::fromUtf8("rainbow"),11);
  palette_combo->setCurrentIndex(0);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); palette_combo->setSizePolicy(policy); }
  layout_103->addWidget(palette_combo);
  layout_103->setStretch(2,0);
  auto* depth_weight=new QCheckBox();
  objects[QString::fromUtf8("depth_weight")]=depth_weight; depth_weight->setObjectName(QString::fromUtf8("depth_weight"));
  depth_weight->setEnabled(true);
  depth_weight->setMinimumSize(0,0); depth_weight->setMaximumSize(16777215,16777215);
  depth_weight->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  depth_weight->setText(QString::fromUtf8("値で明度を変える（最小値は暗さで明度）")); depth_weight->setCheckable(true); depth_weight->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); depth_weight->setSizePolicy(policy); }
  layout_103->addWidget(depth_weight);
  layout_103->setStretch(3,0);
  auto* depth_legend=new QLabel();
  objects[QString::fromUtf8("depth_legend")]=depth_legend; depth_legend->setObjectName(QString::fromUtf8("depth_legend"));
  depth_legend->setEnabled(true);
  depth_legend->setMinimumSize(0,0); depth_legend->setMaximumSize(16777215,16777215);
  depth_legend->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  depth_legend->setText(QString::fromUtf8("")); depth_legend->setWordWrap(false); depth_legend->setTextInteractionFlags(Qt::TextInteractionFlags(4)); depth_legend->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); depth_legend->setSizePolicy(policy); }
  layout_103->addWidget(depth_legend);
  layout_103->setStretch(4,0);
  auto* depth_text=new QLabel();
  objects[QString::fromUtf8("depth_text")]=depth_text; depth_text->setObjectName(QString::fromUtf8("depth_text"));
  depth_text->setEnabled(true);
  depth_text->setMinimumSize(0,0); depth_text->setMaximumSize(16777215,16777215);
  depth_text->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  depth_text->setText(QString::fromUtf8("同値は最も小さい層番号を採用します。")); depth_text->setWordWrap(true); depth_text->setTextInteractionFlags(Qt::TextInteractionFlags(4)); depth_text->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); depth_text->setSizePolicy(policy); }
  layout_103->addWidget(depth_text);
  layout_103->setStretch(5,0);
  auto* w_112=new QPushButton();
  objects[QString::fromUtf8("w_112")]=w_112; w_112->setObjectName(QString::fromUtf8("w_112"));
  w_112->setEnabled(true);
  w_112->setMinimumSize(0,0); w_112->setMaximumSize(16777215,16777215);
  w_112->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_112->setText(QString::fromUtf8("通常の層表示へ戻る")); w_112->setCheckable(false); w_112->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_112->setSizePolicy(policy); }
  layout_103->addWidget(w_112);
  layout_103->setStretch(6,0);
  w_102->setLayout(layout_103);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_102->setSizePolicy(policy); }
  layout_096->addWidget(w_102);
  layout_096->setStretch(3,0);
  auto* w_113=new QGroupBox();
  objects[QString::fromUtf8("w_113")]=w_113; w_113->setObjectName(QString::fromUtf8("w_113"));
  w_113->setEnabled(true);
  w_113->setMinimumSize(0,0); w_113->setMaximumSize(16777215,16777215);
  w_113->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_113->setTitle(QString::fromUtf8("指定した層をそれぞれPNG出力"));
  auto* layout_114=new QVBoxLayout();
  layout_114->setContentsMargins(9,9,9,9); layout_114->setSpacing(6);
  auto* batch_crop=new QCheckBox();
  objects[QString::fromUtf8("batch_crop")]=batch_crop; batch_crop->setObjectName(QString::fromUtf8("batch_crop"));
  batch_crop->setEnabled(true);
  batch_crop->setMinimumSize(0,0); batch_crop->setMaximumSize(16777215,16777215);
  batch_crop->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  batch_crop->setText(QString::fromUtf8("選択範囲だけを書き出す")); batch_crop->setCheckable(true); batch_crop->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); batch_crop->setSizePolicy(policy); }
  layout_114->addWidget(batch_crop);
  layout_114->setStretch(0,0);
  auto* export_button=new QPushButton();
  objects[QString::fromUtf8("export_button")]=export_button; export_button->setObjectName(QString::fromUtf8("export_button"));
  export_button->setEnabled(false);
  export_button->setMinimumSize(0,0); export_button->setMaximumSize(16777215,16777215);
  export_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  export_button->setText(QString::fromUtf8("保存先を選んで一括出力")); export_button->setCheckable(false); export_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); export_button->setSizePolicy(policy); }
  layout_114->addWidget(export_button);
  layout_114->setStretch(1,0);
  auto* w_117=new QLabel();
  objects[QString::fromUtf8("w_117")]=w_117; w_117->setObjectName(QString::fromUtf8("w_117"));
  w_117->setEnabled(true);
  w_117->setMinimumSize(0,0); w_117->setMaximumSize(16777215,16777215);
  w_117->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_117->setText(QString::fromUtf8("保存先内に新しい専用フォルダを作成します。元画像／処理後の画素値を保持し、層ごとにPNGを保存します。処理条件と欠損層の記録も保存します。")); w_117->setWordWrap(true); w_117->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_117->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_117->setSizePolicy(policy); }
  layout_114->addWidget(w_117);
  layout_114->setStretch(2,0);
  w_113->setLayout(layout_114);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_113->setSizePolicy(policy); }
  layout_096->addWidget(w_113);
  layout_096->setStretch(4,0);
  auto* w_118=new QPushButton();
  objects[QString::fromUtf8("w_118")]=w_118; w_118->setObjectName(QString::fromUtf8("w_118"));
  w_118->setEnabled(true);
  w_118->setMinimumSize(0,0); w_118->setMaximumSize(16777215,16777215);
  w_118->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_118->setText(QString::fromUtf8("表示層をPNG保存")); w_118->setCheckable(false); w_118->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_118->setSizePolicy(policy); }
  layout_096->addWidget(w_118);
  layout_096->setStretch(5,0);
  layout_096->addItem(new QSpacerItem(0,0,QSizePolicy::Policy(1),QSizePolicy::Policy(7)));
  layout_096->setStretch(6,0);
  w_095->setLayout(layout_096);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_095->setSizePolicy(policy); }
  w_094->setWidgetResizable(true); w_094->setWidget(w_095);
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_094->setSizePolicy(policy); }
  feature_tabs->addTab(w_094,QString::fromUtf8("重ね合わせ・出力"));
  auto* processing_page=new QScrollArea();
  objects[QString::fromUtf8("processing_page")]=processing_page; processing_page->setObjectName(QString::fromUtf8("processing_page"));
  processing_page->setEnabled(true);
  processing_page->setMinimumSize(0,0); processing_page->setMaximumSize(16777215,16777215);
  processing_page->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)));
  auto* w_120=new QWidget();
  objects[QString::fromUtf8("w_120")]=w_120; w_120->setObjectName(QString::fromUtf8("w_120"));
  w_120->setEnabled(true);
  w_120->setMinimumSize(0,0); w_120->setMaximumSize(16777215,16777215);
  w_120->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  auto* layout_121=new QVBoxLayout();
  layout_121->setContentsMargins(10,10,10,10); layout_121->setSpacing(6);
  auto* layout_122=new QFormLayout();
  layout_122->setContentsMargins(0,0,0,0); layout_122->setSpacing(6);
  layout_122->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_122->setFormAlignment(Qt::Alignment(33)); layout_122->setLabelAlignment(Qt::Alignment(1));
  auto* w_123=new QLabel();
  objects[QString::fromUtf8("w_123")]=w_123; w_123->setObjectName(QString::fromUtf8("w_123"));
  w_123->setEnabled(true);
  w_123->setMinimumSize(0,0); w_123->setMaximumSize(16777215,16777215);
  w_123->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_123->setText(QString::fromUtf8("表示データ")); w_123->setWordWrap(false); w_123->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_123->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_123->setSizePolicy(policy); }
  layout_122->setWidget(0,QFormLayout::ItemRole(0),w_123);
  auto* stage_combo=new QComboBox();
  objects[QString::fromUtf8("stage_combo")]=stage_combo; stage_combo->setObjectName(QString::fromUtf8("stage_combo"));
  stage_combo->setEnabled(false);
  stage_combo->setMinimumSize(0,0); stage_combo->setMaximumSize(16777215,16777215);
  stage_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  stage_combo->setEditable(false); stage_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); stage_combo->setMaxVisibleItems(10); stage_combo->setMinimumContentsLength(0); stage_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(1)); stage_combo->setIconSize(QSize(16,16));
  stage_combo->addItem(QString::fromUtf8("元画像"),QVariant());
  stage_combo->setCurrentIndex(0);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); stage_combo->setSizePolicy(policy); }
  layout_122->setWidget(0,QFormLayout::ItemRole(1),stage_combo);
  layout_121->addLayout(layout_122);
  layout_121->setStretch(0,0);
  auto* scan_auto=new QCheckBox();
  objects[QString::fromUtf8("scan_auto")]=scan_auto; scan_auto->setObjectName(QString::fromUtf8("scan_auto"));
  scan_auto->setEnabled(true);
  scan_auto->setMinimumSize(0,0); scan_auto->setMaximumSize(16777215,16777215);
  scan_auto->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  scan_auto->setText(QString::fromUtf8("スキャン設定を自動読み込み（Layer別）")); scan_auto->setCheckable(true); scan_auto->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); scan_auto->setSizePolicy(policy); }
  layout_121->addWidget(scan_auto);
  layout_121->setStretch(1,0);
  auto* layout_126=new QHBoxLayout();
  layout_126->setContentsMargins(0,0,0,0); layout_126->setSpacing(6);
  auto* w_127=new QPushButton();
  objects[QString::fromUtf8("w_127")]=w_127; w_127->setObjectName(QString::fromUtf8("w_127"));
  w_127->setEnabled(true);
  w_127->setMinimumSize(0,0); w_127->setMaximumSize(16777215,16777215);
  w_127->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_127->setText(QString::fromUtf8("設定JSONを選択")); w_127->setCheckable(false); w_127->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_127->setSizePolicy(policy); }
  layout_126->addWidget(w_127);
  layout_126->setStretch(0,0);
  auto* w_128=new QPushButton();
  objects[QString::fromUtf8("w_128")]=w_128; w_128->setObjectName(QString::fromUtf8("w_128"));
  w_128->setEnabled(true);
  w_128->setMinimumSize(0,0); w_128->setMaximumSize(16777215,16777215);
  w_128->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  w_128->setText(QString::fromUtf8("自動検出をやり直す")); w_128->setCheckable(false); w_128->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_128->setSizePolicy(policy); }
  layout_126->addWidget(w_128);
  layout_126->setStretch(1,0);
  layout_121->addLayout(layout_126);
  layout_121->setStretch(2,0);
  auto* scan_layer=new QSpinBox();
  objects[QString::fromUtf8("scan_layer")]=scan_layer; scan_layer->setObjectName(QString::fromUtf8("scan_layer"));
  scan_layer->setEnabled(true);
  scan_layer->setMinimumSize(0,0); scan_layer->setMaximumSize(16777215,16777215);
  scan_layer->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  scan_layer->setRange(-1,999); scan_layer->setSingleStep(1); scan_layer->setValue(-1); scan_layer->setKeyboardTracking(true);
  scan_layer->setPrefix(QString::fromUtf8("設定Layer ")); scan_layer->setSuffix(QString::fromUtf8("")); scan_layer->setSpecialValueText(QString::fromUtf8("設定Layer未指定"));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); scan_layer->setSizePolicy(policy); }
  layout_121->addWidget(scan_layer);
  layout_121->setStretch(3,0);
  auto* scan_source=new QLineEdit();
  objects[QString::fromUtf8("scan_source")]=scan_source; scan_source->setObjectName(QString::fromUtf8("scan_source"));
  scan_source->setEnabled(true);
  scan_source->setMinimumSize(0,0); scan_source->setMaximumSize(16777215,16777215);
  scan_source->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)));
  scan_source->setText(QString::fromUtf8("")); scan_source->setReadOnly(true); scan_source->setPlaceholderText(QString::fromUtf8("読み込み元の設定ファイル"));
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); scan_source->setSizePolicy(policy); }
  layout_121->addWidget(scan_source);
  layout_121->setStretch(4,0);
  auto* scan_info=new QLabel();
  objects[QString::fromUtf8("scan_info")]=scan_info; scan_info->setObjectName(QString::fromUtf8("scan_info"));
  scan_info->setEnabled(true);
  scan_info->setMinimumSize(0,0); scan_info->setMaximumSize(16777215,16777215);
  scan_info->setSizePolicy(QSizePolicy(QSizePolicy::Policy(13),QSizePolicy::Policy(5)));
  scan_info->setText(QString::fromUtf8("画像を開くとPARAMS/CombinedParam.jsonを検索します。")); scan_info->setWordWrap(true); scan_info->setTextInteractionFlags(Qt::TextInteractionFlags(4)); scan_info->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(13),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); scan_info->setSizePolicy(policy); }
  layout_121->addWidget(scan_info);
  layout_121->setStretch(5,0);
  auto* process_info=new QLabel();
  objects[QString::fromUtf8("process_info")]=process_info; process_info->setObjectName(QString::fromUtf8("process_info"));
  process_info->setEnabled(true);
  process_info->setMinimumSize(0,0); process_info->setMaximumSize(16777215,16777215);
  process_info->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  process_info->setText(QString::fromUtf8("処理は未適用。設定を変更したら「全層へ適用」を押してください。")); process_info->setWordWrap(true); process_info->setTextInteractionFlags(Qt::TextInteractionFlags(4)); process_info->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); process_info->setSizePolicy(policy); }
  layout_121->addWidget(process_info);
  layout_121->setStretch(6,0);
  auto* resize_check=new QCheckBox();
  objects[QString::fromUtf8("resize_check")]=resize_check; resize_check->setObjectName(QString::fromUtf8("resize_check"));
  resize_check->setEnabled(true);
  resize_check->setMinimumSize(0,0); resize_check->setMaximumSize(16777215,16777215);
  resize_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  resize_check->setText(QString::fromUtf8("画素補間")); resize_check->setCheckable(true); resize_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); resize_check->setSizePolicy(policy); }
  layout_121->addWidget(resize_check);
  layout_121->setStretch(7,0);
  auto* layout_134=new QFormLayout();
  layout_134->setContentsMargins(0,0,0,0); layout_134->setSpacing(6);
  layout_134->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_134->setFormAlignment(Qt::Alignment(33)); layout_134->setLabelAlignment(Qt::Alignment(1));
  auto* w_135=new QLabel();
  objects[QString::fromUtf8("w_135")]=w_135; w_135->setObjectName(QString::fromUtf8("w_135"));
  w_135->setEnabled(true);
  w_135->setMinimumSize(0,0); w_135->setMaximumSize(16777215,16777215);
  w_135->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_135->setText(QString::fromUtf8("補間倍率")); w_135->setWordWrap(false); w_135->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_135->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_135->setSizePolicy(policy); }
  layout_134->setWidget(0,QFormLayout::ItemRole(0),w_135);
  auto* scale_box=new QDoubleSpinBox();
  objects[QString::fromUtf8("scale_box")]=scale_box; scale_box->setObjectName(QString::fromUtf8("scale_box"));
  scale_box->setEnabled(true);
  scale_box->setMinimumSize(0,0); scale_box->setMaximumSize(16777215,16777215);
  scale_box->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  scale_box->setDecimals(4);
  scale_box->setRange(0.25,3.0); scale_box->setSingleStep(0.0001); scale_box->setValue(1.4142); scale_box->setKeyboardTracking(false);
  scale_box->setPrefix(QString::fromUtf8("")); scale_box->setSuffix(QString::fromUtf8("")); scale_box->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); scale_box->setSizePolicy(policy); }
  layout_134->setWidget(0,QFormLayout::ItemRole(1),scale_box);
  auto* w_137=new QLabel();
  objects[QString::fromUtf8("w_137")]=w_137; w_137->setObjectName(QString::fromUtf8("w_137"));
  w_137->setEnabled(true);
  w_137->setMinimumSize(0,0); w_137->setMaximumSize(16777215,16777215);
  w_137->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_137->setText(QString::fromUtf8("補間方式")); w_137->setWordWrap(false); w_137->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_137->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_137->setSizePolicy(policy); }
  layout_134->setWidget(1,QFormLayout::ItemRole(0),w_137);
  auto* interp_combo=new QComboBox();
  objects[QString::fromUtf8("interp_combo")]=interp_combo; interp_combo->setObjectName(QString::fromUtf8("interp_combo"));
  interp_combo->setEnabled(true);
  interp_combo->setMinimumSize(0,0); interp_combo->setMaximumSize(16777215,16777215);
  interp_combo->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  interp_combo->setEditable(false); interp_combo->setInsertPolicy(QComboBox::InsertPolicy(3)); interp_combo->setMaxVisibleItems(10); interp_combo->setMinimumContentsLength(0); interp_combo->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(1)); interp_combo->setIconSize(QSize(16,16));
  interp_combo->addItem(QString::fromUtf8("CPU cubic"),QVariant());
  interp_combo->addItem(QString::fromUtf8("GPU形式 A=-0.5"),QVariant());
  interp_combo->setCurrentIndex(1);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); interp_combo->setSizePolicy(policy); }
  layout_134->setWidget(1,QFormLayout::ItemRole(1),interp_combo);
  layout_121->addLayout(layout_134);
  layout_121->setStretch(8,0);
  auto* highpass_check=new QCheckBox();
  objects[QString::fromUtf8("highpass_check")]=highpass_check; highpass_check->setObjectName(QString::fromUtf8("highpass_check"));
  highpass_check->setEnabled(true);
  highpass_check->setMinimumSize(0,0); highpass_check->setMaximumSize(16777215,16777215);
  highpass_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  highpass_check->setText(QString::fromUtf8("2D filter：FIR背景 B − I")); highpass_check->setCheckable(true); highpass_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); highpass_check->setSizePolicy(policy); }
  layout_121->addWidget(highpass_check);
  layout_121->setStretch(9,0);
  auto* layout_140=new QFormLayout();
  layout_140->setContentsMargins(0,0,0,0); layout_140->setSpacing(6);
  layout_140->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_140->setFormAlignment(Qt::Alignment(33)); layout_140->setLabelAlignment(Qt::Alignment(1));
  auto* w_141=new QLabel();
  objects[QString::fromUtf8("w_141")]=w_141; w_141->setObjectName(QString::fromUtf8("w_141"));
  w_141->setEnabled(true);
  w_141->setMinimumSize(0,0); w_141->setMaximumSize(16777215,16777215);
  w_141->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_141->setText(QString::fromUtf8("FIR tap数（奇数）")); w_141->setWordWrap(false); w_141->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_141->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_141->setSizePolicy(policy); }
  layout_140->setWidget(0,QFormLayout::ItemRole(0),w_141);
  auto* tap_box=new QSpinBox();
  objects[QString::fromUtf8("tap_box")]=tap_box; tap_box->setObjectName(QString::fromUtf8("tap_box"));
  tap_box->setEnabled(true);
  tap_box->setMinimumSize(0,0); tap_box->setMaximumSize(16777215,16777215);
  tap_box->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  tap_box->setRange(3,101); tap_box->setSingleStep(1); tap_box->setValue(15); tap_box->setKeyboardTracking(false);
  tap_box->setPrefix(QString::fromUtf8("")); tap_box->setSuffix(QString::fromUtf8("")); tap_box->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); tap_box->setSizePolicy(policy); }
  layout_140->setWidget(0,QFormLayout::ItemRole(1),tap_box);
  auto* w_143=new QLabel();
  objects[QString::fromUtf8("w_143")]=w_143; w_143->setObjectName(QString::fromUtf8("w_143"));
  w_143->setEnabled(true);
  w_143->setMinimumSize(0,0); w_143->setMaximumSize(16777215,16777215);
  w_143->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_143->setText(QString::fromUtf8("NormCutOffFreq")); w_143->setWordWrap(false); w_143->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_143->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_143->setSizePolicy(policy); }
  layout_140->setWidget(1,QFormLayout::ItemRole(0),w_143);
  auto* cutoff_box=new QDoubleSpinBox();
  objects[QString::fromUtf8("cutoff_box")]=cutoff_box; cutoff_box->setObjectName(QString::fromUtf8("cutoff_box"));
  cutoff_box->setEnabled(true);
  cutoff_box->setMinimumSize(0,0); cutoff_box->setMaximumSize(16777215,16777215);
  cutoff_box->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  cutoff_box->setDecimals(3);
  cutoff_box->setRange(0.001,0.499); cutoff_box->setSingleStep(0.01); cutoff_box->setValue(0.1); cutoff_box->setKeyboardTracking(false);
  cutoff_box->setPrefix(QString::fromUtf8("")); cutoff_box->setSuffix(QString::fromUtf8("")); cutoff_box->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); cutoff_box->setSizePolicy(policy); }
  layout_140->setWidget(1,QFormLayout::ItemRole(1),cutoff_box);
  layout_121->addLayout(layout_140);
  layout_121->setStretch(10,0);
  auto* z_check=new QCheckBox();
  objects[QString::fromUtf8("z_check")]=z_check; z_check->setObjectName(QString::fromUtf8("z_check"));
  z_check->setEnabled(true);
  z_check->setMinimumSize(0,0); z_check->setMaximumSize(16777215,16777215);
  z_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  z_check->setText(QString::fromUtf8("Z filter")); z_check->setCheckable(true); z_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); z_check->setSizePolicy(policy); }
  layout_121->addWidget(z_check);
  layout_121->setStretch(11,0);
  auto* layout_146=new QFormLayout();
  layout_146->setContentsMargins(0,0,0,0); layout_146->setSpacing(6);
  layout_146->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_146->setFormAlignment(Qt::Alignment(33)); layout_146->setLabelAlignment(Qt::Alignment(1));
  auto* w_147=new QLabel();
  objects[QString::fromUtf8("w_147")]=w_147; w_147->setObjectName(QString::fromUtf8("w_147"));
  w_147->setEnabled(true);
  w_147->setMinimumSize(0,0); w_147->setMaximumSize(16777215,16777215);
  w_147->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_147->setText(QString::fromUtf8("Z filter係数")); w_147->setWordWrap(false); w_147->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_147->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_147->setSizePolicy(policy); }
  layout_146->setWidget(0,QFormLayout::ItemRole(0),w_147);
  auto* z_weight=new QDoubleSpinBox();
  objects[QString::fromUtf8("z_weight")]=z_weight; z_weight->setObjectName(QString::fromUtf8("z_weight"));
  z_weight->setEnabled(true);
  z_weight->setMinimumSize(0,0); z_weight->setMaximumSize(16777215,16777215);
  z_weight->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  z_weight->setDecimals(3);
  z_weight->setRange(-5.0,5.0); z_weight->setSingleStep(0.01); z_weight->setValue(-0.2); z_weight->setKeyboardTracking(false);
  z_weight->setPrefix(QString::fromUtf8("")); z_weight->setSuffix(QString::fromUtf8("")); z_weight->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); z_weight->setSizePolicy(policy); }
  layout_146->setWidget(0,QFormLayout::ItemRole(1),z_weight);
  auto* w_149=new QLabel();
  objects[QString::fromUtf8("w_149")]=w_149; w_149->setObjectName(QString::fromUtf8("w_149"));
  w_149->setEnabled(true);
  w_149->setMinimumSize(0,0); w_149->setMaximumSize(16777215,16777215);
  w_149->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_149->setText(QString::fromUtf8("Z参照間隔")); w_149->setWordWrap(false); w_149->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_149->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_149->setSizePolicy(policy); }
  layout_146->setWidget(1,QFormLayout::ItemRole(0),w_149);
  auto* z_step=new QComboBox();
  objects[QString::fromUtf8("z_step")]=z_step; z_step->setObjectName(QString::fromUtf8("z_step"));
  z_step->setEnabled(true);
  z_step->setMinimumSize(0,0); z_step->setMaximumSize(16777215,16777215);
  z_step->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)));
  z_step->setEditable(false); z_step->setInsertPolicy(QComboBox::InsertPolicy(3)); z_step->setMaxVisibleItems(10); z_step->setMinimumContentsLength(0); z_step->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy(1)); z_step->setIconSize(QSize(16,16));
  z_step->addItem(QString::fromUtf8("前後1層（通常）"),QVariant());
  z_step->addItem(QString::fromUtf8("前後2層（1層飛ばし）"),QVariant());
  z_step->setCurrentIndex(0);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); z_step->setSizePolicy(policy); }
  layout_146->setWidget(1,QFormLayout::ItemRole(1),z_step);
  layout_121->addLayout(layout_146);
  layout_121->setStretch(12,0);
  auto* binary_check=new QCheckBox();
  objects[QString::fromUtf8("binary_check")]=binary_check; binary_check->setObjectName(QString::fromUtf8("binary_check"));
  binary_check->setEnabled(true);
  binary_check->setMinimumSize(0,0); binary_check->setMaximumSize(16777215,16777215);
  binary_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  binary_check->setText(QString::fromUtf8("二値化（値 > 閾値を1）")); binary_check->setCheckable(true); binary_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); binary_check->setSizePolicy(policy); }
  layout_121->addWidget(binary_check);
  layout_121->setStretch(13,0);
  auto* layout_152=new QFormLayout();
  layout_152->setContentsMargins(0,0,0,0); layout_152->setSpacing(6);
  layout_152->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_152->setFormAlignment(Qt::Alignment(33)); layout_152->setLabelAlignment(Qt::Alignment(1));
  auto* w_153=new QLabel();
  objects[QString::fromUtf8("w_153")]=w_153; w_153->setObjectName(QString::fromUtf8("w_153"));
  w_153->setEnabled(true);
  w_153->setMinimumSize(0,0); w_153->setMaximumSize(16777215,16777215);
  w_153->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_153->setText(QString::fromUtf8("二値化閾値")); w_153->setWordWrap(false); w_153->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_153->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_153->setSizePolicy(policy); }
  layout_152->setWidget(0,QFormLayout::ItemRole(0),w_153);
  auto* threshold_box=new QDoubleSpinBox();
  objects[QString::fromUtf8("threshold_box")]=threshold_box; threshold_box->setObjectName(QString::fromUtf8("threshold_box"));
  threshold_box->setEnabled(true);
  threshold_box->setMinimumSize(0,0); threshold_box->setMaximumSize(16777215,16777215);
  threshold_box->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  threshold_box->setDecimals(2);
  threshold_box->setRange(-1.0,255.0); threshold_box->setSingleStep(1.0); threshold_box->setValue(9.0); threshold_box->setKeyboardTracking(false);
  threshold_box->setPrefix(QString::fromUtf8("")); threshold_box->setSuffix(QString::fromUtf8("")); threshold_box->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); threshold_box->setSizePolicy(policy); }
  layout_152->setWidget(0,QFormLayout::ItemRole(1),threshold_box);
  layout_121->addLayout(layout_152);
  layout_121->setStretch(14,0);
  auto* dilate_check=new QCheckBox();
  objects[QString::fromUtf8("dilate_check")]=dilate_check; dilate_check->setObjectName(QString::fromUtf8("dilate_check"));
  dilate_check->setEnabled(true);
  dilate_check->setMinimumSize(0,0); dilate_check->setMaximumSize(16777215,16777215);
  dilate_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  dilate_check->setText(QString::fromUtf8("膨張")); dilate_check->setCheckable(true); dilate_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); dilate_check->setSizePolicy(policy); }
  layout_121->addWidget(dilate_check);
  layout_121->setStretch(15,0);
  auto* layout_156=new QFormLayout();
  layout_156->setContentsMargins(0,0,0,0); layout_156->setSpacing(6);
  layout_156->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy(2)); layout_156->setFormAlignment(Qt::Alignment(33)); layout_156->setLabelAlignment(Qt::Alignment(1));
  auto* w_157=new QLabel();
  objects[QString::fromUtf8("w_157")]=w_157; w_157->setObjectName(QString::fromUtf8("w_157"));
  w_157->setEnabled(true);
  w_157->setMinimumSize(0,0); w_157->setMaximumSize(16777215,16777215);
  w_157->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_157->setText(QString::fromUtf8("膨張 n×n")); w_157->setWordWrap(false); w_157->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_157->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_157->setSizePolicy(policy); }
  layout_156->setWidget(0,QFormLayout::ItemRole(0),w_157);
  auto* dilate_box=new QSpinBox();
  objects[QString::fromUtf8("dilate_box")]=dilate_box; dilate_box->setObjectName(QString::fromUtf8("dilate_box"));
  dilate_box->setEnabled(true);
  dilate_box->setMinimumSize(0,0); dilate_box->setMaximumSize(16777215,16777215);
  dilate_box->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  dilate_box->setRange(1,31); dilate_box->setSingleStep(1); dilate_box->setValue(2); dilate_box->setKeyboardTracking(false);
  dilate_box->setPrefix(QString::fromUtf8("")); dilate_box->setSuffix(QString::fromUtf8("")); dilate_box->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); dilate_box->setSizePolicy(policy); }
  layout_156->setWidget(0,QFormLayout::ItemRole(1),dilate_box);
  layout_121->addLayout(layout_156);
  layout_121->setStretch(16,0);
  auto* process_button=new QPushButton();
  objects[QString::fromUtf8("process_button")]=process_button; process_button->setObjectName(QString::fromUtf8("process_button"));
  process_button->setEnabled(false);
  process_button->setMinimumSize(0,0); process_button->setMaximumSize(16777215,16777215);
  process_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  process_button->setText(QString::fromUtf8("設定を現在のspngの全層へ適用")); process_button->setCheckable(false); process_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); process_button->setSizePolicy(policy); }
  layout_121->addWidget(process_button);
  layout_121->setStretch(17,0);
  auto* w_160=new QLabel();
  objects[QString::fromUtf8("w_160")]=w_160; w_160->setObjectName(QString::fromUtf8("w_160"));
  w_160->setEnabled(true);
  w_160->setMinimumSize(0,0); w_160->setMaximumSize(16777215,16777215);
  w_160->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_160->setText(QString::fromUtf8("8bitグレースケールに対応。全層と全処理段階を一度計算し、ディスクキャッシュから表示します。元データは変更しません。")); w_160->setWordWrap(true); w_160->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_160->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_160->setSizePolicy(policy); }
  layout_121->addWidget(w_160);
  layout_121->setStretch(18,0);
  auto* w_161=new QLabel();
  objects[QString::fromUtf8("w_161")]=w_161; w_161->setObjectName(QString::fromUtf8("w_161"));
  w_161->setEnabled(true);
  w_161->setMinimumSize(0,0); w_161->setMaximumSize(16777215,16777215);
  w_161->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_161->setText(QString::fromUtf8("補間 → 2D → Z → 二値化 → 膨張。途中はuint8へ丸め・飽和。二値画像は0/1。XY端はreflect101、Z端は片側の層を全係数で使用し、欠損隣接層は中心層で代用します。")); w_161->setWordWrap(true); w_161->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_161->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_161->setSizePolicy(policy); }
  layout_121->addWidget(w_161);
  layout_121->setStretch(19,0);
  auto* w_162=new QLabel();
  objects[QString::fromUtf8("w_162")]=w_162; w_162->setObjectName(QString::fromUtf8("w_162"));
  w_162->setEnabled(true);
  w_162->setMinimumSize(0,0); w_162->setMaximumSize(16777215,16777215);
  w_162->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_162->setText(QString::fromUtf8("前後2層は偶数層・奇数層を別々に処理します。16層模擬では再生・出力も0-31:2など同じ偶奇で選択してください。")); w_162->setWordWrap(true); w_162->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_162->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_162->setSizePolicy(policy); }
  layout_121->addWidget(w_162);
  layout_121->setStretch(20,0);
  auto* w_163=new QLabel();
  objects[QString::fromUtf8("w_163")]=w_163; w_163->setObjectName(QString::fromUtf8("w_163"));
  w_163->setEnabled(true);
  w_163->setMinimumSize(0,0); w_163->setMaximumSize(16777215,16777215);
  w_163->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_163->setText(QString::fromUtf8("可視化用フィルタです。GPU形式のA=-0.5補間をCPUで模擬します。dead pixel補正・affine・走査マスクには対応していません。")); w_163->setWordWrap(true); w_163->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_163->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_163->setSizePolicy(policy); }
  layout_121->addWidget(w_163);
  layout_121->setStretch(21,0);
  layout_121->addItem(new QSpacerItem(0,0,QSizePolicy::Policy(1),QSizePolicy::Policy(7)));
  layout_121->setStretch(22,0);
  w_120->setLayout(layout_121);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_120->setSizePolicy(policy); }
  processing_page->setWidgetResizable(true); processing_page->setWidget(w_120);
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); processing_page->setSizePolicy(policy); }
  feature_tabs->addTab(processing_page,QString::fromUtf8("画像処理"));
  feature_tabs->setCurrentIndex(0);
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); feature_tabs->setSizePolicy(policy); }
  w_001->addWidget(feature_tabs);
  auto* w_164=new QWidget();
  objects[QString::fromUtf8("w_164")]=w_164; w_164->setObjectName(QString::fromUtf8("w_164"));
  w_164->setEnabled(true);
  w_164->setMinimumSize(0,0); w_164->setMaximumSize(16777215,16777215);
  w_164->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  auto* layout_165=new QVBoxLayout();
  layout_165->setContentsMargins(8,10,12,8); layout_165->setSpacing(6);
  auto* filename=new QLabel();
  objects[QString::fromUtf8("filename")]=filename; filename->setObjectName(QString::fromUtf8("filename"));
  filename->setEnabled(true);
  filename->setMinimumSize(0,0); filename->setMaximumSize(16777215,16777215);
  filename->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  filename->setText(QString::fromUtf8("画像を開くと、ここにファイル名が表示されます。")); filename->setWordWrap(true); filename->setTextInteractionFlags(Qt::TextInteractionFlags(1)); filename->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); filename->setSizePolicy(policy); }
  layout_165->addWidget(filename);
  layout_165->setStretch(0,0);
  auto* view=new ImageView();
  objects[QString::fromUtf8("view")]=view; view->setObjectName(QString::fromUtf8("view"));
  view->setEnabled(true);
  view->setMinimumSize(300,250); view->setMaximumSize(16777215,16777215);
  view->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)));
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(7)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); view->setSizePolicy(policy); }
  layout_165->addWidget(view);
  layout_165->setStretch(1,1);
  auto* layout_168=new QHBoxLayout();
  layout_168->setContentsMargins(0,0,0,0); layout_168->setSpacing(6);
  auto* play_button=new QPushButton();
  objects[QString::fromUtf8("play_button")]=play_button; play_button->setObjectName(QString::fromUtf8("play_button"));
  play_button->setEnabled(false);
  play_button->setMinimumSize(0,0); play_button->setMaximumSize(16777215,16777215);
  play_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  play_button->setText(QString::fromUtf8("▶ 再生")); play_button->setCheckable(false); play_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); play_button->setSizePolicy(policy); }
  layout_168->addWidget(play_button);
  layout_168->setStretch(0,0);
  auto* play_rate=new QDoubleSpinBox();
  objects[QString::fromUtf8("play_rate")]=play_rate; play_rate->setObjectName(QString::fromUtf8("play_rate"));
  play_rate->setEnabled(true);
  play_rate->setMinimumSize(0,0); play_rate->setMaximumSize(16777215,16777215);
  play_rate->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  play_rate->setDecimals(1);
  play_rate->setRange(0.2,60.0); play_rate->setSingleStep(1.0); play_rate->setValue(8.0); play_rate->setKeyboardTracking(false);
  play_rate->setPrefix(QString::fromUtf8("")); play_rate->setSuffix(QString::fromUtf8(" 層/秒")); play_rate->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); play_rate->setSizePolicy(policy); }
  layout_168->addWidget(play_rate);
  layout_168->setStretch(1,0);
  auto* loop_check=new QCheckBox();
  objects[QString::fromUtf8("loop_check")]=loop_check; loop_check->setObjectName(QString::fromUtf8("loop_check"));
  loop_check->setEnabled(true);
  loop_check->setMinimumSize(0,0); loop_check->setMaximumSize(16777215,16777215);
  loop_check->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  loop_check->setText(QString::fromUtf8("ループ")); loop_check->setCheckable(true); loop_check->setChecked(true);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); loop_check->setSizePolicy(policy); }
  layout_168->addWidget(loop_check);
  layout_168->setStretch(2,0);
  layout_168->addItem(new QSpacerItem(0,0,QSizePolicy::Policy(7),QSizePolicy::Policy(1)));
  layout_168->setStretch(3,0);
  layout_165->addLayout(layout_168);
  layout_165->setStretch(2,0);
  auto* layout_172=new QHBoxLayout();
  layout_172->setContentsMargins(0,0,0,0); layout_172->setSpacing(6);
  auto* prev_button=new QPushButton();
  objects[QString::fromUtf8("prev_button")]=prev_button; prev_button->setObjectName(QString::fromUtf8("prev_button"));
  prev_button->setEnabled(false);
  prev_button->setMinimumSize(0,0); prev_button->setMaximumSize(16777215,16777215);
  prev_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  prev_button->setText(QString::fromUtf8("◀ 前の層")); prev_button->setCheckable(false); prev_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); prev_button->setSizePolicy(policy); }
  layout_172->addWidget(prev_button);
  layout_172->setStretch(0,0);
  auto* layer_slider=new QSlider();
  objects[QString::fromUtf8("layer_slider")]=layer_slider; layer_slider->setObjectName(QString::fromUtf8("layer_slider"));
  layer_slider->setEnabled(false);
  layer_slider->setMinimumSize(0,0); layer_slider->setMaximumSize(16777215,16777215);
  layer_slider->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)));
  layer_slider->setOrientation(Qt::Orientation(1)); layer_slider->setRange(0,0); layer_slider->setValue(0);
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); layer_slider->setSizePolicy(policy); }
  layout_172->addWidget(layer_slider);
  layout_172->setStretch(1,1);
  auto* w_175=new QLabel();
  objects[QString::fromUtf8("w_175")]=w_175; w_175->setObjectName(QString::fromUtf8("w_175"));
  w_175->setEnabled(true);
  w_175->setMinimumSize(0,0); w_175->setMaximumSize(16777215,16777215);
  w_175->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  w_175->setText(QString::fromUtf8("層")); w_175->setWordWrap(false); w_175->setTextInteractionFlags(Qt::TextInteractionFlags(4)); w_175->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_175->setSizePolicy(policy); }
  layout_172->addWidget(w_175);
  layout_172->setStretch(2,0);
  auto* layer_number=new QSpinBox();
  objects[QString::fromUtf8("layer_number")]=layer_number; layer_number->setObjectName(QString::fromUtf8("layer_number"));
  layer_number->setEnabled(false);
  layer_number->setMinimumSize(0,0); layer_number->setMaximumSize(16777215,16777215);
  layer_number->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  layer_number->setRange(0,0); layer_number->setSingleStep(1); layer_number->setValue(0); layer_number->setKeyboardTracking(false);
  layer_number->setPrefix(QString::fromUtf8("")); layer_number->setSuffix(QString::fromUtf8("")); layer_number->setSpecialValueText(QString::fromUtf8(""));
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); layer_number->setSizePolicy(policy); }
  layout_172->addWidget(layer_number);
  layout_172->setStretch(3,0);
  auto* layer_range=new QLabel();
  objects[QString::fromUtf8("layer_range")]=layer_range; layer_range->setObjectName(QString::fromUtf8("layer_range"));
  layer_range->setEnabled(true);
  layer_range->setMinimumSize(0,0); layer_range->setMaximumSize(16777215,16777215);
  layer_range->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  layer_range->setText(QString::fromUtf8("/ ―")); layer_range->setWordWrap(false); layer_range->setTextInteractionFlags(Qt::TextInteractionFlags(4)); layer_range->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); layer_range->setSizePolicy(policy); }
  layout_172->addWidget(layer_range);
  layout_172->setStretch(4,0);
  auto* next_button=new QPushButton();
  objects[QString::fromUtf8("next_button")]=next_button; next_button->setObjectName(QString::fromUtf8("next_button"));
  next_button->setEnabled(false);
  next_button->setMinimumSize(0,0); next_button->setMaximumSize(16777215,16777215);
  next_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  next_button->setText(QString::fromUtf8("次の層 ▶")); next_button->setCheckable(false); next_button->setChecked(false);
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); next_button->setSizePolicy(policy); }
  layout_172->addWidget(next_button);
  layout_172->setStretch(5,0);
  layout_165->addLayout(layout_172);
  layout_165->setStretch(3,0);
  auto* layout_179=new QHBoxLayout();
  layout_179->setContentsMargins(0,0,0,0); layout_179->setSpacing(6);
  auto* progress_bar=new QProgressBar();
  objects[QString::fromUtf8("progress_bar")]=progress_bar; progress_bar->setObjectName(QString::fromUtf8("progress_bar"));
  progress_bar->setEnabled(true);
  progress_bar->setMinimumSize(0,0); progress_bar->setMaximumSize(16777215,16777215);
  progress_bar->setSizePolicy(QSizePolicy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)));
  progress_bar->setRange(0,100); progress_bar->setValue(-1);
  progress_bar->hide();
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); progress_bar->setSizePolicy(policy); }
  layout_179->addWidget(progress_bar);
  layout_179->setStretch(0,1);
  auto* cancel_button=new QPushButton();
  objects[QString::fromUtf8("cancel_button")]=cancel_button; cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
  cancel_button->setEnabled(true);
  cancel_button->setMinimumSize(0,0); cancel_button->setMaximumSize(16777215,16777215);
  cancel_button->setSizePolicy(QSizePolicy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)));
  cancel_button->setText(QString::fromUtf8("中止")); cancel_button->setCheckable(false); cancel_button->setChecked(false);
  cancel_button->hide();
  { QSizePolicy policy(QSizePolicy::Policy(1),QSizePolicy::Policy(0)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); cancel_button->setSizePolicy(policy); }
  layout_179->addWidget(cancel_button);
  layout_179->setStretch(1,0);
  layout_165->addLayout(layout_179);
  layout_165->setStretch(4,0);
  auto* state_label=new QLabel();
  objects[QString::fromUtf8("state_label")]=state_label; state_label->setObjectName(QString::fromUtf8("state_label"));
  state_label->setEnabled(true);
  state_label->setMinimumSize(0,0); state_label->setMaximumSize(16777215,16777215);
  state_label->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  state_label->setText(QString::fromUtf8("準備完了")); state_label->setWordWrap(true); state_label->setTextInteractionFlags(Qt::TextInteractionFlags(4)); state_label->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(true); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); state_label->setSizePolicy(policy); }
  layout_165->addWidget(state_label);
  layout_165->setStretch(5,0);
  auto* cursor_label=new QLabel();
  objects[QString::fromUtf8("cursor_label")]=cursor_label; cursor_label->setObjectName(QString::fromUtf8("cursor_label"));
  cursor_label->setEnabled(true);
  cursor_label->setStyleSheet(QString::fromUtf8("background:#eaf2f6;padding:8px;font-size:14px;color:#16384d;"));
  cursor_label->setMinimumSize(0,0); cursor_label->setMaximumSize(16777215,16777215);
  cursor_label->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  cursor_label->setText(QString::fromUtf8("カーソル［左下原点・pixel］ X: ―　Y: ―　画素値: ―")); cursor_label->setWordWrap(false); cursor_label->setTextInteractionFlags(Qt::TextInteractionFlags(1)); cursor_label->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); cursor_label->setSizePolicy(policy); }
  layout_165->addWidget(cursor_label);
  layout_165->setStretch(6,0);
  w_164->setLayout(layout_165);
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(1); policy.setVerticalStretch(1); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_164->setSizePolicy(policy); }
  w_001->addWidget(w_164);
  w_001->setSizes({460,990}); w_001->setStretchFactor(0,0); w_001->setStretchFactor(1,1);
  { QSizePolicy policy(QSizePolicy::Policy(7),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); w_001->setSizePolicy(policy); }
  win->setCentralWidget(w_001);
  auto* toolbar_184=new QToolBar(QString::fromUtf8("ファイルと表示"),win); toolbar_184->setMovable(false); win->addToolBar(toolbar_184);
  toolbar_184->setIconSize(QSize(24,24)); toolbar_184->setToolButtonStyle(Qt::ToolButtonStyle(0));
  auto* open_action=toolbar_184->addAction(QString::fromUtf8("ファイルを開く")); objects[QString::fromUtf8("open_action")]=open_action;
  open_action->setSeparator(false); open_action->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+O"))); open_action->setEnabled(true);
  auto* toolbar_action_186=toolbar_184->addAction(QString::fromUtf8("フォルダを開く")); objects[QString::fromUtf8("toolbar_action_186")]=toolbar_action_186;
  toolbar_action_186->setSeparator(false); toolbar_action_186->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+Shift+O"))); toolbar_action_186->setEnabled(true);
  auto* toolbar_action_187=toolbar_184->addSeparator(); objects[QString::fromUtf8("toolbar_action_187")]=toolbar_action_187;
  toolbar_action_187->setSeparator(true); toolbar_action_187->setShortcut(QKeySequence(QString::fromUtf8(""))); toolbar_action_187->setEnabled(true);
  auto* full_action=toolbar_184->addAction(QString::fromUtf8("表示層をPNG保存")); objects[QString::fromUtf8("full_action")]=full_action;
  full_action->setSeparator(false); full_action->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+S"))); full_action->setEnabled(false);
  auto* crop_action=toolbar_184->addAction(QString::fromUtf8("選択範囲をPNG保存")); objects[QString::fromUtf8("crop_action")]=crop_action;
  crop_action->setSeparator(false); crop_action->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+Shift+S"))); crop_action->setEnabled(false);
  auto* toolbar_action_190=toolbar_184->addSeparator(); objects[QString::fromUtf8("toolbar_action_190")]=toolbar_action_190;
  toolbar_action_190->setSeparator(true); toolbar_action_190->setShortcut(QKeySequence(QString::fromUtf8(""))); toolbar_action_190->setEnabled(true);
  auto* toolbar_action_191=toolbar_184->addAction(QString::fromUtf8("全体表示")); objects[QString::fromUtf8("toolbar_action_191")]=toolbar_action_191;
  toolbar_action_191->setSeparator(false); toolbar_action_191->setShortcut(QKeySequence(QString::fromUtf8("F"))); toolbar_action_191->setEnabled(true);
  auto* toolbar_action_192=toolbar_184->addAction(QString::fromUtf8("100%")); objects[QString::fromUtf8("toolbar_action_192")]=toolbar_action_192;
  toolbar_action_192->setSeparator(false); toolbar_action_192->setShortcut(QKeySequence(QString::fromUtf8("1"))); toolbar_action_192->setEnabled(true);
  auto* zoom_label=new QLabel();
  objects[QString::fromUtf8("zoom_label")]=zoom_label; zoom_label->setObjectName(QString::fromUtf8("zoom_label"));
  zoom_label->setEnabled(true);
  zoom_label->setMinimumSize(0,0); zoom_label->setMaximumSize(16777215,16777215);
  zoom_label->setSizePolicy(QSizePolicy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)));
  zoom_label->setText(QString::fromUtf8("  ― % ")); zoom_label->setWordWrap(false); zoom_label->setTextInteractionFlags(Qt::TextInteractionFlags(4)); zoom_label->setAlignment(Qt::Alignment(129));
  { QSizePolicy policy(QSizePolicy::Policy(5),QSizePolicy::Policy(5)); policy.setHorizontalStretch(0); policy.setVerticalStretch(0); policy.setHeightForWidth(false); policy.setWidthForHeight(false); policy.setRetainSizeWhenHidden(false); zoom_label->setSizePolicy(policy); }
  toolbar_184->addWidget(zoom_label);
  auto* toolbar_action_194=toolbar_184->addSeparator(); objects[QString::fromUtf8("toolbar_action_194")]=toolbar_action_194;
  toolbar_action_194->setSeparator(true); toolbar_action_194->setShortcut(QKeySequence(QString::fromUtf8(""))); toolbar_action_194->setEnabled(true);
  auto* toolbar_action_195=toolbar_184->addAction(QString::fromUtf8("使い方")); objects[QString::fromUtf8("toolbar_action_195")]=toolbar_action_195;
  toolbar_action_195->setSeparator(false); toolbar_action_195->setShortcut(QKeySequence(QString::fromUtf8(""))); toolbar_action_195->setEnabled(true);
  auto* shortcut_196=new QAction(win); shortcut_196->setShortcut(QKeySequence(QString::fromUtf8("Left"))); win->addAction(shortcut_196); objects[QString::fromUtf8("shortcut_196")]=shortcut_196;
  auto* shortcut_197=new QAction(win); shortcut_197->setShortcut(QKeySequence(QString::fromUtf8("Right"))); win->addAction(shortcut_197); objects[QString::fromUtf8("shortcut_197")]=shortcut_197;
  auto* shortcut_198=new QAction(win); shortcut_198->setShortcut(QKeySequence(QString::fromUtf8("PgUp"))); win->addAction(shortcut_198); objects[QString::fromUtf8("shortcut_198")]=shortcut_198;
  auto* shortcut_199=new QAction(win); shortcut_199->setShortcut(QKeySequence(QString::fromUtf8("PgDown"))); win->addAction(shortcut_199); objects[QString::fromUtf8("shortcut_199")]=shortcut_199;
  auto* shortcut_200=new QAction(win); shortcut_200->setShortcut(QKeySequence(QString::fromUtf8("Esc"))); win->addAction(shortcut_200); objects[QString::fromUtf8("shortcut_200")]=shortcut_200;
  win->setWindowTitle(QStringLiteral("SPNG Viewer")); win->resize(1450,950); win->setAcceptDrops(true);
}
}
