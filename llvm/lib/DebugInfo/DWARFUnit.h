//===-- DWARFUnit.h ---------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_DEBUGINFO_DWARFUNIT_H
#define LLVM_LIB_DEBUGINFO_DWARFUNIT_H

#include "DWARFDebugAbbrev.h"
#include "DWARFDebugInfoEntry.h"
#include "DWARFDebugRangeList.h"
#include "DWARFRelocMap.h"
#include <vector>

namespace llvm {

namespace object {
class ObjectFile;
}

class DWARFContext;
class DWARFDebugAbbrev;
class StringRef;
class raw_ostream;
class DWARFUnit;

/// Base class for all DWARFUnitSection classes. This provides the
/// functionality common to all unit types.
class DWARFUnitSectionBase {
public:
  /// Returns the Unit that contains the given section offset in the
  /// same section this Unit originated from.
  virtual DWARFUnit *getUnitForOffset(uint32_t Offset) const = 0;

protected:
  ~DWARFUnitSectionBase() {}
};

/// Concrete instance of DWARFUnitSection, specialized for one Unit type.
template<typename UnitType>
class DWARFUnitSection final : public SmallVector<std::unique_ptr<UnitType>, 1>,
                               public DWARFUnitSectionBase {

  struct UnitOffsetComparator {
    bool operator()(const std::unique_ptr<UnitType> &LHS,
                    const std::unique_ptr<UnitType> &RHS) const {
      return LHS->getOffset() < RHS->getOffset();
    }
    bool operator()(const std::unique_ptr<UnitType> &LHS,
                    uint32_t RHS) const {
      return LHS->getOffset() < RHS;
    }
    bool operator()(uint32_t LHS,
                    const std::unique_ptr<UnitType> &RHS) const {
      return LHS < RHS->getOffset();
    }
  };

public:
  typedef llvm::SmallVectorImpl<std::unique_ptr<UnitType>> UnitVector;
  typedef typename UnitVector::iterator iterator;
  typedef llvm::iterator_range<typename UnitVector::iterator> iterator_range;

  UnitType *getUnitForOffset(uint32_t Offset) const {
    auto *CU = std::lower_bound(this->begin(), this->end(), Offset,
                                UnitOffsetComparator());
    if (CU != this->end())
      return CU->get();
    return nullptr;
  }
};

class DWARFUnit {
  DWARFContext &Context;

  const DWARFDebugAbbrev *Abbrev;
  StringRef InfoSection;
  StringRef RangeSection;
  uint32_t RangeSectionBase;
  StringRef StringSection;
  StringRef StringOffsetSection;
  StringRef AddrOffsetSection;
  uint32_t AddrOffsetSectionBase;
  const RelocAddrMap *RelocMap;
  bool isLittleEndian;
  const DWARFUnitSectionBase &UnitSection;

  uint32_t Offset;
  uint32_t Length;
  uint16_t Version;
  const DWARFAbbreviationDeclarationSet *Abbrevs;
  uint8_t AddrSize;
  uint64_t BaseAddr;
  // The compile unit debug information entry items.
  std::vector<DWARFDebugInfoEntryMinimal> DieArray;

  class DWOHolder {
    object::OwningBinary<object::ObjectFile> DWOFile;
    std::unique_ptr<DWARFContext> DWOContext;
    DWARFUnit *DWOU;
  public:
    DWOHolder(StringRef DWOPath);
    DWARFUnit *getUnit() const { return DWOU; }
  };
  std::unique_ptr<DWOHolder> DWO;

protected:
  virtual bool extractImpl(DataExtractor debug_info, uint32_t *offset_ptr);
  /// Size in bytes of the unit header.
  virtual uint32_t getHeaderSize() const { return 11; }

public:
  DWARFUnit(DWARFContext& Context, const DWARFDebugAbbrev *DA, StringRef IS,
            StringRef RS, StringRef SS, StringRef SOS, StringRef AOS,
            const RelocAddrMap *M, bool LE, const DWARFUnitSectionBase &UnitSection);

  virtual ~DWARFUnit();

  DWARFContext& getContext() const { return Context; }

