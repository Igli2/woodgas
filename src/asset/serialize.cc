#include "asset.h"

#include <cstring>

namespace serialize {
    template <typename T>
    void write_value(void** data, T value) {
        **((T**)data) = value;
        *((T**)data) += 1;
    }
    template <typename T>
    T read_value(void** data) {
        T value = **((T**)data);
        *((T**)data) += 1;
        return value;
    }

    size_t size_string(const std::string& value) {
        return sizeof(value.size()) + value.size();
    }

    void write_string(void** data, const std::string& value) {
        write_value(data, value.size());
        std::memcpy((char*)*data, &(*value.begin()), value.size());
        *((char**)data) += value.size();
    }

    std::string read_string(void** data) {
        size_t len = read_value<size_t>(data);
        std::string dest;
        dest.resize(len);
        std::memcpy(&(*dest.begin()), (char*)*data, len);
        *((char**)data) += len;
        return dest;
    }

    void write_image(void** data, asset::Image& img) {
        size_t size = img.get_width() * img.get_height() * img.get_components();
        write_value(data, img.get_width());
        write_value(data, img.get_height());
        write_value(data, img.get_components());
        std::memcpy((char*)*data, img.get_data(), size);
        *((char**)data) += size;
    }

    size_t size_image(asset::Image& img) {
        return sizeof(img.get_width()) + sizeof(img.get_height()) +
               sizeof(img.get_components()) +
               img.get_width() * img.get_height() * img.get_components();
    }

    asset::Image read_image(void** data) {
        size_t width = read_value<size_t>(data);
        size_t height = read_value<size_t>(data);
        int components = read_value<int>(data);
        std::vector<unsigned char> img_data;
        img_data.resize(width * height * components);
        std::memcpy(&(*img_data.begin()), (char*)*data, img_data.size());
        *((char**)data) += img_data.size();
        return asset::Image(width, height, components, img_data);
    }
}
