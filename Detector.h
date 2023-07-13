#ifndef GOPROSURFSPLITTER_DETECTOR_H
#define GOPROSURFSPLITTER_DETECTOR_H

#include <vector>
#include <cstdlib>
#include "GpsInfo.h"

struct timeFrame {
    size_t start, finish;
};

class Detector {
public:
    static std::vector<timeFrame> GetWaves(GpsInfo &gps, size_t margin, size_t minLength, float minSpeed) {
        std::vector<timeFrame> res;

        auto speeds = gps.GetAvgSpeeds3d();

        size_t l = 0;
        size_t r = 0;

        while (l < speeds.size()) {
            if (speeds[l] < minSpeed) {
                ++l;
                continue;
            }

            for (r = l + 1; r < speeds.size(); ++r) {
                if (speeds[r] < minSpeed) {
                    if (r - l > minLength)
                        res.push_back({l, r - 1});
                    l = r;
                    break;
                }
            }

            if (r == speeds.size() && speeds[r - 1] >= minSpeed && r - l > minLength)
                res.push_back({l, r - 1});

            l = r;
        }


        // Add margins
        for (auto &frame: res) {
            frame.start = frame.start > margin ? frame.start - margin : 0;
            frame.finish = frame.finish + margin < speeds.size() ? frame.finish + margin : speeds.size() - 1;
        }

        return res;
    };
};


#endif //GOPROSURFSPLITTER_DETECTOR_H
