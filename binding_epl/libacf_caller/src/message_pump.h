#ifndef ACF_EPL_WRAPPER_MESSAGE_PUMP_H_
#define ACF_EPL_WRAPPER_MESSAGE_PUMP_H_
#pragma once

#include <windows.h>
#include "include/internal/acf_scoped_refptr.h"

namespace acf_wrapper {

class UIThreadTask : public virtual AcfBaseRefCounted {
 public:
  virtual void Execute() {}
};

class MessagePump {
 public:
  MessagePump(HINSTANCE instance);
  ~MessagePump();

  void PostTask(AcfRefPtr<UIThreadTask> task);

  UINT GetMsgId();

 private:
  HWND message_window_;
  UINT task_message_id_;

};

} // namespace acf_wrapper

#endif // ACF_EPL_WRAPPER_MESSAGE_PUMP_H_
