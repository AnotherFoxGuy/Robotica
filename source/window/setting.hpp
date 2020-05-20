#pragma once

#include <utility/traits.hpp>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace robotica
{
using setting_types = pack<u8, i8, u16, i16, u32, i32, u64, i64, float, double, long double, bool, std::string>;

template <typename T, typename C>
struct setting
{
    using type = T;

    // We need pointers to either int or float for the actual value so we have to use those types for the actual storage.
    using stored_type = std::conditional_t<
        std::is_integral_v<T>,
        int,
        std::conditional_t<std::is_floating_point_v<T>, float, T>>;

    std::string name;
    T default, min, max;
    stored_type value;
    int setting_group;

    setting(C& container, std::string&& name, T&& default, T&& min, T&& max, int setting_group)
        : name(std::move(name)),
          default(std::forward<T>(default)),
          min(std::forward<T>(min)),
          max(std::forward<T>(max)),
          value((stored_type) default),
          setting_group(setting_group)
    {
        container.register_setting(*this, setting_group);
    }

    operator stored_type&(void) { return value; }
};

template <typename Derived>
class setting_storage
{
  public:
    using container_t = Derived;

    template <typename T>
    void register_setting(setting<T, Derived>& s, int setting_group)
    {
        if (settings.size() < setting_group + 1) settings.resize(setting_group + 1);

        using type = std::vector<std::reference_wrapper<setting<T, Derived>>>;
        std::get<type>(settings[setting_group]).push_back(s);
    }

  protected:
    using contained_t = std::vector<typename setting_types ::template apply_compress<typename bind_template_back<setting, Derived>::type>::template apply_compress<std::reference_wrapper>::template apply_compress<std::vector>::template apply_expand<std::tuple>>;

    contained_t settings;
};
} // namespace robotica