  StringRef getStringSection() const { return StringSection; }
  StringRef getStringOffsetSection() const { return StringOffsetSection; }
  void setAddrOffsetSection(StringRef AOS, uint32_t Base) {
    AddrOffsetSection = AOS;
    AddrOffsetSectionBase = Base;
  }
  void setRangesSection(StringRef RS, uint32_t Base) {
    RangeSection = RS;
    RangeSectionBase = Base;
  }

  bool getAddrOffsetSectionItem(uint32_t Index, uint64_t &Result) const;
  // FIXME: Result should be uint64_t in DWARF64.
  bool getStringOffsetSectionItem(uint32_t Index, uint32_t &Result) const;

  DataExtractor getDebugInfoExtractor() const {
    return DataExtractor(InfoSection, isLittleEndian, AddrSize);
  }
  DataExtractor getStringExtractor() const {
    return DataExtractor(StringSection, false, 0);
  }

  const RelocAddrMap *getRelocMap() const { return RelocMap; }

  bool extract(DataExtractor debug_info, uint32_t* offset_ptr);

  /// extractRangeList - extracts the range list referenced by this compile
  /// unit from .debug_ranges section. Returns true on success.
  /// Requires that compile unit is already extracted.
  bool extractRangeList(uint32_t RangeListOffset,
                        DWARFDebugRangeList &RangeList) const;
  void clear();
  uint32_t getOffset() const { return Offset; }
  uint32_t getNextUnitOffset() const { return Offset + Length + 4; }
  uint32_t getLength() const { return Length; }
  uint16_t getVersion() const { return Version; }
  const DWARFAbbreviationDeclarationSet *getAbbreviations() const {
    return Abbrevs;
  }
  uint8_t getAddressByteSize() const { return AddrSize; }
  uint64_t getBaseAddress() const { return BaseAddr; }

  void setBaseAddress(uint64_t base_addr) {
    BaseAddr = base_addr;
  }

  const DWARFDebugInfoEntryMinimal *
  getCompileUnitDIE(bool extract_cu_die_only = true) {
    extractDIEsIfNeeded(extract_cu_die_only);
    return DieArray.empty() ? nullptr : &DieArray[0];
  }

  const char *getCompilationDir();
  uint64_t getDWOId();

  void collectAddressRanges(DWARFAddressRangesVector &CURanges);

  /// getInlinedChainForAddress - fetches inlined chain for a given address.
  /// Returns empty chain if there is no subprogram containing address. The
  /// chain is valid as long as parsed compile unit DIEs are not cleared.
  DWARFDebugInfoEntryInlinedChain getInlinedChainForAddress(uint64_t Address);

  /// getUnitSection - Return the DWARFUnitSection containing this unit.
  const DWARFUnitSectionBase &getUnitSection() const { return UnitSection; }

private:
  /// Size in bytes of the .debug_info data associated with this compile unit.
  size_t getDebugInfoSize() const { return Length + 4 - getHeaderSize(); }

  /// extractDIEsIfNeeded - Parses a compile unit and indexes its DIEs if it
  /// hasn't already been done. Returns the number of DIEs parsed at this call.
  size_t extractDIEsIfNeeded(bool CUDieOnly);
  /// extractDIEsToVector - Appends all parsed DIEs to a vector.
  void extractDIEsToVector(bool AppendCUDie, bool AppendNonCUDIEs,
                           std::vector<DWARFDebugInfoEntryMinimal> &DIEs) const;
  /// setDIERelations - We read in all of the DIE entries into our flat list
  /// of DIE entries and now we need to go back through all of them and set the
  /// parent, sibling and child pointers for quick DIE navigation.
  void setDIERelations();
  /// clearDIEs - Clear parsed DIEs to keep memory usage low.
  void clearDIEs(bool KeepCUDie);

  /// parseDWO - Parses .dwo file for current compile unit. Returns true if
  /// it was actually constructed.
  bool parseDWO();

  /// getSubprogramForAddress - Returns subprogram DIE with address range
  /// encompassing the provided address. The pointer is alive as long as parsed
  /// compile unit DIEs are not cleared.
  const DWARFDebugInfoEntryMinimal *getSubprogramForAddress(uint64_t Address);
};

}

#endif
