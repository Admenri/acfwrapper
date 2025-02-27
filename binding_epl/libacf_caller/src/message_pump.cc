
#include "message_pump.h"

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
LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam,
                                LPARAM lParam) {
  acf_wrapper::MessagePump* self =
      GetUserDataPtr<acf_wrapper::MessagePump*>(hWnd);

  if (self && message == self->GetMsgId()) {
    // Execute the task.
    acf_wrapper::UIThreadTask* task =
        reinterpret_cast<acf_wrapper::UIThreadTask*>(wParam);
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

namespace acf_wrapper {

MessagePump::MessagePump(HINSTANCE instance)
    : task_message_id_(RegisterWindowMessage(kTaskMessageName)) {
  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = MessageWndProc;
  wc.hInstance = instance;
  wc.lpszClassName = kWndClass;
  RegisterClassEx(&wc);

  message_window_ =
      CreateWindow(kWndClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, instance, 0);
  SetUserDataPtr(message_window_, this);
}

MessagePump::~MessagePump() { DestroyWindow(message_window_); }

void MessagePump::PostTask(AcfRefPtr<UIThreadTask> task) {
  // Add a reference that will be released in MessageWndProc.
  task->AddRef();

  // Post the task for execution by the message window.
  PostMessage(message_window_, task_message_id_,
              reinterpret_cast<WPARAM>(task.get()),
              0);
}

UINT MessagePump::GetMsgId() { return task_message_id_; }

}  // namespace acf_wrapper
