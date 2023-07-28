#ifndef GOPROSURFSPLITTER_GPSINFO_H
#define GOPROSURFSPLITTER_GPSINFO_H

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include "demo/GPMF_mp4reader.h"
#include "GPMF_common.h"
#include "GPMF_parser.h"


struct gps5 {
    float latitude;
    float longitude;
    float altitude;
    float speed2d;
    float speed3d;
};

class GpsStream {
public:
    GpsStream() = default;

    GpsStream(uint8_t fix, uint16_t precision, std::array<int32_t, 5> &scale,
              std::vector<std::array<int32_t, 5>> &rawFrames) : fix(fix), precision(precision) {
        for (const auto &frame: rawFrames) {
            frames.push_back(gps5{
                    float(frame[0]) / float(scale[0]),
                    float(frame[1]) / float(scale[1]),
                    float(frame[2]) / float(scale[2]),
                    float(frame[3]) / float(scale[3]),
                    float(frame[4]) / float(scale[4]),
            });
        }
    }

    [[nodiscard]] float GetAvgSpeed3d() const {
        if (fix == 0 || precision > 500 || frames.empty())
            return 0;

        float sum = 0.0f;
        for (const auto &f: frames) {
            sum += f.speed3d;
        }

        return sum / float(frames.size());
    }

    float GetAvgAltitudes() {
        if (fix == 0 || precision > 500 || frames.empty())
            return 0;

        float sum = 0.0f;
        for (const auto &f: frames) {
            sum += f.altitude;
        }

        return sum / float(frames.size());
    }

//private:
    uint8_t fix{};
    uint16_t precision{};
    std::vector<gps5> frames;
};

class GpsInfo {
public:
    void ReadFromVideo(std::string &filename) {
        auto mp4handle = OpenMP4Source(filename.data(), MOV_GPMF_TRAK_TYPE, MOV_GPMF_TRAK_SUBTYPE, 0);
        if (mp4handle == 0)
            throw std::invalid_argument(filename + " is an invalid MP4/MOV or it has no GPMF gpsBySeconds");

        duration = GetDuration(mp4handle);
        if (duration == 0)
            throw std::invalid_argument(filename + " has no GPMF gpsBySeconds");

        auto payloads = GetNumberPayloads(mp4handle);
        for (auto i = 0; i < payloads; i++) {
            auto payloadSize = GetPayloadSize(mp4handle, i);
            auto payloadRes = GetPayloadResource(mp4handle, 0, payloadSize);
            auto payload = GetPayload(mp4handle, payloadRes, i);
            if (payload == nullptr)
                continue;

            double in = 0.0, out = 0.0;
            auto ret = GetPayloadTime(mp4handle, i, &in, &out);
            if (ret != GPMF_OK)
                continue;

            processStream(size_t(in), payload, payloadSize);
        }

        CloseSource(mp4handle);
    };

    std::vector<float> GetAvgSpeeds3d() {
        std::vector<float> res;
        res.reserve(gpsBySeconds.size());

        for (auto &stream: gpsBySeconds) {
            res.push_back(stream.GetAvgSpeed3d());
        }

        return res;
    }

    [[nodiscard]] float GetMaxSpeed3d() const {
        float res = 0;
        for (auto &stream: gpsBySeconds)
            res = std::max(res, stream.GetAvgSpeed3d());

        return res;
    }

    std::vector<float> GetAvgAltitudes() {
        std::vector<float> res;
        res.reserve(gpsBySeconds.size());

        for (auto &stream: gpsBySeconds) {
            res.push_back(stream.GetAvgAltitudes());
        }

        return res;
    }

    [[nodiscard]] float GetVideoDuration() const {
        return duration;
    }

    [[nodiscard]] QDateTime GetTs() const {
        return ts;
    }

private:
    void processStream(size_t sec, uint32_t *buffer, size_t size) {
        GPMF_stream metadata_stream = {nullptr}, *ms = &metadata_stream;

        auto ret = GPMF_Init(ms, buffer, size);
        if (ret != GPMF_OK)
            throw std::invalid_argument("cannot read metadata: " + std::to_string(ret));

        GPMF_ERR nextRet;
        std::array<int32_t, 5> scale{};
        uint8_t fix{};
        uint16_t precision{};
        std::vector<std::array<int32_t, 5>> gps5;

        do {
            char *rawdata = (char *) GPMF_RawData(ms);
            uint32_t key = GPMF_Key(ms);
            GPMF_SampleType type = GPMF_Type(ms);
            uint32_t samples = GPMF_Repeat(ms);
            uint32_t elements = GPMF_ElementsInStruct(ms);
            auto level = GPMF_NestLevel(ms);

            if (key == STR2FOURCC("STRM") && level == 0) {
                if (!gps5.empty()) {
                    GpsStream stream(fix, precision, scale, gps5);
                    add(sec, stream);
                }
                gps5.clear();
            }

            if (key == STR2FOURCC("SCAL") && type == GPMF_TYPE_SIGNED_LONG && samples == 5) {
                auto *l = (int32_t *) rawdata;
                scale = {
                        (int32_t) BYTESWAP32(l[0]),
                        (int32_t) BYTESWAP32(l[1]),
                        (int32_t) BYTESWAP32(l[2]),
                        (int32_t) BYTESWAP32(l[3]),
                        (int32_t) BYTESWAP32(l[4]),
                };
            }

            if (key == STR2FOURCC("GPSF")) {
                auto *l = (int32_t *) rawdata;
                fix = (uint8_t) BYTESWAP32(l[0]);
//                printf("GPSF: %d\n", fix);
            }

            if (key == STR2FOURCC("GPSP")) {
                auto *l = (int16_t *) rawdata;
                precision = (uint16_t) BYTESWAP16(l[0]);
            }

            if (key == STR2FOURCC("GPSU")) {
                char t[13];
                t[12] = 0;
                strncpy(t, rawdata, 12);
                auto dt = QDateTime::fromString(QString("20") + t, "yyyyMMddHHmmss");

                if (ts.isNull() || dt < ts)
                    ts = dt;
            }

            if (key == STR2FOURCC("GPS5")) {
                if (elements != 5)
                    throw std::invalid_argument("invalid GPS5 structure");

                auto *l = (int32_t *) rawdata;
                for (int i = 0; i < samples; ++i) {
                    gps5.push_back({
                                           (int32_t) BYTESWAP32(l[0]),
                                           (int32_t) BYTESWAP32(l[1]),
                                           (int32_t) BYTESWAP32(l[2]),
                                           (int32_t) BYTESWAP32(l[3]),
                                           (int32_t) BYTESWAP32(l[4]),
                                   });
                    l += 5;
                }
            }

            nextRet = GPMF_Next(ms, GPMF_LEVELS(GPMF_RECURSE_LEVELS | GPMF_TOLERANT));

            while (nextRet == GPMF_ERROR_UNKNOWN_TYPE)
                nextRet = GPMF_Next(ms, GPMF_RECURSE_LEVELS);

        } while (GPMF_OK == nextRet);
    }

    void add(size_t sec, GpsStream &stream) {
        if (sec >= gpsBySeconds.size()) {
            gpsBySeconds.resize(sec + 1);
        }
        gpsBySeconds[sec] = stream;
    };

    std::vector<GpsStream> gpsBySeconds{};
    float duration{};
    QDateTime ts{};
};


#endif //GOPROSURFSPLITTER_GPSINFO_H
