// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_VALUES_H_
#define ACF_INCLUDE_ACF_VALUES_H_

#include <vector>

#include "include/internal/acf_scoped_refptr.h"
#include "include/internal/acf_string.h"
#include "include/internal/acf_string_list.h"
#include "include/internal/acf_types.h"

class AcfBinaryValue;
class AcfDictionaryValue;
class AcfListValue;

typedef acf_value_type_t AcfValueType;

///
/// Class that wraps other data value types. Complex types (binary, dictionary
/// and list) will be referenced but not owned by this object. Can be used on
/// any process and thread.
///
/*--acf(source=library)--*/
class AcfValue : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Creates a new object.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfValue> Create();

  ///
  /// Returns true if the underlying data is valid. This will always be true for
  /// simple types. For complex types (binary, dictionary and list) the
  /// underlying data may become invalid if owned by another object (e.g. list
  /// or dictionary) and that other object is then modified or destroyed. This
  /// value object can be re-used by calling Set*() even if the underlying data
  /// is invalid.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Returns true if the underlying data is owned by another object.
  ///
  /*--acf()--*/
  virtual bool IsOwned() = 0;

  ///
  /// Returns true if the underlying data is read-only. Some APIs may expose
  /// read-only objects.
  ///
  /*--acf()--*/
  virtual bool IsReadOnly() = 0;

  ///
  /// Returns true if this object and |that| object have the same underlying
  /// data. If true modifications to this object will also affect |that| object
  /// and vice-versa.
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfValue> that) = 0;

  ///
  /// Returns true if this object and |that| object have an equivalent
  /// underlying value but are not necessarily the same object.
  ///
  /*--acf()--*/
  virtual bool IsEqual(AcfRefPtr<AcfValue> that) = 0;

  ///
  /// Returns a copy of this object. The underlying data will also be copied.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfValue> Copy() = 0;

  ///
  /// Returns the underlying value type.
  ///
  /*--acf(default_retval=VTYPE_INVALID)--*/
  virtual AcfValueType GetType() = 0;

  ///
  /// Returns the underlying value as type bool.
  ///
  /*--acf()--*/
  virtual bool GetBool() = 0;

  ///
  /// Returns the underlying value as type int.
  ///
  /*--acf()--*/
  virtual int GetInt() = 0;

  ///
  /// Returns the underlying value as type double.
  ///
  /*--acf()--*/
  virtual double GetDouble() = 0;

  ///
  /// Returns the underlying value as type string.
  ///
  /*--acf()--*/
  virtual AcfString GetString() = 0;

  ///
  /// Returns the underlying value as type binary. The returned reference may
  /// become invalid if the value is owned by another object or if ownership is
  /// transferred to another object in the future. To maintain a reference to
  /// the value after assigning ownership to a dictionary or list pass this
  /// object to the SetValue() method instead of passing the returned reference
  /// to SetBinary().
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfBinaryValue> GetBinary() = 0;

  ///
  /// Returns the underlying value as type dictionary. The returned reference
  /// may become invalid if the value is owned by another object or if ownership
  /// is transferred to another object in the future. To maintain a reference to
  /// the value after assigning ownership to a dictionary or list pass this
  /// object to the SetValue() method instead of passing the returned reference
  /// to SetDictionary().
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfDictionaryValue> GetDictionary() = 0;

  ///
  /// Returns the underlying value as type list. The returned reference may
  /// become invalid if the value is owned by another object or if ownership is
  /// transferred to another object in the future. To maintain a reference to
  /// the value after assigning ownership to a dictionary or list pass this
  /// object to the SetValue() method instead of passing the returned reference
  /// to SetList().
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfListValue> GetList() = 0;

  ///
  /// Sets the underlying value as type null. Returns true if the value was set
  /// successfully.
  ///
  /*--acf()--*/
  virtual bool SetNull() = 0;

  ///
  /// Sets the underlying value as type bool. Returns true if the value was set
  /// successfully.
  ///
  /*--acf()--*/
  virtual bool SetBool(bool value) = 0;

  ///
  /// Sets the underlying value as type int. Returns true if the value was set
  /// successfully.
  ///
  /*--acf()--*/
  virtual bool SetInt(int value) = 0;

  ///
  /// Sets the underlying value as type double. Returns true if the value was
  /// set successfully.
  ///
  /*--acf()--*/
  virtual bool SetDouble(double value) = 0;

  ///
  /// Sets the underlying value as type string. Returns true if the value was
  /// set successfully.
  ///
  /*--acf(optional_param=value)--*/
  virtual bool SetString(const AcfString& value) = 0;

  ///
  /// Sets the underlying value as type binary. Returns true if the value was
  /// set successfully. This object keeps a reference to |value| and ownership
  /// of the underlying data remains unchanged.
  ///
  /*--acf()--*/
  virtual bool SetBinary(AcfRefPtr<AcfBinaryValue> value) = 0;

  ///
  /// Sets the underlying value as type dict. Returns true if the value was set
  /// successfully. This object keeps a reference to |value| and ownership of
  /// the underlying data remains unchanged.
  ///
  /*--acf()--*/
  virtual bool SetDictionary(AcfRefPtr<AcfDictionaryValue> value) = 0;

  ///
  /// Sets the underlying value as type list. Returns true if the value was set
  /// successfully. This object keeps a reference to |value| and ownership of
  /// the underlying data remains unchanged.
  ///
  /*--acf()--*/
  virtual bool SetList(AcfRefPtr<AcfListValue> value) = 0;
};

