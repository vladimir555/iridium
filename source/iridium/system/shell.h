// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
#define HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2


#include <string>

#include "iridium/io/multiplexer.h"


namespace iridium::system {


class Shell: public pattern::IInitializable {
public:
    DEFINE_IMPLEMENTATION(Shell)
    Shell();

    typedef std::chrono::seconds TTimeDuration;

    static TTimeDuration const DEFAULT_TIMEOUT;

    struct TResult {
        std::string output;
        int         code;
    };

    void initialize() override;
    void finalize()   override;

    TResult run(std::string const &command_line, TTimeDuration const &timeout = DEFAULT_TIMEOUT);

private:
    io::IMultiplexer::TSharedPtr    m_multiplexer;
};


} // namespace iridium::system


#endif // HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
