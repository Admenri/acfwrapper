#include "message_loop_manager.h"

namespace acfclient {

namespace {

const wchar_t kWndClass[] = L"acfClient_MessageWindow";
const wchar_t kTaskMessageName[] = L"acfClient_CustomTask";

void SetUserDataPtr(HWND hWnd, void* ptr) {
  SetLastError(ERROR_SUCCESS);
  ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr));
}

// Return the window's user data pointer.
template <typename T>
T GetUserDataPtr(HWND hWnd) {
  return reinterpret_cast<T>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

// static
LRESULT CALLBACK MessageWndProc(HWND hWnd,
                                UINT message,
                                WPARAM wParam,
                                LPARAM lParam) {
  MessageWindow* self = GetUserDataPtr<MessageWindow*>(hWnd);

  if (self && message == self->GetMsgId()) {
    // Execute the task.
    Task* task = reinterpret_cast<Task*>(wParam);
    task->Execute();

    // Release the reference added in PostTaskInternal. This will likely result
    // in |task| being deleted.
    task->Release();
  } else {
    switch (message) {
      case WM_NCDESTROY:
        // Clear the reference to |self|.
        SetUserDataPtr(hWnd, nullptr);
        break;
    }
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}

}  // namespace

MessageWindow::MessageWindow(HINSTANCE instance)
    : task_message_id_(RegisterWindowMessage(kTaskMessageName)) {
  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = MessageWndProc;
  wc.hInstance = instance;
  wc.lpszClassName = kWndClass;
  RegisterClassEx(&wc);

  msg_hwnd_ =
      CreateWindow(kWndClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, instance, 0);
  SetUserDataPtr(msg_hwnd_, this);
}

MessageWindow::~MessageWindow() {
  DestroyWindow(msg_hwnd_);
}

void MessageWindow::PostTask(AcfRefPtr<Task> task) {
  // Add a reference that will be released in MessageWndProc.
  task->AddRef();

  // Post the task for execution by the message window.
  PostMessage(msg_hwnd_, task_message_id_, reinterpret_cast<WPARAM>(task.get()),
              0);
}

UINT MessageWindow::GetMsgId() {
  return task_message_id_;
}

}  // namespace acfclient
