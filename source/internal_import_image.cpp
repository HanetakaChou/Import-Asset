//
// Copyright (C) YuqiaoZhang(HanetakaChou)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include "internal_import_image.h"
#include "internal_import_image_config.h"
#include "internal_import_png_image.h"
#include "internal_import_jpeg_image.h"
#include "internal_import_webp_image.h"
#include <cassert>
#include <algorithm>

#define CV_IGNORE_DEBUG_BUILD_GUARD 1
#if defined(__GNUC__)
// GCC or CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
#include <opencv2/opencv.hpp>
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#if defined(__clang__)
// CLANG-CL
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Winvalid-noreturn"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include <opencv2/opencv.hpp>
#pragma GCC diagnostic pop
#else
// MSVC
#include <opencv2/opencv.hpp>
#endif
#else
#error Unknown Compiler
#endif

extern bool internal_import_image(void const *data_base, size_t data_size, mcrt_vector<uint32_t> &out_rgba_data, uint32_t *out_width, uint32_t *out_height)
{
    mcrt_vector<uint32_t> origin_rgba_data;
    uint32_t origin_width = 0;
    uint32_t origin_height = 0;
    bool status_internal_import_image;
    if (data_size >= 12U &&
        (82U == reinterpret_cast<uint8_t const *>(data_base)[0]) &&
        (73U == reinterpret_cast<uint8_t const *>(data_base)[1]) &&
        (70U == reinterpret_cast<uint8_t const *>(data_base)[2]) &&
        (70U == reinterpret_cast<uint8_t const *>(data_base)[3]) &&
        (87U == reinterpret_cast<uint8_t const *>(data_base)[8]) &&
        (69U == reinterpret_cast<uint8_t const *>(data_base)[9]) &&
        (66U == reinterpret_cast<uint8_t const *>(data_base)[10]) &&
        (80U == reinterpret_cast<uint8_t const *>(data_base)[11]))
    {
        // https://chromium.googlesource.com/webm/libwebp/+/refs/heads/main/imageio/image_dec.c
        // webp_dec.c: ParseRIFF
        // 82 73 70 70 _ _ _ _ 87 69 66 80
        // R  I  F  F  _ _ _ _ W  E  B  P

        status_internal_import_image = internal_import_webp_image(data_base, data_size, origin_rgba_data, &origin_width, &origin_height);
    }
    else if (data_size >= 8U &&
             (137U == reinterpret_cast<uint8_t const *>(data_base)[0]) &&
             (80U == reinterpret_cast<uint8_t const *>(data_base)[1]) &&
             (78U == reinterpret_cast<uint8_t const *>(data_base)[2]) &&
             (71U == reinterpret_cast<uint8_t const *>(data_base)[3]) &&
             (13U == reinterpret_cast<uint8_t const *>(data_base)[4]) &&
             (10U == reinterpret_cast<uint8_t const *>(data_base)[5]) &&
             (26U == reinterpret_cast<uint8_t const *>(data_base)[6]) &&
             (10U == reinterpret_cast<uint8_t const *>(data_base)[7]))
    {
        // png.c: png_sig_cmp
        // 137 80 78 71 13 10 26  10
        // N/A P  N  G  CR LF EOF LF

        status_internal_import_image = internal_import_png_image(data_base, data_size, origin_rgba_data, &origin_width, &origin_height);
    }
    else if (data_size >= 2U &&
             (0XFFU == reinterpret_cast<uint8_t const *>(data_base)[0]) &&
             (0XD8U == reinterpret_cast<uint8_t const *>(data_base)[1]))
    {
        // jdmarker.c: first_marker
        // 0XFF 0XD8
        // N/A  SOI

        status_internal_import_image = internal_import_jpeg_image(data_base, data_size, origin_rgba_data, &origin_width, &origin_height);
    }
    else
    {
        status_internal_import_image = false;
    }

    if (status_internal_import_image)
    {
        uint32_t target_width;
        uint32_t target_height;
        {
            assert((origin_width > 0U) && (origin_width <= k_max_width_or_height) && (origin_height > 0U) && (origin_height <= k_max_width_or_height));

            if ((0U == (origin_width & (origin_width - 1U))) && (0U == (origin_height & (origin_height - 1U))))
            {
                target_width = origin_width;
                target_height = origin_height;
            }
            else
            {
                // DirectXTex/texconv.cpp: FitPowerOf2

                if (origin_width > origin_height)
                {
                    if (0U == (origin_width & (origin_width - 1U)))
                    {
                        target_width = origin_width;
                    }
                    else
                    {
                        uint32_t w;
                        for (w = k_max_width_or_height; w > 1; w >>= 1)
                        {
                            if (w <= origin_width)
                            {
                                break;
                            }
                        }
                        target_width = w;
                    }

                    {
                        float const origin_aspect_ratio = static_cast<float>(origin_width) / static_cast<float>(origin_height);
                        float best_score = FLT_MAX;
                        for (uint32_t h = k_max_width_or_height; h > 0; h >>= 1)
                        {
                            float const score = std::abs((static_cast<float>(target_width) / static_cast<float>(h)) - origin_aspect_ratio);
                            if (score < best_score)
                            {
                                best_score = score;
                                target_height = h;
                            }
                        }
                    }
                }
                else
                {
                    if (0U == (origin_height & (origin_height - 1U)))
                    {
                        target_height = origin_height;
                    }
                    else
                    {
                        uint32_t h;
                        for (h = k_max_width_or_height; h > 1; h >>= 1)
                        {
                            if (h <= origin_height)
                            {
                                break;
                            }
                        }
                        target_height = h;
                    }

                    {
                        float const rcp_origin_aspect_ratio = static_cast<float>(origin_height) / static_cast<float>(origin_width);
                        float best_score = FLT_MAX;
                        for (uint32_t w = k_max_width_or_height; w > 0; w >>= 1)
                        {
                            float const score = std::abs((static_cast<float>(target_height) / static_cast<float>((w))) - rcp_origin_aspect_ratio);
                            if (score < best_score)
                            {
                                best_score = score;
                                target_width = w;
                            }
                        }
                    }
                }
            }
        }

        if ((target_width == origin_width) && (target_height == origin_height))
        {
            out_rgba_data = std::move(origin_rgba_data);
            (*out_width) = origin_width;
            (*out_height) = origin_height;
        }
        else
        {
            // mediapipe/framework/formats/image_frame_opencv.cc
            int const dims = 2;
            int const origin_sizes[dims] = {static_cast<int>(origin_height), static_cast<int>(origin_width)};
            int const type = CV_MAKETYPE(CV_8U, 4);
            size_t const origin_stride = static_cast<size_t>(k_channel_size) * static_cast<size_t>(k_num_channels) * static_cast<uint64_t>(origin_width);
            size_t const origin_steps[dims] = {origin_stride, 8U};

            cv::Mat origin_image(dims, origin_sizes, type, reinterpret_cast<uint8_t *>(origin_rgba_data.data()), origin_steps);

            cv::Mat target_image;
            int const interpolation = ((target_width <= origin_width) && (target_height <= origin_height)) ? cv::INTER_AREA : cv::INTER_LANCZOS4;
            cv::resize(origin_image, target_image, cv::Size(static_cast<int>(target_width), static_cast<int>(target_height)), 0.0, 0.0, interpolation);
            assert(origin_image.isContinuous());

            size_t const target_stride = static_cast<size_t>(k_channel_size) * static_cast<size_t>(k_num_channels) * static_cast<uint64_t>(target_width);
            assert(target_image.type() == type);
            assert(target_image.step[0] == target_stride);
            assert(target_width == target_image.cols);
            assert(target_height == target_image.rows);
            assert(target_image.isContinuous());

            out_rgba_data.resize(static_cast<size_t>(target_width) * static_cast<size_t>(target_height));
            std::memcpy(out_rgba_data.data(), target_image.data, target_image.step[0] * target_image.rows);

            (*out_width) = target_width;
            (*out_height) = target_height;
        }
    }

    return status_internal_import_image;
}
