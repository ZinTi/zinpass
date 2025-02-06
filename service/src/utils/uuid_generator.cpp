#include "utils/uuid_generator.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace zinpass::utils {
    std::string UUIDGenerator::generate() {
        static boost::uuids::random_generator gen;
        return boost::uuids::to_string(gen());
    }
}