#ifndef ACF_MESSAGE_LOOP_MANAGER_H_
#define ACF_MESSAGE_LOOP_MANAGER_H_

#include <windows.h>
#include <queue>

#include "include/internal/acf_scoped_refptr.h"

namespace acfclient {

class Task : public virtual AcfBaseRefCounted {
 public:
  virtual void Execute() {}
};

// Must create on UI thread
class MessageWindow {
 public:
  MessageWindow(HINSTANCE instance);
  ~MessageWindow();

  void PostTask(AcfRefPtr<Task> task);

  UINT GetMsgId();

 private:
  HWND msg_hwnd_;
  UINT task_message_id_;
};

}  // namespace acfclient

#endif
