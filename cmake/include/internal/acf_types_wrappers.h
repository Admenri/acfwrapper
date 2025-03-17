// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_INTERNAL_ACF_TYPES_WRAPPERS_H_
#define ACF_INCLUDE_INTERNAL_ACF_TYPES_WRAPPERS_H_

#include "include/internal/acf_types.h"

///
/// Template class that provides common functionality for ACF structure
/// wrapping. Use only with non-POD types that benefit from referencing unowned
/// members.
///
template <class traits>
class AcfStructBase : public traits::struct_type {
 public:
  using struct_type = typename traits::struct_type;

  AcfStructBase() : attached_to_(NULL) { Init(); }
  virtual ~AcfStructBase() {
    // Only clear this object's data if it isn't currently attached to a
    // structure.
    if (!attached_to_) {
      Clear(this);
    }
  }

  AcfStructBase(const AcfStructBase& r) {
    Init();
    *this = r;
  }
  AcfStructBase(const struct_type& r) {
    Init();
    *this = r;
  }

  ///
  /// Attach to the source structure's existing values. DetachTo() can be called
  /// to insert the values back into the existing structure.
  ///
  void AttachTo(struct_type& source) {
    // Only clear this object's data if it isn't currently attached to a
    // structure.
    if (!attached_to_) {
      Clear(this);
    }

    // This object is now attached to the new structure.
    attached_to_ = &source;

    // Transfer ownership of the values from the source structure.
    memcpy(static_cast<struct_type*>(this), &source, sizeof(struct_type));
  }

  ///
  /// Relinquish ownership of values to the target structure.
  ///
  void DetachTo(struct_type& target) {
    if (attached_to_ != &target) {
      // Clear the target structure's values only if we are not currently
      // attached to that structure.
      Clear(&target);
    }

    // Transfer ownership of the values to the target structure.
    memcpy(&target, static_cast<struct_type*>(this), sizeof(struct_type));

    // Remove the references from this object.
    Init();
  }

  ///
  /// Set this object's values. If |copy| is true the source structure's values
  /// will be copied instead of referenced.
  ///
  void Set(const struct_type& source, bool copy) {
    traits::set(&source, this, copy);
  }

  AcfStructBase& operator=(const AcfStructBase& s) {
    return operator=(static_cast<const struct_type&>(s));
  }

  AcfStructBase& operator=(const struct_type& s) {
    Set(s, true);
    return *this;
  }

 protected:
  void Init() {
    memset(static_cast<struct_type*>(this), 0, sizeof(struct_type));
    attached_to_ = NULL;
    traits::init(this);
  }

  static void Clear(struct_type* s) { traits::clear(s); }

  struct_type* attached_to_;
};

///
/// Class representing a range.
///

class AcfRange : public acf_range_t {
 public:
  AcfRange() : acf_range_t{} {}
  AcfRange(const acf_range_t& r) : acf_range_t(r) {}
  AcfRange(uint32_t from, uint32_t to) : acf_range_t{from, to} {}

  static AcfRange InvalidRange() {
    return AcfRange((std::numeric_limits<uint32_t>::max)(),
                    (std::numeric_limits<uint32_t>::max)());
  }

  void Set(int from_val, int to_val) { from = from_val, to = to_val; }
};

inline bool operator==(const AcfRange& a, const AcfRange& b) {
  return a.from == b.from && a.to == b.to;
}

inline bool operator!=(const AcfRange& a, const AcfRange& b) {
  return !(a == b);
}

///
// Class representing a point.
///

struct AcfPointTraits {
  typedef acf_point_t struct_type;

  static inline void init(struct_type* s) {}
  static inline void clear(struct_type* s) {}

  static inline void set(const struct_type* src,
                         struct_type* target,
                         bool copy) {
    *target = *src;
  }
};

class AcfPoint : public AcfStructBase<AcfPointTraits> {
 public:
  typedef AcfStructBase<AcfPointTraits> parent;

  AcfPoint() {}
  AcfPoint(const acf_point_t& r) : parent(r) {}
  AcfPoint(int x, int y) { Set(x, y); }

  bool IsEmpty() const { return x <= 0 && y <= 0; }
  void Set(int x_val, int y_val) { x = x_val, y = y_val; }
};

inline bool operator==(const AcfPoint& a, const AcfPoint& b) {
  return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const AcfPoint& a, const AcfPoint& b) {
  return !(a == b);
}

///
// Class representing a rectangle.
///

struct AcfRectTraits {
  typedef acf_rect_t struct_type;

  static inline void init(struct_type* s) {}
  static inline void clear(struct_type* s) {}

  static inline void set(const struct_type* src,
                         struct_type* target,
                         bool copy) {
    *target = *src;
  }
};

class AcfRect : public AcfStructBase<AcfRectTraits> {
 public:
  typedef AcfStructBase<AcfRectTraits> parent;

  AcfRect() {}
  AcfRect(const acf_rect_t& r) : parent(r) {}
  AcfRect(int x, int y, int width, int height) { Set(x, y, width, height); }

  bool IsEmpty() const { return width <= 0 || height <= 0; }
  void Set(int x_val, int y_val, int width_val, int height_val) {
    x = x_val, y = y_val, width = width_val, height = height_val;
  }

  // Returns true if the point identified by point_x and point_y falls inside
  // this rectangle.  The point (x, y) is inside the rectangle, but the
  // point (x + width, y + height) is not.
  bool Contains(int point_x, int point_y) const {
    return (point_x >= x) && (point_x < x + width) && (point_y >= y) &&
           (point_y < y + height);
  }
  bool Contains(const AcfPoint& point) const {
    return Contains(point.x, point.y);
  }
};

