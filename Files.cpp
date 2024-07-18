#include "Files.h"

#include "fmt/ostream.h"

namespace f {
        std::vector<File> Files::getSkins() {
            auto skins = std::vector<File>();

            for(const auto& entry : std::filesystem::directory_iterator(Files::osuPath)) {
                skins.push_back(f::File(entry.path().generic_string(),entry.path().filename().generic_string(),true));
            }

            return skins;
        }
}