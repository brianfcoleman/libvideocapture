#include "TypeLibrary.hpp"

#define BOOST_BIND_ENABLE_STDCALL
#define BOOST_MEM_FN_ENABLE_STDCALL

#include <iostream>
#include "boost/shared_ptr.hpp"
#include "boost/shared_array.hpp"
#include "boost/cstdint.hpp"
#include "COMUtilities.hpp"
#include "GUIDUtilities.hpp"

namespace VideoCapture {

static boost::shared_ptr<ITypeLib>
typeLibSharedPtr(const std::wstring& fileNameTypeLib);

static const GUID guidOfTypeKind(
    const boost::shared_ptr<ITypeLib>& typeLib,
    const std::wstring& nameTypeInfo,
    const TYPEKIND typeKind);

static const GUID uniqueGUIDOfTypeKind(
    const std::vector<boost::shared_ptr<ITypeInfo>>& typeInfoSharedPtrVector,
    const TYPEKIND typeKind);

static boost::shared_ptr<TYPEATTR> typeAttributeSharedPtr(
    boost::shared_ptr<ITypeInfo>& typeInfo);

static boost::shared_array<wchar_t> copyWideCharArrayFromString(
    const std::wstring& wideCharString);

template<typename TypeKindGUID> bool mapNamesToGUIDs(
    const boost::shared_ptr<ITypeLib>& typeLib,
    const TYPEKIND typeKind,
    const std::vector<const std::wstring>& names,
    std::map<const std::wstring, TypeKindGUID>& guidsByName);

template<typename TypeKindGUID> TypeKindGUID findGUIDByName(
    const std::wstring& name,
    const std::map<const std::wstring, TypeKindGUID>& guidsByName);

static boost::shared_ptr<ITypeLib> typeLibSharedPtr(
    const std::wstring& fileNameTypeLib) {
  ITypeLib* pTypeLib = 0;
  HRESULT result =
  LoadTypeLib(
      static_cast<const OLECHAR FAR*>(fileNameTypeLib.c_str()),
      static_cast<ITypeLib FAR* FAR*>(&pTypeLib));
  if (result != S_OK) {
    boost::shared_ptr<ITypeLib> typeLibSharedPtr;
    return typeLibSharedPtr;
  }
  boost::shared_ptr<ITypeLib> typeLibSharedPtr(
      comInterfaceSharedPtr<ITypeLib>(pTypeLib));
  return typeLibSharedPtr;
}

static const GUID guidOfTypeKind(
    const boost::shared_ptr<ITypeLib>& typeLib,
    const std::wstring& nameTypeInfo,
    const TYPEKIND typeKind) {
  if (!typeLib) {
    return emptyGUID();
  }
  boost::shared_array<wchar_t> wideCharArrayNameTypeInfo(
      copyWideCharArrayFromString(nameTypeInfo));
  if (!wideCharArrayNameTypeInfo) {
    return emptyGUID();
  }
  UINT countTypeInfo = typeLib->GetTypeInfoCount();
  if (!countTypeInfo) {
    return emptyGUID();
  }
  boost::shared_array<ITypeInfo*> typeInfoPtrArray(
      new ITypeInfo*[countTypeInfo]);
  boost::shared_array<MEMBERID> memberIdArray(new MEMBERID[countTypeInfo]);
  long hashValue = 0;
  USHORT countFound = countTypeInfo;
  HRESULT result =
  typeLib->FindName(
      static_cast<OLECHAR FAR*>(wideCharArrayNameTypeInfo.get()),
      hashValue,
      static_cast<ITypeInfo FAR* FAR*>(typeInfoPtrArray.get()),
      static_cast<MEMBERID FAR*>(memberIdArray.get()),
      &countFound);
  if (result != S_OK) {
    return emptyGUID();
  }
  std::wcout << "Found " << countFound;
  std::wcout << " ITypeInfo with name " << nameTypeInfo;
  std::wcout << " in ITypeLib" << std::endl;
  if (!countFound) {
    return emptyGUID();
  }
  std::vector<boost::shared_ptr<ITypeInfo>> typeInfoSharedPtrVector(
      comInterfaceSharedPtrVector(typeInfoPtrArray.get(), countFound));
  return uniqueGUIDOfTypeKind(typeInfoSharedPtrVector, typeKind);
}

static const GUID uniqueGUIDOfTypeKind(
    const std::vector<boost::shared_ptr<ITypeInfo>>& typeInfoSharedPtrVector,
    const TYPEKIND typeKind) {
  if (!typeInfoSharedPtrVector.size()) {
    return emptyGUID();
  }
  std::size_t countGUIDsOfTypeKind = 0;
  boost::shared_ptr<TYPEATTR> typeAttributeOfTypeKind;
  for (std::vector<boost::shared_ptr<ITypeInfo>>::const_iterator
           iterator(typeInfoSharedPtrVector.begin());
       iterator != typeInfoSharedPtrVector.end();
       ++iterator) {
    boost::shared_ptr<ITypeInfo> typeInfo(*iterator);
    boost::shared_ptr<TYPEATTR> typeAttribute(typeAttributeSharedPtr(typeInfo));
    if (!typeAttribute) {
      continue;
    }
    if (typeAttribute->typekind != typeKind) {
      continue;
    }
    ++countGUIDsOfTypeKind;
    if (countGUIDsOfTypeKind > 1) {
      return emptyGUID();
    }
    typeAttributeOfTypeKind = typeAttribute;
  }
  if (countGUIDsOfTypeKind != 1) {
    return emptyGUID();
  }
  if (!typeAttributeOfTypeKind) {
    return emptyGUID();
  }
  std::wcout << "GUID " << typeAttributeOfTypeKind->guid;
  std::wcout << " is unique and of TYPEKIND" << std::endl;
  return typeAttributeOfTypeKind->guid;
}

static boost::shared_ptr<TYPEATTR> typeAttributeSharedPtr(
    boost::shared_ptr<ITypeInfo>& typeInfo) {
  if (!typeInfo) {
    boost::shared_ptr<TYPEATTR> typeAttributeSharedPtr;
    return typeAttributeSharedPtr;
  }
  TYPEATTR* pTypeAttribute = 0;
  HRESULT result = typeInfo->GetTypeAttr(
      static_cast<TYPEATTR FAR* FAR*>(&pTypeAttribute));
  if (result != S_OK) {
    boost::shared_ptr<TYPEATTR> typeAttributeSharedPtr;
    return typeAttributeSharedPtr;
  }
  boost::shared_ptr<TYPEATTR> typeAttributeSharedPtr(
      pTypeAttribute,
      boost::bind(&ITypeInfo::ReleaseTypeAttr, typeInfo, _1));
  return typeAttributeSharedPtr;
}

static boost::shared_array<wchar_t> copyWideCharArrayFromString(
    const std::wstring& wideCharString) {
  if (!wideCharString.size()) {
    boost::shared_array<wchar_t> wideCharSharedArray;
    return wideCharSharedArray;
  }
  std::size_t sizeWideCharStringWithNulTerminator = wideCharString.size() + 1;
  boost::shared_array<wchar_t> wideCharSharedArray(
      new wchar_t[sizeWideCharStringWithNulTerminator]);
  if (!wideCharSharedArray) {
    return wideCharSharedArray;
  }
  wcsncpy_s(
      wideCharSharedArray.get(),
      sizeWideCharStringWithNulTerminator,
      wideCharString.c_str(),
      sizeWideCharStringWithNulTerminator);
  return wideCharSharedArray;
}

template<typename TypeKindGUID> bool mapNamesToGUIDs(
    const boost::shared_ptr<ITypeLib>& typeLib,
    const TYPEKIND typeKind,
    const std::vector<const std::wstring>& names,
    std::map<const std::wstring, TypeKindGUID>& guidsByName) {
  if (!typeLib) {
    return false;
  }
  if (!names.size()) {
    return false;
  }
  for(std::vector<const std::wstring>::const_iterator iterator(names.begin());
      iterator != names.end();
      ++iterator) {
    const std::wstring& nameTypeInfo(*iterator);
    const GUID guid(guidOfTypeKind(typeLib, nameTypeInfo, typeKind));
    if (isEmptyGUID(guid)) {
      guidsByName.clear();
      return false;
    }
    guidsByName[nameTypeInfo] = static_cast<TypeKindGUID>(guid);
  }
  return true;
}

template<typename TypeKindGUID> TypeKindGUID findGUIDByName(
    const std::wstring& name,
    const std::map<const std::wstring, TypeKindGUID>& guidsByName) {
  std::map<const std::wstring, TypeKindGUID>::const_iterator
  iterator(guidsByName.find(name));
  if (iterator == guidsByName.end()) {
    return emptyGUID();
  }
  TypeKindGUID guid(iterator->second);
  std::wcout << "Requested GUID " << static_cast<const GUID>(guid);
  std::wcout << " with name " << name << std::endl;
  return guid;
}

TypeLibrary::TypeLibrary(const std::wstring& typeLibraryName,
                const std::vector<const std::wstring>& interfaceIdentifierNames,
                const std::vector<const std::wstring>& classIdentifierNames) :
  boost::noncopyable() {
  m_isInitialized = init(typeLibraryName,
                       interfaceIdentifierNames,
                       classIdentifierNames);
}

TypeLibrary::~TypeLibrary() {

}

bool TypeLibrary::init(const std::wstring& typeLibraryName,
                const std::vector<const std::wstring>& interfaceIdentifierNames,
                const std::vector<const std::wstring>& classIdentifierNames) {
  boost::shared_ptr<ITypeLib> typeLib = typeLibSharedPtr(typeLibraryName);
  if (!typeLib) {
    return false;
  }
  if (!mapNamesToGUIDs(typeLib,
                       TKIND_INTERFACE,
                       interfaceIdentifierNames,
                       m_interfaceIdentifiersByName)) {
    return false;
  }
  if (!mapNamesToGUIDs(typeLib,
                       TKIND_COCLASS,
                       classIdentifierNames,
                       m_classIdentifiersByName)) {
    return false;
  }
  return true;
}

IID TypeLibrary::interfaceIdByName(const std::wstring& name) const {
  if (!m_isInitialized) {
    return emptyGUID();
  }
  return findGUIDByName(name, m_interfaceIdentifiersByName);
}

CLSID TypeLibrary::classIdByName(const std::wstring& name) const {
  if (!m_isInitialized) {
    return emptyGUID();
  }
  return findGUIDByName(name, m_classIdentifiersByName);
}

} // VideoCapture
