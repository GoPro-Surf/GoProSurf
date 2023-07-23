//
// Created by svistunov on 18.07.23.
//

#ifndef GOPROSURFSPLITTER_QWAVESMODEL_H
#define GOPROSURFSPLITTER_QWAVESMODEL_H


#include <QAbstractItemModel>
#include <QFileInfo>
#include "QGoProFiles.h"

class QWavesModel : public QAbstractItemModel {
public:
    void setData(goProFile &data) {
        beginResetModel();
        proFile = data;
        endResetModel();
    }

    timeFrame getWave(QModelIndex index) {
        return proFile.waves[index.row()];
    }

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override {
        return createIndex(row, column);
    }

    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override {
        return {};
    }

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override {
        return (int) proFile.waves.size();
    }

    [[nodiscard]] int columnCount(const QModelIndex &parent) const override {
        return (int) tableHeader.size();
    }

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override {
        if (role == Qt::DisplayRole) {
            QTime t(0, 0, 0);
            switch (index.column()) {
                case 0:
                    return t.addSecs(int(proFile.waves[index.row()].finish - proFile.waves[index.row()].start))
                            .toString("mm:ss");
                case 1:
                    return t.addSecs((int) proFile.waves[index.row()].start).toString("mm:ss");
                case 2:
                    return t.addSecs((int) proFile.waves[index.row()].finish).toString("mm:ss");
            }
        }

        return {};
    }

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole) {
            if (orientation == Qt::Horizontal) {
                return tableHeader[section];
            } else if (orientation == Qt::Vertical)
                return section + 1;
        }
        return {};
    }

private:
    const std::array<QString, 3> tableHeader = {"Duration", "From", "To"};
    goProFile proFile;
};


#endif //GOPROSURFSPLITTER_QWAVESMODEL_H
