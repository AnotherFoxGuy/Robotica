#pragma once

#include <opencv2/opencv.hpp>


namespace robotica {
    inline int difference(const cv::Vec3b& a, const cv::Vec3b& b) {
        return std::abs(((int)b[0]) - ((int)a[0])) + std::abs(((int)b[1]) - ((int)a[1])) + std::abs(((int)b[2]) - ((int)a[2]));
    }


    // Apply the predicate to each pixel found by floodfilling from the given point.
    // TODO: Optimize this, use QuickFill or similar.
    template <typename Pred> inline void floodfill_foreach(const cv::Mat& m, const cv::Point& where, const Pred& pred) {
        cv::Mat visited_map(m.rows, m.cols, CV_8UC1);
        visited_map = cv::Scalar(0);

        auto visited = [&](const cv::Point& pt) -> decltype(auto) { return visited_map.at<uchar>(pt); };
        auto clr_at  = [&](const cv::Point& pt) { return m.at<uchar>(pt); };

        std::vector<cv::Point> to_visit = { where };
        uchar color = m.at<uchar>(where);

        while (to_visit.size() > 0) {
            cv::Point pt = to_visit.back();
            to_visit.pop_back();

            visited(pt) = 1;

            if (clr_at(pt) == color) {
                pred(pt);

                const std::array deltas = {
                    cv::Point{ -1,  0 },
                    cv::Point{  1,  0 },
                    cv::Point{  0, -1 },
                    cv::Point{  0,  1 }
                };

                for (const auto& d : deltas) {
                    auto npt = pt + d;

                    if (npt.x < 0 || npt.x >= m.rows) continue;
                    if (npt.y < 0 || npt.y >= m.cols) continue;

                    if (visited(npt) == 0) to_visit.push_back(npt);
                }
            }
        }
    }


    // Finds the area of a blob through floodfill.
    inline int floodfill_area(const cv::Mat& m, const cv::Point& where) {
        int count = 0;
        floodfill_foreach(m, where, [&](const auto& pt) { ++count; });
        return count;
    }


    inline cv::Vec3b floodfill_avgcolor(const cv::Mat& floodmap, const cv::Mat& colormap, const cv::Point& where) {
        int count = 0;
        unsigned long long accum[3] = { 0, 0, 0 };

        floodfill_foreach(floodmap, where, [&](const auto& pt) {
            ++count;

            const auto& clr = colormap.at<cv::Vec3b>(pt);
            for (int i = 0; i < 3; ++i) accum[i] += clr[i];
        });

        return cv::Vec3b {
            (uchar) (accum[0] / count),
            (uchar) (accum[1] / count),
            (uchar) (accum[2] / count)
        };

        return { 0, 0, 0 };
    }
}