inline bool operator==(const AcfRect& a, const AcfRect& b) {
  return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}

inline bool operator!=(const AcfRect& a, const AcfRect& b) {
  return !(a == b);
}

///
// Class representing a size.
///

struct AcfSizeTraits {
  typedef acf_size_t struct_type;

  static inline void init(struct_type* s) {}
  static inline void clear(struct_type* s) {}

  static inline void set(const struct_type* src,
                         struct_type* target,
                         bool copy) {
    *target = *src;
  }
};

class AcfSize : public AcfStructBase<AcfSizeTraits> {
 public:
  typedef AcfStructBase<AcfSizeTraits> parent;

  AcfSize() {}
  AcfSize(const acf_size_t& r) : parent(r) {}
  AcfSize(int width, int height) { Set(width, height); }

  bool IsEmpty() const { return width <= 0 || height <= 0; }
  void Set(int width_val, int height_val) {
    width = width_val, height = height_val;
  }
};

inline bool operator==(const AcfSize& a, const AcfSize& b) {
  return a.width == b.width && a.height == b.height;
}

inline bool operator!=(const AcfSize& a, const AcfSize& b) {
  return !(a == b);
}

///
/// Class defined browser settings wrapper
///
class AcfBrowserSettings : public acf_browser_settings_t {
 public:
  AcfBrowserSettings() : acf_browser_settings_t{} {}
  AcfBrowserSettings(const acf_browser_settings_t& r)
      : acf_browser_settings_t(r) {}
};

///
/// Class defined environment create settings wrapper
///
struct AcfEnvironmentSettingsTraits {
  using struct_type = acf_environment_settings_t;

  static inline void init(struct_type* s) { s->size = sizeof(struct_type); }

  static inline void clear(struct_type* s) {
    acf_string_clear(&s->user_data_dir);
  }

  static inline void set(const struct_type* src,
                         struct_type* target,
                         bool copy) {
    acf_string_set(src->user_data_dir.str, src->user_data_dir.length,
                   &target->user_data_dir, copy);
  }
};
using AcfEnvironmentSettings = AcfStructBase<AcfEnvironmentSettingsTraits>;

///
/// Profile structure wrapper
///
struct AcfProfileCreateParamsTraits {
  using struct_type = acf_profile_create_params_t;

  static inline void init(struct_type* s) {
    s->incognito_mode = false;
    s->restore_old_session_cookies = false;
    s->persist_session_cookies = false;
    s->enable_encrypted_cookies = true;
    s->ignore_certificate_errors = false;
  }

  static inline void clear(struct_type* s) {
    acf_string_clear(&s->storage_path);
    acf_string_clear(&s->host_mapping_rules);
    acf_string_clear(&s->host_resolver_mapping_rules);
  }

  static inline void set(const struct_type* src,
                         struct_type* target,
                         bool copy) {
    acf_string_set(src->storage_path.str, src->storage_path.length,
                   &target->storage_path, copy);
    target->incognito_mode = src->incognito_mode;
    target->restore_old_session_cookies = src->restore_old_session_cookies;
    target->persist_session_cookies = src->persist_session_cookies;
    target->enable_encrypted_cookies = src->enable_encrypted_cookies;
    target->ignore_certificate_errors = src->ignore_certificate_errors;
    acf_string_set(src->host_mapping_rules.str, src->host_mapping_rules.length,
                   &target->host_mapping_rules, copy);
    acf_string_set(src->host_resolver_mapping_rules.str,
                   src->host_resolver_mapping_rules.length,
                   &target->host_resolver_mapping_rules, copy);
  }
};
using AcfProfileCreateParams = AcfStructBase<AcfProfileCreateParamsTraits>;

///
/// Class representing popup window features.
///
class AcfPopupFeatures : public acf_popup_features_t {
 public:
  AcfPopupFeatures() : acf_popup_features_t{} {}
  AcfPopupFeatures(const acf_popup_features_t& r) : acf_popup_features_t(r) {}
};

struct AcfBrowserCreateParamsTraits {
  using struct_type = acf_browser_create_params_t;

  static inline void init(struct_type* s) {
    s->size = sizeof(struct_type);
    s->parent = 0;
    s->visible = true;
    s->rect = {0};
  }

  static inline void clear(struct_type* s) {}

  static inline void set(const struct_type* src,
                         struct_type* target,
                         bool copy) {
    memcpy(target, src, sizeof(struct_type));
  }
};
using AcfBrowserCreateParams = AcfStructBase<AcfBrowserCreateParamsTraits>;

///
/// Class representing a a keyboard event.
///
class AcfKeyEvent : public acf_key_event_t {
 public:
  AcfKeyEvent() : acf_key_event_t{} {}
  AcfKeyEvent(const acf_key_event_t& r) : acf_key_event_t(r) {}
};

///
/// Class representing a mouse event.
///
class AcfMouseEvent : public acf_mouse_event_t {
 public:
  AcfMouseEvent() : acf_mouse_event_t{} {}
  AcfMouseEvent(const acf_mouse_event_t& r) : acf_mouse_event_t(r) {}
};

///
/// Class representing IME composition underline.
///
class AcfCompositionUnderline : public acf_composition_underline_t {
 public:
  AcfCompositionUnderline() : acf_composition_underline_t{} {}
  AcfCompositionUnderline(const acf_composition_underline_t& r)
      : acf_composition_underline_t(r) {}
};

#endif  //! ACF_INCLUDE_INTERNAL_ACF_TYPES_WRAPPERS_H_
