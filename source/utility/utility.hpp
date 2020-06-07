#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <algorithm>

#include "typedefs.hpp"


namespace robotica {
    inline std::string datetime_string(std::string_view fmt = "%Y%m%d_%H%M%S") {
        auto time = std::time(nullptr);
        auto localtime = std::localtime(&time);

        std::stringstream stream;
        stream << std::put_time(localtime, fmt.data());

        return stream.str();
    }


    inline void image_to_file(std::string_view folder, std::string_view filename_root, cv::Mat image) {
        if (!fs::exists(folder)) fs::create_directories(folder);

        fs::path dest_root = fs::path(folder) / filename_root;
        fs::path dest = dest_root;

        int i = 0;
        while (fs::exists(dest)) dest = dest_root.string() + "_" + std::to_string(i++);

        cv::imwrite(dest.string() + ".png", image);
    }


    inline cv::Mat crop_image(cv::Mat source, cv::Rect area) {
        // TODO: Refactor this into something less wasteful. Default OpenCV crop method doesn't work.
        cv::Mat dest(area.width, area.height, source.type());

        int index_og = 0, index_new = 0;
        for (int x = 0; x < source.cols; ++x) {
            for (int y = 0; y < source.rows; ++y) {
                if (x >= area.x && x < area.x + area.width) {
                    if (y >= area.y && y < area.y + area.height) {
                        for (int i = 0; i < source.channels(); ++i) dest.data[index_new + i] = source.data[index_og + i];
                        ++index_new;
                    }
                }

                ++index_og;
            }
        }

        return dest;
    }


    template <typename Container, typename Pred>
    inline void remove_if(Container& ctr, Pred& pred) {
        ctr.erase(
            std::remove_if(
                ctr.begin(),
                ctr.end(),
                pred
            ),
            ctr.end()
        );
    }


    template <const char* name> inline void show(const cv::Mat& m) {
        static bool created = false;

        if (!created) {
            cv::namedWindow(name);
            created = true;
        }

        cv::imshow(name, m);
    }


    // Finds the size of a blob by using floodfill.
    // cv::contourArea uses the Green formula, which can give incorrect results.
    // TODO: Optimize this, use QuickFill or similar.
    inline int floodfill_area_estimate(const cv::Mat& m, const cv::Point& where) {
        cv::Mat visited_map(m.rows, m.cols, CV_8UC1);
        visited_map = cv::Scalar(0);

        auto visited = [&](const cv::Point& pt) -> decltype(auto) { return visited_map.at<uchar>(pt); };
        auto clr_at  = [&](const cv::Point& pt) { return m.at<uchar>(pt); };

        std::vector<cv::Point> to_visit = { where };
        uchar color = m.at<uchar>(where);

        int count = 0;
        while (to_visit.size() > 0) {
            cv::Point pt = to_visit.back();
            to_visit.pop_back();

            visited(pt) = 1;

            if (clr_at(pt) == color) {
                ++count;

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

        return count;
    }
} // namespace robotica