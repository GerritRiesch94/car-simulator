/** 
 * @file session_controller.h
 *
 */

#ifndef SESSION_CONTROLLER_H
#define SESSION_CONTROLLER_H

#include "ecu_timer.h"
#include <atomic>

enum class UdsSession
{
    DEFAULT = 0x01, ///< the default UDS session
    PROGRAMMING = 0x02,
    EXTENDED = 0x03
};

class SessionController : public EcuTimer
{
public:
    SessionController() = default;
    SessionController(const SessionController& orig) = default;
    SessionController& operator =(const SessionController& orig) = default;
    SessionController(SessionController&& orig) = default;
    SessionController& operator =(SessionController&& orig) = default;
    virtual ~SessionController() = default;

    void startSession();
    UdsSession getCurretnUdsSession() const noexcept;
    void setCurrentUdsSession(const UdsSession ses) noexcept;

private:
    std::atomic<UdsSession> session_{UdsSession::DEFAULT};
    virtual void wakeup() override;
};

#endif /* SESSION_CONTROLLER_H */
