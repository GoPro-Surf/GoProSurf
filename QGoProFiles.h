//
// Created by svistunov on 16.07.23.
//

#ifndef GOPROSURFSPLITTER_QGOPROFILES_H
#define GOPROSURFSPLITTER_QGOPROFILES_H

#include "GpsInfo.h"
#include "Detector.h"
#include <QFileInfo.h>
#include <QAbstractItemModel>
#include <QBrush>
#include <QDir>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QCoreApplication>

struct goProFile {
    QFileInfo fileInfo;
    GpsInfo gpsInfo;
    bool noGpsInfo;
    std::vector<timeFrame> waves;
};

class QGoProFiles : public QAbstractItemModel {
public:
    explicit QGoProFiles(float minSpeed, size_t minDuration, QObject *parent = nullptr)
            : QAbstractItemModel(parent), minSpeed(minSpeed), minDuration(minDuration) {}

    void setPath(const QString &path) {
        fsModel.setRootPath(path);
        update();
    }

    void setMinSpeed(const float value) {
        minSpeed = value;
        update();
    }

    void setMinDuration(const size_t value) {
        minDuration = value;
        update();
    }

    goProFile getProfile(QModelIndex index) {
        return files[index.row()];
    }

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override {
        return createIndex(row, column);
    }

    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override {
        return {};
    }

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override {
        return (int) files.size();
    }

    [[nodiscard]] int columnCount(const QModelIndex &parent) const override {
        return (int) tableHeader.size();
    }

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override {
        if (index.row() >= files.size())
            return {};

        QTime t(0, 0, 0);
        size_t sum = 0;

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0:
                    return files[index.row()].fileInfo.fileName();
                case 1:
                    if (!files[index.row()].gpsInfo.GetTs().isNull())
                        return files[index.row()].gpsInfo.GetTs();

                    return files[index.row()].fileInfo.birthTime();
                case 2:
                    for (const auto &tf: files[index.row()].waves)
                        sum += tf.finish - tf.start;

                    return QString("%1 [%2]")
                            .arg((int) files[index.row()].waves.size())
                            .arg(t.addSecs((int) sum).toString("mm:ss"));
                case 3:
                    return t.addSecs((int) files[index.row()].gpsInfo.GetVideoDuration()).toString("mm:ss");
                default:
                    return QString("-");
            }

        } else if (role == Qt::DecorationRole && index.column() == 0) {
            return fsIconProvider.icon(files[index.row()].fileInfo);

        } else if (role == Qt::ForegroundRole) {
            if (files[index.row()].noGpsInfo)
                return QBrush(Qt::GlobalColor::darkRed);
            else if (files[index.row()].waves.empty())
                return QBrush(Qt::GlobalColor::gray);

        } else if (role == Qt::ToolTipRole && files[index.row()].noGpsInfo) {
            return QString("No GPS data was found");

        }

        return {};
    }

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole) {
            if (orientation == Qt::Horizontal) {
                return tableHeader[section];
            }
        }
        return {};
    }

private:
    void update() {
        beginResetModel();

        files.clear();

        auto filesList = fsModel.rootDirectory().entryInfoList(QDir::Files, QDir::Name);
        for (const auto &fileInfo: filesList) {
            QCoreApplication::processEvents();

            if (fileInfo.filesystemFilePath().extension() == ".MP4") {
                auto inputFile = fileInfo.absoluteFilePath().toStdString();
                GpsInfo gpsInfo;
                auto noGpsInfo = false;
                std::vector<timeFrame> waves;

                try {
                    gpsInfo.ReadFromVideo(inputFile);
                    waves = Detector::GetWaves(gpsInfo, 0, minDuration, minSpeed);
                } catch (...) {
                    noGpsInfo = true;
                }
                files.push_back({fileInfo, gpsInfo, noGpsInfo, waves});
            }
        }

        std::sort(files.begin(), files.end(), [](const goProFile &a, const goProFile &b) {
            const auto &tsA = a.gpsInfo.GetTs().isNull() ? a.fileInfo.birthTime() : a.gpsInfo.GetTs();
            const auto &tsB = b.gpsInfo.GetTs().isNull() ? b.fileInfo.birthTime() : b.gpsInfo.GetTs();
            return tsA < tsB;
        });

        endResetModel();
    }

    const std::array<QString, 4> tableHeader = {"File", "Time", "Waves", "Duration"};
    float minSpeed;
    size_t minDuration;
    QFileSystemModel fsModel;
    QFileIconProvider fsIconProvider;
    std::vector<goProFile> files;
};


#endif //GOPROSURFSPLITTER_QGOPROFILES_H
