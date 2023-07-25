#include <QApplication>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QAudioOutput>
#include <QLineSeries>
#include <QStandardPaths>
#include "QTWindow.h"
#include "QGoProFiles.h"
#include "QWavesModel.h"
#include "QVersion.h"

#ifdef _WIN32

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(__argc, __argv);

#else
    int main(int argc, char* argv[]) {
        QApplication app(argc, argv);
#endif

    QMainWindow mainWindow;
    Ui_MainWindow mainWindowUi{};
    mainWindowUi.setupUi(&mainWindow);

    checkNewVersion(&mainWindow);

    QFileSystemModel dirsModel;
    QGoProFiles filesModel((float) mainWindowUi.minSpeedSpinBox->value(), mainWindowUi.minDurationSpinBox->value());
    QWavesModel wavesModel;
    QMediaPlayer player;
    QGraphicsLineItem *marker{};

    auto setEnabledAll = [&](bool enabled) {
        mainWindowUi.directoriesTreeView->setEnabled(enabled);
        mainWindowUi.filesTable->setEnabled(enabled);
        mainWindowUi.wavesTable->setEnabled(enabled);
        mainWindowUi.minSpeedSpinBox->setEnabled(enabled);
        mainWindowUi.minDurationSpinBox->setEnabled(enabled);
        mainWindowUi.playerButton->setEnabled(enabled && mainWindowUi.filesTable->selectionModel()->hasSelection());
        mainWindowUi.playerSlider->setEnabled(enabled && mainWindowUi.filesTable->selectionModel()->hasSelection());
    };

    QFileIconProvider fsIconProvider;
    dirsModel.setIconProvider(&fsIconProvider);
    dirsModel.setRootPath("");
    dirsModel.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Drives);
    mainWindowUi.directoriesTreeView->setModel(&dirsModel);
    mainWindowUi.directoriesTreeView->hideColumn(1);
    mainWindowUi.directoriesTreeView->hideColumn(2);
    mainWindowUi.directoriesTreeView->hideColumn(3);
    mainWindowUi.directoriesTreeView->setHeaderHidden(true);
    QModelIndex rootIndex = dirsModel.index(dirsModel.rootPath());
    mainWindowUi.directoriesTreeView->expand(rootIndex);

    // Dirs tree
    QModelIndex directoryIndex = dirsModel.index(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
    mainWindowUi.directoriesTreeView->setCurrentIndex(directoryIndex);
    mainWindowUi.directoriesTreeView->scrollTo(directoryIndex);

    // Files table
    filesModel.setPath(dirsModel.filePath(directoryIndex));
    mainWindowUi.filesTable->setModel(&filesModel);
    mainWindowUi.filesTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    // Waves view
    mainWindowUi.wavesTable->setModel(&wavesModel);

    // Player
    player.setVideoOutput(mainWindowUi.videoWidget);
    QAudioOutput audioOutput;
    player.setAudioOutput(&audioOutput);
    player.setLoops(QMediaPlayer::Infinite);

    // Directory
    QObject::connect(mainWindowUi.directoriesTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [&](const QItemSelection &selected, const QItemSelection &deselected) {
                         setEnabledAll(false);
                         mainWindowUi.filesTable->clearSelection();
                         QModelIndexList indexes = selected.indexes();
                         QString path = indexes.isEmpty() ? "" : dirsModel.filePath(indexes.first());

                         filesModel.setPath(path);
                         setEnabledAll(true);
                     });

    // Files
    QObject::connect(mainWindowUi.filesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [&](const QItemSelection &selected, const QItemSelection &deselected) {
                         auto indexes = selected.indexes();
                         if (!indexes.isEmpty()) {
                             auto profile = filesModel.getProfile(indexes.first());

                             wavesModel.setData(profile);

                             auto *series = new QLineSeries;
                             auto i = 0;
                             float maxSpeed = 0;
                             for (const auto &speed: profile.gpsInfo.GetAvgSpeeds3d()) {
                                 series->append(i++, speed * 3.6);
                                 if (speed > maxSpeed)
                                     maxSpeed = speed;
                             }

                             auto *chart = new QChart();
                             chart->legend()->hide();
                             chart->addSeries(series);
                             chart->createDefaultAxes();
                             chart->axes(Qt::Vertical)[0]->setTitleText("km/h");
                             chart->axes(Qt::Vertical)[0]->setTruncateLabels(false);

                             mainWindowUi.chartView->setChart(chart);

                             QPen pen(Qt::red);
                             pen.setWidth(1);
                             if (marker)
                                 mainWindowUi.chartView->scene()->removeItem(marker);
                             marker = new QGraphicsLineItem();
                             marker->setPen(pen);

                             auto scenePos1 = chart->mapToPosition(QPointF(0, 0));
                             auto scenePos2 = chart->mapToPosition(QPointF(0, maxSpeed * 3.6));
                             marker->setLine(QLineF(0, scenePos1.y(), 0, scenePos2.y()));

                             mainWindowUi.chartView->scene()->addItem(marker);

                             player.setSource(QUrl::fromLocalFile(profile.fileInfo.absoluteFilePath()));
                         } else {
                             goProFile data;
                             wavesModel.setData(data);
                             player.stop();
                             player.setSource(QUrl(""));
                         }

                         setEnabledAll(true);
                     });

    // Waves
    QObject::connect(mainWindowUi.wavesTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                     [&](const QItemSelection &selected, const QItemSelection &deselected) {
                         auto indexes = selected.indexes();
                         if (!indexes.isEmpty()) {
                             player.setPosition((qint64) wavesModel.getWave(indexes.first()).start * 1000);
                         }
                     });

    QObject::connect(mainWindowUi.wavesTable, &QAbstractItemView::clicked,
                     [&](const QModelIndex &index) {
                         if (index.isValid()) {
                             player.setPosition((qint64) wavesModel.getWave(index).start * 1000);
                         }
                     });

    // Detect settings
    QObject::connect(mainWindowUi.minSpeedSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                     [&](double value) {
                         setEnabledAll(false);
                         filesModel.setMinSpeed((float) value);
                         setEnabledAll(true);
                     });

    QObject::connect(mainWindowUi.minDurationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     [&](int value) {
                         setEnabledAll(false);
                         filesModel.setMinDuration(value);
                         setEnabledAll(true);
                     });

    // Player
    QObject::connect(&player, &QMediaPlayer::errorOccurred, [&]() {
        QMessageBox::critical(nullptr, "Error", "Err" + player.errorString());
    });

    QObject::connect(&player, &QMediaPlayer::mediaStatusChanged, [&](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::LoadedMedia) {
            mainWindowUi.playerSlider->setRange(0, (int) player.duration());
            mainWindowUi.playerButton->setText("▶");

            //ToDo: Hack to avoid sound delay, doesn't work well
            player.audioOutput()->setMuted(true);
            player.play();
            player.pause();
            player.audioOutput()->setMuted(false);
        }
    });

    QObject::connect(&player, &QMediaPlayer::positionChanged, [&](qint64 position) {
        mainWindowUi.playerSlider->setValue((int) position);
        marker->setX(mainWindowUi.chartView->chart()->mapToPosition(QPointF((qreal) position / 1000, 0)).x());
    });

    // Slider
    QObject::connect(mainWindowUi.playerSlider, &QSlider::valueChanged, &player, &QMediaPlayer::setPosition);

    // Play Pause Button
    QObject::connect(mainWindowUi.playerButton, &QPushButton::clicked, [&]() {
        if (mainWindowUi.playerButton->text() == "▶") {
            mainWindowUi.playerButton->setText("⏸");
            player.play();
        } else {
            mainWindowUi.playerButton->setText("▶");
            player.pause();
        }
    });

    mainWindow.show();

    return QApplication::exec();
}
