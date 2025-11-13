// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570
#define HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570


#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include <atomic>


namespace iridium::threading {


class IRunnable: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IRunnable)
    virtual void run(std::atomic<bool> &is_running) = 0;
};


} // namespace iridium::threading


#endif // HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570