///
/// Class representing a binary value. Can be used on any process and thread.
///
/*--acf(source=library)--*/
class AcfBinaryValue : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Creates a new object that is not owned by any other object. The specified
  /// |data| will be copied.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfBinaryValue> Create(const void* data, size_t data_size);

  ///
  /// Returns true if this object is valid. This object may become invalid if
  /// the underlying data is owned by another object (e.g. list or dictionary)
  /// and that other object is then modified or destroyed. Do not call any other
  /// methods if this method returns false.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Returns true if this object is currently owned by another object.
  ///
  /*--acf()--*/
  virtual bool IsOwned() = 0;

  ///
  /// Returns true if this object and |that| object have the same underlying
  /// data.
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfBinaryValue> that) = 0;

  ///
  /// Returns true if this object and |that| object have an equivalent
  /// underlying value but are not necessarily the same object.
  ///
  /*--acf()--*/
  virtual bool IsEqual(AcfRefPtr<AcfBinaryValue> that) = 0;

  ///
  /// Returns a copy of this object. The data in this object will also be
  /// copied.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfBinaryValue> Copy() = 0;

  ///
  /// Returns the data size.
  ///
  /*--acf()--*/
  virtual size_t GetSize() = 0;

  ///
  /// Read up to |buffer_size| number of bytes into |buffer|. Reading begins at
  /// the specified byte |data_offset|. Returns the number of bytes read.
  ///
  /*--acf()--*/
  virtual size_t GetData(void* buffer,
                         size_t buffer_size,
                         size_t data_offset) = 0;
};

///
/// Class representing a dictionary value. Can be used on any process and
/// thread.
///
/*--acf(source=library)--*/
class AcfDictionaryValue : public virtual AcfBaseRefCounted {
 public:
  typedef std::vector<AcfString> KeyList;

  ///
  /// Creates a new object that is not owned by any other object.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfDictionaryValue> Create();

  ///
  /// Returns true if this object is valid. This object may become invalid if
  /// the underlying data is owned by another object (e.g. list or dictionary)
  /// and that other object is then modified or destroyed. Do not call any other
  /// methods if this method returns false.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Returns true if this object is currently owned by another object.
  ///
  /*--acf()--*/
  virtual bool IsOwned() = 0;

  ///
  /// Returns true if the values of this object are read-only. Some APIs may
  /// expose read-only objects.
  ///
  /*--acf()--*/
  virtual bool IsReadOnly() = 0;

