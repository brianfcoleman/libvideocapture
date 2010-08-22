#ifndef TYPE_LIBRARY_H
#define TYPE_LIBRARY_H

#include <vector>
#include <map>
#include <string>
#include "boost/utility.hpp"
#include "oaidl.h"

namespace VideoCapture {

class TypeLibrary :
  private boost::noncopyable {
public:
  TypeLibrary(const std::wstring& typeLibraryName,
              const std::vector<const std::wstring>& interfaceIdentifierNames,
              const std::vector<const std::wstring>& classIdentifierNames);
  virtual ~TypeLibrary();
  IID interfaceIdByName(const std::wstring& name) const;
  CLSID classIdByName(const std::wstring& name) const;
  operator bool() const {
    return m_isInitialized;
  }
private:
  std::map<const std::wstring, IID> m_interfaceIdentifiersByName;
  std::map<const std::wstring, CLSID> m_classIdentifiersByName;
  bool m_isInitialized;
  bool init(const std::wstring& typeLibraryName,
            const std::vector<const std::wstring>& interfaceIdentifierNames,
            const std::vector<const std::wstring>& classIdentifierNames);
};

}

#endif // TYPE_LIBRARY_H