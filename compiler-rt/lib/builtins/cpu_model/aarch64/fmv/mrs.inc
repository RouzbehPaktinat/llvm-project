#if __has_include(<sys/auxv.h>)
#include <sys/auxv.h>
#define HAVE_SYS_AUXV_H
#endif



static void __init_cpu_features_constructor(unsigned long hwcap,
                                            const __ifunc_arg_t *arg) {
#define setCPUFeature(F) __aarch64_cpu_features.features |= 1ULL << F
#define getCPUFeature(id, ftr) __asm__("mrs %0, " #id : "=r"(ftr))
#define extractBits(val, start, number)                                        \
  (val & ((1ULL << number) - 1ULL) << start) >> start
  unsigned long hwcap2 = 0;
  if (hwcap & _IFUNC_ARG_HWCAP)
    hwcap2 = arg->_hwcap2;
  if (hwcap & HWCAP_CRC32)
    setCPUFeature(FEAT_CRC);
  if (hwcap & HWCAP_FLAGM)
    setCPUFeature(FEAT_FLAGM);
  if (hwcap2 & HWCAP2_FLAGM2) {
    setCPUFeature(FEAT_FLAGM);
    setCPUFeature(FEAT_FLAGM2);
  }
  if (hwcap & HWCAP_SM3 && hwcap & HWCAP_SM4)
    setCPUFeature(FEAT_SM4);
  if (hwcap & HWCAP_ASIMDDP)
    setCPUFeature(FEAT_DOTPROD);
  if (hwcap & HWCAP_ASIMDFHM)
    setCPUFeature(FEAT_FP16FML);
  if (hwcap & HWCAP_FPHP) {
    setCPUFeature(FEAT_FP16);
    setCPUFeature(FEAT_FP);
  }
  if (hwcap & HWCAP_ASIMDRDM)
    setCPUFeature(FEAT_RDM);
  if (hwcap & HWCAP_ILRCPC)
    setCPUFeature(FEAT_RCPC2);
  if (hwcap & HWCAP_AES)
    setCPUFeature(FEAT_AES);
  if (hwcap & HWCAP_SHA2)
    setCPUFeature(FEAT_SHA2);
  if (hwcap & HWCAP_JSCVT)
    setCPUFeature(FEAT_JSCVT);
  if (hwcap & HWCAP_FCMA)
    setCPUFeature(FEAT_FCMA);
  if (hwcap & HWCAP_SB)
    setCPUFeature(FEAT_SB);
  if (hwcap & HWCAP_SSBS)
    setCPUFeature(FEAT_SSBS);
  if (hwcap2 & HWCAP2_MTE)
    setCPUFeature(FEAT_MEMTAG);
  if (hwcap2 & HWCAP2_SVEAES)
    setCPUFeature(FEAT_SVE_AES);
  if (hwcap2 & HWCAP2_SVEBITPERM)
    setCPUFeature(FEAT_SVE_BITPERM);
  if (hwcap2 & HWCAP2_SVESHA3)
    setCPUFeature(FEAT_SVE_SHA3);
  if (hwcap2 & HWCAP2_SVESM4)
    setCPUFeature(FEAT_SVE_SM4);
  if (hwcap2 & HWCAP2_DCPODP)
    setCPUFeature(FEAT_DPB2);
  if (hwcap & HWCAP_ATOMICS)
    setCPUFeature(FEAT_LSE);
  if (hwcap2 & HWCAP2_RNG)
    setCPUFeature(FEAT_RNG);
  if (hwcap2 & HWCAP2_I8MM)
    setCPUFeature(FEAT_I8MM);
  if (hwcap2 & HWCAP2_FRINT)
    setCPUFeature(FEAT_FRINTTS);
  if (hwcap2 & HWCAP2_SVEF32MM)
    setCPUFeature(FEAT_SVE_F32MM);
  if (hwcap2 & HWCAP2_SVEF64MM)
    setCPUFeature(FEAT_SVE_F64MM);
  if (hwcap2 & HWCAP2_RPRES)
    setCPUFeature(FEAT_RPRES);
  if (hwcap2 & HWCAP2_WFXT)
    setCPUFeature(FEAT_WFXT);
  if (hwcap2 & HWCAP2_SME)
    setCPUFeature(FEAT_SME);
  if (hwcap2 & HWCAP2_SME_I16I64)
    setCPUFeature(FEAT_SME_I64);
  if (hwcap2 & HWCAP2_SME_F64F64)
    setCPUFeature(FEAT_SME_F64);
  if (hwcap2 & HWCAP2_MOPS)
    setCPUFeature(FEAT_MOPS);
  if (hwcap & HWCAP_CPUID) {
    unsigned long ftr;
    getCPUFeature(ID_AA64PFR1_EL1, ftr);
    // ID_AA64PFR1_EL1.MTE >= 0b0001
    if (extractBits(ftr, 8, 4) >= 0x1)
      setCPUFeature(FEAT_MEMTAG);
    // ID_AA64PFR1_EL1.SSBS == 0b0001
    if (extractBits(ftr, 4, 4) == 0x1)
      setCPUFeature(FEAT_SSBS);
    // ID_AA64PFR1_EL1.SME == 0b0010
    if (extractBits(ftr, 24, 4) == 0x2)
      setCPUFeature(FEAT_SME2);
    getCPUFeature(ID_AA64PFR0_EL1, ftr);
    // ID_AA64PFR0_EL1.FP != 0b1111
    if (extractBits(ftr, 16, 4) != 0xF) {
      setCPUFeature(FEAT_FP);
      // ID_AA64PFR0_EL1.AdvSIMD has the same value as ID_AA64PFR0_EL1.FP
      setCPUFeature(FEAT_SIMD);
    }
    // ID_AA64PFR0_EL1.SVE != 0b0000
    if (extractBits(ftr, 32, 4) != 0x0) {
      // get ID_AA64ZFR0_EL1, that name supported
      // if sve enabled only
      getCPUFeature(S3_0_C0_C4_4, ftr);
      // ID_AA64ZFR0_EL1.SVEver == 0b0000
      if (extractBits(ftr, 0, 4) == 0x0)
        setCPUFeature(FEAT_SVE);
      // ID_AA64ZFR0_EL1.SVEver == 0b0001
      if (extractBits(ftr, 0, 4) == 0x1)
        setCPUFeature(FEAT_SVE2);
    }
    getCPUFeature(ID_AA64ISAR0_EL1, ftr);
    // ID_AA64ISAR0_EL1.SHA3 != 0b0000
    if (extractBits(ftr, 32, 4) != 0x0)
      setCPUFeature(FEAT_SHA3);
    getCPUFeature(ID_AA64ISAR1_EL1, ftr);
    // ID_AA64ISAR1_EL1.DPB >= 0b0001
    if (extractBits(ftr, 0, 4) >= 0x1)
      setCPUFeature(FEAT_DPB);
    // ID_AA64ISAR1_EL1.LRCPC != 0b0000
    if (extractBits(ftr, 20, 4) != 0x0)
      setCPUFeature(FEAT_RCPC);
    // ID_AA64ISAR1_EL1.LRCPC == 0b0011
    if (extractBits(ftr, 20, 4) == 0x3)
      setCPUFeature(FEAT_RCPC3);
    // ID_AA64ISAR1_EL1.SPECRES == 0b0001
    if (extractBits(ftr, 40, 4) == 0x2)
      setCPUFeature(FEAT_PREDRES);
    // ID_AA64ISAR1_EL1.BF16 != 0b0000
    if (extractBits(ftr, 44, 4) != 0x0)
      setCPUFeature(FEAT_BF16);
    // ID_AA64ISAR1_EL1.LS64 >= 0b0001
    if (extractBits(ftr, 60, 4) >= 0x1)
      setCPUFeature(FEAT_LS64);
  } else {
    // Set some features in case of no CPUID support
    if (hwcap & (HWCAP_FP | HWCAP_FPHP)) {
      setCPUFeature(FEAT_FP);
      // FP and AdvSIMD fields have the same value
      setCPUFeature(FEAT_SIMD);
    }
    if (hwcap & HWCAP_DCPOP || hwcap2 & HWCAP2_DCPODP)
      setCPUFeature(FEAT_DPB);
    if (hwcap & HWCAP_LRCPC || hwcap & HWCAP_ILRCPC)
      setCPUFeature(FEAT_RCPC);
    if (hwcap2 & HWCAP2_BF16 || hwcap2 & HWCAP2_EBF16)
      setCPUFeature(FEAT_BF16);
    if (hwcap2 & HWCAP2_SVE2 && hwcap & HWCAP_SVE)
      setCPUFeature(FEAT_SVE2);
    if (hwcap & HWCAP_SHA3)
      setCPUFeature(FEAT_SHA3);
  }
  setCPUFeature(FEAT_INIT);
}