  ///
  /// Returns true if this object and |that| object have the same underlying
  /// data. If true modifications to this object will also affect |that| object
  /// and vice-versa.
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfDictionaryValue> that) = 0;

  ///
  /// Returns true if this object and |that| object have an equivalent
  /// underlying value but are not necessarily the same object.
  ///
  /*--acf()--*/
  virtual bool IsEqual(AcfRefPtr<AcfDictionaryValue> that) = 0;

  ///
  /// Returns a writable copy of this object. If |exclude_empty_children| is
  /// true any empty dictionaries or lists will be excluded from the copy.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfDictionaryValue> Copy(bool exclude_empty_children) = 0;

  ///
  /// Returns the number of values.
  ///
  /*--acf()--*/
  virtual size_t GetSize() = 0;

  ///
  /// Removes all values. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool Clear() = 0;

  ///
  /// Returns true if the current dictionary has a value for the given key.
  ///
  /*--acf()--*/
  virtual bool HasKey(const AcfString& key) = 0;

  ///
  /// Reads all keys for this dictionary into the specified vector.
  ///
  /*--acf()--*/
  virtual bool GetKeys(KeyList& keys) = 0;

  ///
  /// Removes the value at the specified key. Returns true is the value was
  /// removed successfully.
  ///
  /*--acf()--*/
  virtual bool Remove(const AcfString& key) = 0;

  ///
  /// Returns the value type for the specified key.
  ///
  /*--acf(default_retval=VTYPE_INVALID)--*/
  virtual AcfValueType GetType(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key. For simple types the returned
  /// value will copy existing data and modifications to the value will not
  /// modify this object. For complex types (binary, dictionary and list) the
  /// returned value will reference existing data and modifications to the value
  /// will modify this object.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfValue> GetValue(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key as type bool.
  ///
  /*--acf()--*/
  virtual bool GetBool(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key as type int.
  ///
  /*--acf()--*/
  virtual int GetInt(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key as type double.
  ///
  /*--acf()--*/
  virtual double GetDouble(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key as type string.
  ///
  /*--acf()--*/
  virtual AcfString GetString(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key as type binary. The returned
  /// value will reference existing data.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfBinaryValue> GetBinary(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key as type dictionary. The returned
  /// value will reference existing data and modifications to the value will
  /// modify this object.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfDictionaryValue> GetDictionary(const AcfString& key) = 0;

  ///
  /// Returns the value at the specified key as type list. The returned value
  /// will reference existing data and modifications to the value will modify
  /// this object.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfListValue> GetList(const AcfString& key) = 0;

  ///
  /// Sets the value at the specified key. Returns true if the value was set
  /// successfully. If |value| represents simple data then the underlying data
  /// will be copied and modifications to |value| will not modify this object.
  /// If |value| represents complex data (binary, dictionary or list) then the
  /// underlying data will be referenced and modifications to |value| will
  /// modify this object.
  ///
  /*--acf()--*/
  virtual bool SetValue(const AcfString& key, AcfRefPtr<AcfValue> value) = 0;

  ///
  /// Sets the value at the specified key as type null. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetNull(const AcfString& key) = 0;

  ///
  /// Sets the value at the specified key as type bool. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetBool(const AcfString& key, bool value) = 0;

  ///
  /// Sets the value at the specified key as type int. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetInt(const AcfString& key, int value) = 0;

  ///
  /// Sets the value at the specified key as type double. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetDouble(const AcfString& key, double value) = 0;

  ///
  /// Sets the value at the specified key as type string. Returns true if the
  /// value was set successfully.
  ///
  /*--acf(optional_param=value)--*/
  virtual bool SetString(const AcfString& key, const AcfString& value) = 0;

  ///
  /// Sets the value at the specified key as type binary. Returns true if the
  /// value was set successfully. If |value| is currently owned by another
  /// object then the value will be copied and the |value| reference will not
  /// change. Otherwise, ownership will be transferred to this object and the
  /// |value| reference will be invalidated.
  ///
  /*--acf()--*/
  virtual bool SetBinary(const AcfString& key,
                         AcfRefPtr<AcfBinaryValue> value) = 0;

  ///
  /// Sets the value at the specified key as type dict. Returns true if the
  /// value was set successfully. If |value| is currently owned by another
  /// object then the value will be copied and the |value| reference will not
  /// change. Otherwise, ownership will be transferred to this object and the
  /// |value| reference will be invalidated.
  ///
  /*--acf()--*/
  virtual bool SetDictionary(const AcfString& key,
                             AcfRefPtr<AcfDictionaryValue> value) = 0;

  ///
  /// Sets the value at the specified key as type list. Returns true if the
  /// value was set successfully. If |value| is currently owned by another
  /// object then the value will be copied and the |value| reference will not
  /// change. Otherwise, ownership will be transferred to this object and the
  /// |value| reference will be invalidated.
  ///
  /*--acf()--*/
  virtual bool SetList(const AcfString& key, AcfRefPtr<AcfListValue> value) = 0;
};

///
/// Class representing a list value. Can be used on any process and thread.
///
/*--acf(source=library)--*/
class AcfListValue : public virtual AcfBaseRefCounted {
 public:
  ///
  /// Creates a new object that is not owned by any other object.
  ///
  /*--acf()--*/
  static AcfRefPtr<AcfListValue> Create();

  ///
  /// Returns true if this object is valid. This object may become invalid if
  /// the underlying data is owned by another object (e.g. list or dictionary)
  /// and that other object is then modified or destroyed. Do not call any other
  /// methods if this method returns false.
  ///
  /*--acf()--*/
  virtual bool IsValid() = 0;

  ///
  /// Returns true if this object is currently owned by another object.
  ///
  /*--acf()--*/
  virtual bool IsOwned() = 0;

  ///
  /// Returns true if the values of this object are read-only. Some APIs may
  /// expose read-only objects.
  ///
  /*--acf()--*/
  virtual bool IsReadOnly() = 0;

  ///
  /// Returns true if this object and |that| object have the same underlying
  /// data. If true modifications to this object will also affect |that| object
  /// and vice-versa.
  ///
  /*--acf()--*/
  virtual bool IsSame(AcfRefPtr<AcfListValue> that) = 0;

  ///
  /// Returns true if this object and |that| object have an equivalent
  /// underlying value but are not necessarily the same object.
  ///
  /*--acf()--*/
  virtual bool IsEqual(AcfRefPtr<AcfListValue> that) = 0;

  ///
  /// Returns a writable copy of this object.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfListValue> Copy() = 0;

  ///
  /// Sets the number of values. If the number of values is expanded all
  /// new value slots will default to type null. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool SetSize(size_t size) = 0;

  ///
  /// Returns the number of values.
  ///
  /*--acf()--*/
  virtual size_t GetSize() = 0;

  ///
  /// Removes all values. Returns true on success.
  ///
  /*--acf()--*/
  virtual bool Clear() = 0;

  ///
  /// Removes the value at the specified index.
  ///
  /*--acf()--*/
  virtual bool Remove(size_t index) = 0;

  ///
  /// Returns the value type at the specified index.
  ///
  /*--acf(default_retval=VTYPE_INVALID)--*/
  virtual AcfValueType GetType(size_t index) = 0;

  ///
  /// Returns the value at the specified index. For simple types the returned
  /// value will copy existing data and modifications to the value will not
  /// modify this object. For complex types (binary, dictionary and list) the
  /// returned value will reference existing data and modifications to the value
  /// will modify this object.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfValue> GetValue(size_t index) = 0;

  ///
  /// Returns the value at the specified index as type bool.
  ///
  /*--acf()--*/
  virtual bool GetBool(size_t index) = 0;

  ///
  /// Returns the value at the specified index as type int.
  ///
  /*--acf()--*/
  virtual int GetInt(size_t index) = 0;

  ///
  /// Returns the value at the specified index as type double.
  ///
  /*--acf()--*/
  virtual double GetDouble(size_t index) = 0;

  ///
  /// Returns the value at the specified index as type string.
  ///
  /*--acf()--*/
  virtual AcfString GetString(size_t index) = 0;

  ///
  /// Returns the value at the specified index as type binary. The returned
  /// value will reference existing data.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfBinaryValue> GetBinary(size_t index) = 0;

  ///
  /// Returns the value at the specified index as type dictionary. The returned
  /// value will reference existing data and modifications to the value will
  /// modify this object.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfDictionaryValue> GetDictionary(size_t index) = 0;

  ///
  /// Returns the value at the specified index as type list. The returned
  /// value will reference existing data and modifications to the value will
  /// modify this object.
  ///
  /*--acf()--*/
  virtual AcfRefPtr<AcfListValue> GetList(size_t index) = 0;

  ///
  /// Sets the value at the specified index. Returns true if the value was set
  /// successfully. If |value| represents simple data then the underlying data
  /// will be copied and modifications to |value| will not modify this object.
  /// If |value| represents complex data (binary, dictionary or list) then the
  /// underlying data will be referenced and modifications to |value| will
  /// modify this object.
  ///
  /*--acf()--*/
  virtual bool SetValue(size_t index, AcfRefPtr<AcfValue> value) = 0;

  ///
  /// Sets the value at the specified index as type null. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetNull(size_t index) = 0;

  ///
  /// Sets the value at the specified index as type bool. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetBool(size_t index, bool value) = 0;

  ///
  /// Sets the value at the specified index as type int. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetInt(size_t index, int value) = 0;

  ///
  /// Sets the value at the specified index as type double. Returns true if the
  /// value was set successfully.
  ///
  /*--acf()--*/
  virtual bool SetDouble(size_t index, double value) = 0;

  ///
  /// Sets the value at the specified index as type string. Returns true if the
  /// value was set successfully.
  ///
  /*--acf(optional_param=value)--*/
  virtual bool SetString(size_t index, const AcfString& value) = 0;

  ///
  /// Sets the value at the specified index as type binary. Returns true if the
  /// value was set successfully. If |value| is currently owned by another
  /// object then the value will be copied and the |value| reference will not
  /// change. Otherwise, ownership will be transferred to this object and the
  /// |value| reference will be invalidated.
  ///
  /*--acf()--*/
  virtual bool SetBinary(size_t index, AcfRefPtr<AcfBinaryValue> value) = 0;

  ///
  /// Sets the value at the specified index as type dict. Returns true if the
  /// value was set successfully. If |value| is currently owned by another
  /// object then the value will be copied and the |value| reference will not
  /// change. Otherwise, ownership will be transferred to this object and the
  /// |value| reference will be invalidated.
  ///
  /*--acf()--*/
  virtual bool SetDictionary(size_t index,
                             AcfRefPtr<AcfDictionaryValue> value) = 0;

  ///
  /// Sets the value at the specified index as type list. Returns true if the
  /// value was set successfully. If |value| is currently owned by another
  /// object then the value will be copied and the |value| reference will not
  /// change. Otherwise, ownership will be transferred to this object and the
  /// |value| reference will be invalidated.
  ///
  /*--acf()--*/
  virtual bool SetList(size_t index, AcfRefPtr<AcfListValue> value) = 0;
};

#endif  // ACF_INCLUDE_VALUES_H_
