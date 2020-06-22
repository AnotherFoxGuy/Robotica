#pragma once

#include <utility/typedefs.hpp>
#include <utility/utility.hpp>
#include <utility/math.hpp>

#include <glm/glm.hpp>

#include <array>
#include <vector>
#include <cstddef>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <execution>


namespace robotica {
    namespace detail {
        template <typename T> constexpr inline glm::vec3 default_getter(const T& v) {
            return v.position;
        }
    }


    template <
        typename T,
        auto Getter = &detail::default_getter<T>
    > class partition_space {
    public:
        constexpr static std::size_t SCALE_FACTOR       = 128;
        constexpr static std::size_t WORLD_SIZE         = SCALE_FACTOR * 128;
        constexpr static std::size_t CHUNK_SIZE         = SCALE_FACTOR * 8;
        constexpr static std::size_t CBRT_NUM_CHUNKS    = WORLD_SIZE / CHUNK_SIZE;
        constexpr static std::size_t NUM_CHUNKS         = CBRT_NUM_CHUNKS * CBRT_NUM_CHUNKS * CBRT_NUM_CHUNKS;

        using storage_t = std::array<std::vector<T*>, NUM_CHUNKS>;


        partition_space(void) : data(std::make_unique<storage_t>()) {
            std::generate(data->begin(), data->end(), []() { return std::vector<T*>{}; });
        }


        /// <summary>
        /// Fill the partition space with data.
        /// All data currently in the partition space is removed. All references to this data become invalid.
        /// </summary>
        /// <param name="elems">The new data to fill the partition space with.</param>
        void set_data(std::vector<T>& elems) {
            std::for_each(
                std::execution::par_unseq,
                data->begin(),
                data->end(),
                [](auto& v) { v.clear(); }
            );

            for (T& elem : elems) {
                std::size_t index = flatten(Getter(elem));
                assert(index < data->size());

                (*data)[index].push_back(&elem);
            }
        }


        /// <summary>
        /// Finds objects near the given point.
        /// </summary>
        /// <param name="where">The point to find objects around.</param>
        /// <param name="radius">The maximum search radius.</param>
        /// <param name="directions">If an axis is set to true, only results with a greater coordinate along that axis are returned.</param>
        /// <returns>References to nearby objects. Calling set_data or deleting the partition_space invalidates the references.</returns>
        std::vector<std::reference_wrapper<const T>> nearby(const glm::vec3& where, float radius, glm::bvec3 directions = { false, false, false }) const {
            glm::vec3 min, max;
            for (int i = 0; i < 3; ++i) {
                min[i] = directions[i] ? where[i] : where[i] - radius;
                max[i] = where[i] + radius;
            }

            glm::uvec3 minchunk = to_chunk_pos(min), maxchunk = to_chunk_pos(max);


            std::vector<std::reference_wrapper<const T>> result;
            for (std::size_t x = minchunk.x; x <= maxchunk.x; ++x) {
                for (std::size_t y = minchunk.y; y <= maxchunk.y; ++y) {
                    for (std::size_t z = minchunk.z; z <= maxchunk.z; ++z) {
                        const auto& v = (*data)[flatten_chunk({ x, y, z })];
                        for (const auto* e : v) if (distance_squared(where, Getter(*e)) < (radius * radius)) result.push_back(std::cref(*e));
                    }
                }
            }

            return result;
        }


        template <typename Pred> void for_each(Pred pred) {
            for (auto& vec : data) {
                for (auto* elem : vec) pred(*elem);
            }
        }


        template <typename Pred> void for_each(Pred pred) const {
            for (const auto& vec : data) {
                for (const auto* elem : vec) pred(*elem);
            }
        }
    private:
        // ~33MB at default settings, so too big for the stack.
        unique<storage_t> data;


        // Scales and moves the position to the corresponding position in the grid.
        constexpr static glm::vec3 transform_position(const glm::vec3& pos) {
            return (pos * float(SCALE_FACTOR)) + float(WORLD_SIZE / 2);
        }


        constexpr static glm::uvec3 to_chunk_pos(const glm::vec3& pos) {
            return glm::uvec3(glm::floor(transform_position(pos))) / CHUNK_SIZE;
        }


        // Converts a chunk position to the index of that chunk.
        constexpr static std::size_t flatten_chunk(const glm::uvec3& pos) {
            return pos.x + CBRT_NUM_CHUNKS * (pos.y + CBRT_NUM_CHUNKS * pos.z);
        }


        // Converts a world position to the index of the chunk containing it.
        constexpr static std::size_t flatten(const glm::vec3& pos) {
            return flatten_chunk(to_chunk_pos(pos));
        }


        // Converts a chunk index to the corresponding chunk position.
        constexpr static glm::uvec3 unflatten(std::size_t pos) {
            const std::size_t z = pos / (2 * CBRT_NUM_CHUNKS);
            pos -= z * (2 * CBRT_NUM_CHUNKS);

            const std::size_t y = pos / CBRT_NUM_CHUNKS;
            pos -= y * CBRT_NUM_CHUNKS;

            return (glm::uvec3{ i, y, z } - (WORLD_SIZE / 2)) / SCALE_FACTOR;
        }
    };
}